/*
 * SCADA host keepalive daemon (ensuring sort of "high-availability")
 * NOTES
 *   no support for DNS/hostname translation to IPs (getaddrinfo() freeaddrinfo())
 *   no support for IPv6
 *   floating IP not incorporated (needs to be solved in <cmd>)
 *   master responds to each incoming challenge msg disregarding its origin IP
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>  // inet_aton()
#include <arpa/inet.h>  // htons() htonl()
#include <sys/select.h>
#include <sys/time.h>  // gettimeofday()
#include <time.h>  // nanosleep()
#include <errno.h>
#include <limits.h>  // <num_type>_MAX
#include <string.h>  // memset()
#include <stdlib.h>  // exit()
#include <signal.h>  // sigfillset() sigaction()
#include <unistd.h>  // close() execl()
#include <sys/wait.h>  // wait()

#define MAX(x, y) ((x > y) ? (x) : (y))
#define MIN(x, y) ((x < y) ? (x) : (y))

#define BUF_SIZE (1 << 8)
#define MSG_CHALLENGE "HELLO MASTER, ARE YOU ALIVE?"
#define MSG_RESPONSE "HI SLAVE, YES, I AM!"
#define SLEN(s) (sizeof(s) -1)

#define FILL_TIMESPEC(tspec, time_ms) do { \
  time_t _sec = (time_ms) / 1000; \
  (tspec).tv_sec = _sec; \
  /* minimum time for nanosleep is 0 and maximum is 1000 million */ \
  (tspec).tv_nsec = MIN(1000 * 1000000, MAX(0, \
        1000000 * ((long)(time_ms) - (long)_sec * 1000))); \
} while (0)

#define E_EXIT(s) do { \
  fprintf(stderr, s); \
  exit(EXIT_FAILURE); \
} while (0);

#define STRTOL(i, str, e_msg) do { \
  errno = 0; \
  char *_s; \
  i = strtol((str), &_s, 10); \
  if (*(str) == '\0' || errno || *_s != '\0') \
    E_EXIT(e_msg); \
} while (0)

typedef enum {
  STATE_MASTER,
  STATE_SLAVE,
  STATE_UNKNOWN
} state_t;

typedef struct {
  state_t state;
  int port;
  char *cmd;
  unsigned int period_ms;
  unsigned int timeout_ms;
  struct in_addr master_ipv4;
  int master_port;
} args_t;

struct {
  int sock;
  bool chld_running;
} global_vars;

extern int getopt(int, char * const *, const char *);
extern char *optarg;
extern int optind, opterr, optopt;

void parse_args(int argc, char *argv[], args_t *args) {
  args->state        = STATE_UNKNOWN;
  args->port        = 0;  /* pick a random port by default */
  args->cmd         = "true";
  args->period_ms   = 100;
  args->timeout_ms  = 10;
  memset(&args->master_ipv4, 0, sizeof(struct in_addr));
  args->master_port = 0;

  /* no argument */
  if (argc == 1) {
    printf(
        "SYNOPSIS\n"
        "  -m <local_port>\n"
        "  -s <cmd> [-p <msg_period>] [-t <timeout>] <master_IPv4> <master_port>}\n"
        "OPTIONS\n"
        "  -m\n"
        "    run as master (if <local_port> is 0, let the system\n"
        "    choose one itself)\n"
        "  -s <cmd>\n"
        "    run as slave and poll master\n"
        "    if timeout occurs, run sh -c <cmd> (notice no implicit <cmd> quoting)\n"
        "    <cmd> has to process SIGTERM and if possible call exec\n"
        "  -p <msg_period>\n"
        "    time in ms between poll messages; by default 100ms\n"
        "  -t <timeout>\n"
        "    time in ms to wait for response from master; by default 10ms\n"
        "\n"
        "  NOTE: It is recommended to start this daemon on master first\n"
        "        and when exiting, it is recommended to stop this daemon\n"
        "        first on slave.\n");
    exit(EXIT_SUCCESS);
  }

  int opt;
  opterr = 0;  /* disable getopt() writing to stderr */
  long tmp;

  while ((opt = getopt(argc, argv, ":m:s:p:t:")) != -1) {
    switch (opt) {
      case 'm':
        if (args->state != STATE_UNKNOWN)
          E_EXIT("ERR -m/-s mismatch.\n");
        args->state = STATE_MASTER;
        #define _E_M "ERR bad <local_port> number given.\n"
        STRTOL(tmp, optarg, _E_M);
        if (tmp < 0 || tmp > INT_MAX) E_EXIT(_E_M);
        args->port = (int)tmp;
        break;
      case 's':
        if (args->state != STATE_UNKNOWN)
          E_EXIT("ERR -m/-s mismatch.\n");
        args->state = STATE_SLAVE;
        args->cmd = optarg;
        break;
      case 'p':
        if (args->state != STATE_SLAVE)
          E_EXIT("ERR -s required.\n");
        #define _E_P "ERR bad <msg_period> number given.\n"
        STRTOL(tmp, optarg, _E_P);
        if (tmp < 0 || tmp > (long)UINT_MAX) E_EXIT(_E_P);
        args->period_ms = (unsigned int)tmp;
        break;
      case 't':
        if (args->state != STATE_SLAVE)
          E_EXIT("ERR -s required.\n");
        #define _E_T "ERR bad <timeout> number given.\n"
        STRTOL(tmp, optarg, _E_T);
        if (tmp < 0 || tmp > (long)UINT_MAX) E_EXIT(_E_T);
        args->timeout_ms = (unsigned int)tmp;
        break;
      case ':':
        E_EXIT("ERR -m -s -p -t demand a parameter.\n");
      default:
        E_EXIT("ERR unexpected option.\n");
    }
  }

  if (args->state == STATE_UNKNOWN) {
    E_EXIT("ERR -m or -s required.\n");
  }
  else if (args->state == STATE_MASTER) {
    /* optind points to next argument (after the current one) in argv */
    if (optind != argc && optind != argc -1)
      E_EXIT("ERR -m has only one parameter.\n");
  }
  else {
    if (optind != argc -2)
      E_EXIT("ERR <master_IPv4> and <master_port> are mandatory, nothing more, nothing less.\n");
    if (! inet_aton(argv[optind++], &args->master_ipv4))
      E_EXIT("ERR bad <master_IPv4> address given.\n");
    #define _E_MASTER_PORT "ERR bad <master_port> number.\n"
    STRTOL(tmp, argv[optind], _E_MASTER_PORT);
    if (tmp < 0 || tmp > INT_MAX) E_EXIT(_E_MASTER_PORT);
    args->master_port = (int)tmp;
  }
}

void quit_handler(int x) {
  x = x;
  close(global_vars.sock);
  exit(EXIT_SUCCESS);
}

void chld_handler(int x) {
  x = x;
  /* avoid zombies */
  wait(NULL);
  global_vars.chld_running = false;
}

unsigned int tv2ms(struct timeval *tv) {
  return tv->tv_sec * 1000 + tv->tv_usec / 1000;
}

struct timeval *ms2tv(struct timeval *tv, unsigned int ms) {
  tv->tv_sec = ms / 1000;
  tv->tv_usec = (ms - (tv->tv_sec * 1000)) * 1000;
  return tv;
}

int main(int argc, char *argv[]) {
  args_t args;
  parse_args(argc, argv, &args);

  sigset_t sset;
  sigfillset(&sset);  /* block all signals while in handler */
  struct sigaction signew = {
    .sa_handler    = quit_handler,
    //.sa_sigaction  = quit_sigaction,  /* may overlap with sa_handler => do not use both */
    .sa_mask       = sset,
    .sa_flags      = 0,
  };
  sigaction(SIGTERM, &signew, NULL);  /* termination */
  sigaction(SIGHUP,  &signew, NULL);  /* hangup */
  sigaction(SIGINT,  &signew, NULL);  /* interrupt */
  sigaction(SIGQUIT, &signew, NULL);

  signew.sa_handler = chld_handler;
  sigaction(SIGCHLD, &signew, NULL);  /* child terminated/stopped/... */
  global_vars.chld_running = false;

  char buf[BUF_SIZE];
  buf[BUF_SIZE -1] = '\0';

  if (args.state == STATE_SLAVE) {
    pid_t pid = -1;

    if ((global_vars.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      perror("socket()");
      return EXIT_FAILURE;
    }

    struct sockaddr_in sa_dst;  /* man 7 ip */
    memset(&sa_dst, 0, sizeof(sa_dst));
    sa_dst.sin_family      = AF_INET;
    sa_dst.sin_addr.s_addr = args.master_ipv4.s_addr;
    sa_dst.sin_port        = htons(args.master_port);
    struct timespec tspec;
    FILL_TIMESPEC(tspec, 0);

    int a = 0;
    struct timespec b;
    /* _always_ sleep for tspec period */
    for (;; a = nanosleep(&tspec, &b)) {
      /* nanosleep() interrupted */
      if (a == -1) {
        tspec.tv_sec  = b.tv_sec;
        tspec.tv_nsec = b.tv_nsec;
        continue;
      }
      FILL_TIMESPEC(tspec, args.period_ms);

      /* UDP packet */
      if (sendto(global_vars.sock, MSG_CHALLENGE, SLEN(MSG_CHALLENGE), 0,
            (struct sockaddr *)&sa_dst, sizeof(sa_dst)) != SLEN(MSG_CHALLENGE)) {
        perror("ERR [ignoring] sendto()");
        continue;
      }

      struct timeval timeout;
      ms2tv(&timeout, args.timeout_ms);
      /* call select() in a loop until the whole timeout expires */
      for (;;) {
        fd_set fds;
        FD_ZERO(&fds);  /* get empty set of fd's to be checked */
        FD_SET(global_vars.sock, &fds);  /* add socket to the set */

        struct timeval tv;
        /* start measuring processing duration */
        gettimeofday(&tv, NULL);
        unsigned int start_ms = tv2ms(&tv);

        /* block until change in any of the sockets in group or timeout occur */
        int x = select(global_vars.sock +1, &fds, NULL, NULL, &timeout);

        if (x == -1) {
          /* we were interrupted => calculate diff and loop again */
          if (errno == EINTR) {
            x = 0;
          }
          else {
            perror("select()");
            break;  /* start over with args.period_ms */
          }
        }

        /* select_timeout || irrelevant_things_happened */
        if (x == 0 || ! (FD_ISSET(global_vars.sock, &fds))) {
          gettimeofday(&tv, NULL);
          unsigned int diff_ms = tv2ms(&tv) - start_ms;

          /* have we already waited in select() enough? */
          if (diff_ms < args.timeout_ms) {
            ms2tv(&timeout, (diff_ms > args.timeout_ms) ? 0 :
                args.timeout_ms - diff_ms);
          }
          else {
            if (! global_vars.chld_running) {
              pid = fork();

              switch (pid) {
                case -1:
                  perror("fork()");
                  break;
                case 0:
                  if (close(global_vars.sock) == -1)
                    perror("close(sock)");

                  signew.sa_handler = SIG_DFL;
                  //sigaction(SIGTERM, &signew, NULL);
                  sigaction(SIGHUP,  &signew, NULL);
                  sigaction(SIGINT,  &signew, NULL);
                  sigaction(SIGQUIT, &signew, NULL);
                  sigaction(SIGCHLD, &signew, NULL);

                  if (execl("/bin/sh", "sh", "-c", args.cmd,
                        (char *)NULL) == -1) {
                    perror("execl()");
                    exit(1);
                  }
                default:
                  global_vars.chld_running = true;
              }
            }

            /* start over */
            gettimeofday(&tv, NULL);
            FILL_TIMESPEC(tspec, args.period_ms - (tv2ms(&tv) - start_ms));
            break;
          }
        }
        /* the excess data gets discarded */
        else if (recvfrom(global_vars.sock, buf, BUF_SIZE, 0, NULL, NULL) ==
            SLEN(MSG_RESPONSE) &&
            ! strncmp(buf, MSG_RESPONSE, SLEN(MSG_RESPONSE))) {
          if (global_vars.chld_running) {
            /* under QNX, the child has to trap SIGTERM, because the
               default behavior of direct child is to resends the signal
               to its parent => terminating this daemon! */
            if (kill(pid, SIGTERM) == -1)
              perror("ERR [ignoring] kill()");
          }

          /* start over */
          gettimeofday(&tv, NULL);
          FILL_TIMESPEC(tspec, args.period_ms - (tv2ms(&tv) - start_ms));
          break;
        }
        else {
          fprintf(stderr, "WARN: Unexpected msg received"
                          "(first 10B: %10s), ignoring.\n", buf);
          gettimeofday(&tv, NULL);
          unsigned int diff_ms = tv2ms(&tv) - start_ms;
          ms2tv(&timeout, (diff_ms > args.timeout_ms ) ? 0 :
              args.timeout_ms - diff_ms);
        }
      } // for(;;)
    } // for(;;)
  }
  /* STATE_MASTER */
  else {
    if ((global_vars.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      perror(NULL);
      return EXIT_FAILURE;
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(args.port);
    inet_aton("127.0.0.1", &sa.sin_addr);
    // FIXME sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(global_vars.sock, (struct sockaddr *)&sa, sizeof(sa))) == -1) {
      perror("bind()");
      close(global_vars.sock);
      return EXIT_FAILURE;
    }

    socklen_t address_len = sizeof(sa);
    getsockname(global_vars.sock, (struct sockaddr *)&sa, &address_len);
    printf("using port: %d\n", ntohs(sa.sin_port));
    fflush(stdout);

    for (;;) {
      /* reuse sa */
      if (recvfrom(global_vars.sock, buf, BUF_SIZE, 0,
            (struct sockaddr *)&sa, &address_len) != SLEN(MSG_CHALLENGE)) {
        perror("ERR [ignoring] recvfrom()");
        continue;
      }

      if (strncmp(buf, MSG_CHALLENGE, SLEN(MSG_CHALLENGE))) {
        fprintf(stderr, "WARN: Unexpected msg received"
                        "(first 10B: %10s), ignoring.\n", buf);
        continue;
      }

      /* reply to the original IP and port */
      if (sendto(global_vars.sock, MSG_RESPONSE, SLEN(MSG_RESPONSE), 0,
            (struct sockaddr *)&sa, sizeof(sa)) != SLEN(MSG_RESPONSE)) {
        perror("ERR [ignoring] sendto()");
        continue;
      }
    }
  }

  /* will be never reached */
  return EXIT_SUCCESS;
}
