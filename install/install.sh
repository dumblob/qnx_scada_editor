#!/bin/sh

# TODO
#   set up network (touch ... and restart?)
#   set fonts
#   add support for >1 group into the useradd interface
#   replace non-POSIX constructs
#     sed -i
#     tar --no-recursion

print_help() {
  echo "SYNOPSIS"
  echo "  Installs Disam RT SCADA system-wide on measure- or work- station node."
  echo "  All owerwritten files are backed-up."
  echo "USAGE"
  echo "  install.sh <mode>"
  echo "MODES"
  echo "  help"
  echo "  install [-m <ms_hostname>] [-s <src_path>] [-n <node_path>]"
  echo "          -p <project_name> -i <node_id> -t <tech_user>"
  echo "  uninstall [-n <node_path>] -b <backup_dir>"
  echo "OPTIONS"
  echo "  -m <ms_hostname>"
  echo "    install as workstation (aka \`Pracovni stanice') connected to"
  echo "      the given measurement station hostname."
  echo "    default: install as measurement station (aka \`Merici stanice')"
  echo "  -s <src_path>"
  echo "    path to dir with all SCADA files (those, which will be installed)"
  echo "    default: ./"
  echo "  -n <node_path>"
  echo "    path to node we will operate on (e.g. /net/node_name)"
  echo "    default: /"
  echo "  -p <project_name>"
  echo "    name of the project"
  echo "  -i <node_id>"
  echo "    node number (ID); ID=1 has a special meaning"
  echo "  -t <tech_user>"
  echo "    technological user name"
  echo "  -b <backup_dir>"
  echo "    path to dir with backup"
}

# call in subshell to not pollute/change environment
full_path() {
  # basename always returns a valid path (thus / becomes / unexpectedly)
  #   => check if it's a directory first
  if [ -d "$1" ]; then
    cd "$1" || return $?
    echo "$PWD"
  else
    cd "$(dirname "$1")" || return $?
    echo "$PWD/$(basename "$1")"
  fi
  cd "$OLDPWD"
}

TSTAMP="$(date '+%Y%m%d-%H%M%S')"
MEA_ST=
SRCPATH="$(full_path '.')"
NODE='/'
PROJECT=
NODE_ID=
TECH_USER=
BACKUP_DIR=
# help inst uninst
STATE=

msg() {
  echo "$@" | tee -a "$BACKUP_DIR/install.log"
}

emsg() {
  echo "$@" | tee -a "$BACKUP_DIR/install.log" >&2
}

exit_msg() {
  echo "$2" >&2
  exit $1
}

while [ $# -gt 0 ]; do
  case "$1" in
    'help')
      [ -n "$STATE" ] && exit_msg 1 "ERR Mode could be specified only once."
      STATE='help' ;;
    'install')
      [ -n "$STATE" ] && exit_msg 1 "ERR Mode could be specified only once."
      STATE='install' ;;
    'uninstall')
      [ -n "$STATE" ] && exit_msg 1 "ERR Mode could be specified only once."
      STATE='uninstall' ;;
    '-m')
      [ "$STATE" = 'install' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -m demands <ms_hostname>."
      MEA_ST="$1" ;;
    '-s')
      [ "$STATE" = 'install' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -s demands <src_path>."
      SRCPATH="$(full_path "$1")" ;;
    '-n')
      [ "$STATE" = 'install' -o "$STATE" = 'uninstall' ] ||
        exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -n demands <node_path>."
      NODE="$(full_path "$1")" ;;
    '-p')
      [ "$STATE" = 'install' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -p demands <project_name>."
      PROJECT="$1" ;;
    '-i')
      [ "$STATE" = 'install' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -i demands <node_num>."
      NODE_ID="$1"
      # be aware of newline characters!
      echo "$NODE_ID" | awk 'BEGIN { RS="" }
      { if ($0 !~ /^[0-9]+$/) { exit 1 } }' ||
        exit_msg 1 'ERR <node_num> has to match [0-9]+' ;;
    '-t')
      [ "$STATE" = 'install' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -t demands <tech_user>."
      TECH_USER="$1"
      # be aware of newline characters!
      echo "$TECH_USER" | awk 'BEGIN { RS="" }
      { if ($0 !~ /^[a-z_][a-z0-9_-]*[$]?$/) { exit 1 } }' ||
        exit_msg 1 'ERR <tech_user> has to match [a-z_][a-z0-9_-]*[$]?' ;;
    '-b')
      [ "$STATE" = 'uninstall' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -b demands <backup_dir>."
      BACKUP_DIR="$(full_path "$1")" ;;
    *)
      exit_msg 1 "ERR Unknown argument \`$1'." ;;
  esac
  shift
done

case "$STATE" in
  'help')
    print_help
    exit 0 ;;
  'install')
    [ -z "$PROJECT" ]   && exit_msg 1 "ERR Non-empty <project_name> required."
    [ -z "$NODE_ID" ]   && exit_msg 1 "ERR Non-empty <node_id> required."
    [ -z "$TECH_USER" ] && exit_msg 1 "ERR Non-empty <tech_user> required."
    [ -d "$NODE" ]      || exit_msg 1 "ERR Directory $NODE doesn't exist."
    ;;
  'uninstall')
    [ -z "$BACKUP_DIR" ] && exit_msg 1 "ERR Non-empty <backup_dir> required."
    ;;
  *) exit_msg 1 "ERR Mode (help|...) required." ;;
esac

[ "$(id -u)" -eq 0 ] || exit_msg 1 "ERR Root privileges needed."

ask() {
  while printf '%s' "$1"; do
    read x
    case "$x" in
      y|Y) return 0;;
      n|N) return 1;;
      *) msg "Please choose either \`y' as \`yes' or \`n' as \`no'";;
    esac
  done
}

ask_ignore() {
  [ -n "$1" ] && emsg "$1"
  ask "Would you like to ignore it? [y/n] "
}

# non-recursive `cp' with permissions, ownership, mtime and ctime preservation
cp_tar() {
  (cd "$(dirname "$1")" && tar --no-recursion -c "$(basename "$1")") |
  # overwrite metadata if file|dir exists
  { cd "$(dirname "$2")" && tar --same-owner -xp; }
}

# <full_path_of_file_or_dir_to_copy> <dst_path_relative_to_/_of_the_target_system> [<uid:gid>]
# NOTE
#   directories are not copied recursively (thus they will be empty)
#   no checks for path existence are done (path should already exist -
#     ensure it using e.g. `find . | sort')
cp_n_backup() {
  [ -z "$_BACKUP_TREE" ] && {
    _BACKUP_TREE="$BACKUP_DIR/tree"
    # this check is to prevent >1 outputs in case we are in subshell
    [ -e "$_BACKUP_TREE" ] || mkdir -p "$_BACKUP_TREE"
  }

  # use cp -p where possible because cp_tar is slow
  if [ -d "$1" ]; then
    # preserve the first one backuped file/dir (i.e. the original one)
    [ -e "$NODE/$2" -a ! -e "$_BACKUP_DIR/$2" ] && {
      cp_tar "$NODE/$2" "$_BACKUP_DIR/$2" || {
        emsg "ERR Backup failed, original file \`$NODE$2' preserved."
        return 1
      }
    }
    cp_tar "$1" "$NODE/$2"
  else
    [ -e "$NODE/$2" -a ! -e "$_BACKUP_DIR/$2" ] && {
      mv "$NODE/$2" "$_BACKUP_DIR/$2" 2> /dev/null || {
        emsg "ERR Backup failed, original file \`$NODE$2' preserved."
        return 1
      }
    }
    cp -p "$1" "$NODE/$2"
  fi
  # it's not possible to change owner of symlinks
  [ $# -gt 2 -a -n "$3" ] && [ ! -L "$NODE/$2" ] && chown "$3" "$NODE/$2"
  echo "$2" >> "$BACKUP_DIR/installed_files"
}

# <tree_path> [<dst_tree_prefix_relative_to_/_of_the_target_system> [<uid:gid>]]
install_tree() {
  _len="$(echo "$1" | wc -c)"
  # skip first line because it is the $1 itself
  _offset=2
  # create dir first
  [ $# -gt 1 ] && _offset=1
  find "$1" | sort | tail -n +$((_offset)) | while read f; do
    cp_n_backup "$f" "$2$(echo "$f" | cut -b $_len-)" "$3"
  done
}

# name pass GID user_list
group_add() {
  (
  [ $# -eq 4 ] || return 1
  [ -e "$BACKUP_DIR/tree/etc/group" ] || {
    mkdir -p "$BACKUP_DIR/tree"
    cp_tar "$NODE/etc" "$BACKUP_DIR/tree/etc"
    cp -p "$NODE/etc/group" "$BACKUP_DIR/tree/etc/group"
    echo '/etc/group' >> "$BACKUP_DIR/installed_files"
  }
  grep -E "^$1:" < "$NODE/etc/group" > /dev/null 2>&1 && {
    echo "ERR Group \`$1' already exists."
    return 2
  }
  _gid="$3"
  if [ -z "$_gid" ]; then
    _gid=100
    while grep -E "^[^:]+:[^:]*:$_gid:" < "$NODE/etc/group" > /dev/null 2>&1; do
      _gid=$((_gid +1))
    done
  elif grep -E "^[^:]+:[^:]*:$_gid:" < "$NODE/etc/group" > /dev/null 2>&1; then
    echo "ERR GID \`$_gid' already exists."
    return 3
  fi
  echo "$1:$2:$_gid:$4" >> "$NODE/etc/group"
  echo "$_gid"
  )
}

# login pass UID GID comment home_dir cmd_interpret
user_add() {
  (
  [ $# -eq 7 ] || return 1
  [ -e "$BACKUP_DIR/tree/etc/passwd" ] || {
    mkdir -p "$BACKUP_DIR/tree"
    cp_tar "$NODE/etc" "$BACKUP_DIR/tree/etc"
    cp -p "$NODE/etc/passwd" "$BACKUP_DIR/tree/etc/passwd"
    echo "$NODE/etc/passwd" "$BACKUP_DIR/tree/etc/passwd"
    echo '/etc/passwd' >> "$BACKUP_DIR/installed_files"
  }
  grep -E "^$1:" < "$NODE/etc/passwd" > /dev/null 2>&1 && {
    echo "ERR User \`$1' already exists."
    return 2
  }
  _uid="$3"
  if [ -z "$_uid" ]; then
    _uid=100
    while grep -E "^[^:]+:[^:]*:$_uid:" < "$NODE/etc/passwd" > /dev/null 2>&1; do
      _uid=$((_uid +1))
    done
  elif grep -E "^[^:]+:[^:]*:$_uid:" < "$NODE/etc/passwd" > /dev/null 2>&1; then
    echo "ERR UID \`$_uid' already exists."
    return 3
  fi
  grep -E "^[^:]+:[^:]*:$4:" < "$NODE/etc/group" > /dev/null 2>&1 || {
    echo "ERR GID \`$4' doesn't exists."
    return 4
  }
  sed -i -r -e "s|^([^:]+:[^:]*:$4:.*)$|\\1,$1|" -e "s|:,|:|g" \
    "$NODE/etc/passwd"
  echo "$1:$2:$_uid:$4:$5:$6:$7" >> "$NODE/etc/passwd"
  if [ -d "$NODE$6" ]; then
    cp_tar "$NODE$6" "$BACKUP_DIR/tree$6"
    chown "$_uid":"$4" "$NODE$6"
    echo "$6" >> "$BACKUP_DIR/installed_files"
  else
    mkdir -p "$NODE$(dirname "$6")"
    cp -rp /etc/skel "$NODE$6"
    chown -R "$_uid:$4" "$NODE$6"
    cd "$NODE"
    find "$6" >> "$BACKUP_DIR/installed_files"
  fi
  chmod 750 "$NODE$6"
  echo "$_uid"
  )
}

# uid|gid <name>
get_id() {
  (
  if [ "$1" = 'gid' ]; then
    _f="$NODE/etc/group"
    _m="ERR Group $2 not found."
  else
    _f="$NODE/etc/passwd"
    _m="ERR User $2 not found."
  fi
  awk -F: "{ if (\$1 == \"$2\") { print \$3; found = 1; exit 0 } }
    END { if (! found) { exit 1 } }" < "$_f" || {
      emsg "$_m"
      exit 1
    }
  )
}


[ "$STATE" = 'uninstall' ] && {
  msg 'INFO Restoring all files from backup and removing files/dirs'
  msg '     introduced in that particular Disam RT SCADA installation.'
  sed 's|/+|/|g' "$BACKUP_DIR/installed_files" | sort -r | uniq |
  while read f; do
    if [ -e "$BACKUP_DIR/tree/$f" ]; then
      # use cp -p where possible because cp_tar is slow
      if [ -d "$BACKUP_DIR/tree/$f" ]; then
        cp_tar "$BACKUP_DIR/tree/$f" "$NODE/$f"
      else
        mv "$BACKUP_DIR/tree/$f" "$NODE/$f"
      fi
    else
      if [ -d "$NODE/$f" ]; then
        rmdir "$NODE/$f" 2> /dev/null
      elif [ -e "$NODE/$f" ]; then
        rm -f "$NODE/$f"
      fi
    fi
  done
  msg 'INFO Done uninstalling Disam RT SCADA.'
  msg "INFO You can safely remove \`$BACKUP_DIR' if you don't"
  msg '     need logs any more).'
  exit
}


BACKUP_DIR="$NODE/backup_$TSTAMP"
mkdir -p "$BACKUP_DIR" || exit $?
msg "INFO Backup directory is \`$BACKUP_DIR'"


msg 'INFO Installing system files...'


install_tree "$SRCPATH/common"
msg "WARN Please check the following files for your specific settings,"
msg "     because they were overwritten."
msg "  $NODE/etc/rc.local"
msg "  $NODE/etc/ntp.conf"
msg "WARN Assuming hostname \`$(hostname)' of the target node \`$NODE'."
#FIXME which abbreviations are valid?
#  WS work-station aka `pracovni stanice'
#  ES <unknown>
#  MS measurement-station aka `merici stanice'
#  ZMS backup measurement-station aka `zalozni merici stanice'
#  PS <unknown>
echo \
  "ORIG_HOSTNAME=$(hostname)" \
  "ALIAS_HOSTNAME=$(hostname)" \
  "HOST_FCE=$(if [ -z "$MEA_ST" ]; then echo MS; else echo WS; fi)  # WS|ES|MS MS/ZMS/PS" \
  > "$NODE/etc/hostnames.run"


msg 'INFO Installing specific/optional system files...'


#FIXME detect did: 293e,27de,7012
install_tree "$SRCPATH/specific_dll"
install_tree "$SRCPATH/specific_editor"
install_tree "$SRCPATH/specific_etc"
msg "WARN Please check if \`$NODE/etc/ham-ph.cfg contains the right values."
# permissions are preserved when using sh > redirection
f='/etc/profile.d/scada.sh'
sed -r \
  -e 's|(SCADA_MS_NODE=)MS_dummy_name|\1'"$(
    if [ -z "$MEA_ST" ]; then hostname; else echo "$MEA_ST"; fi)"'|' \
  -e 's|(PROJEKT=)MCS_dummy|\1'"$PROJECT"'|' \
  "$SRCPATH/specific_etc$f" > "$NODE$f"
msg "WARN Please check \`$NODE$f' for sockets and ftp-server settings."
#FIXME detect did: 293e,27de,7012
install_tree "$SRCPATH/specific_io-audio"
#FIXME auto-detect Qnet over IP?
install_tree "$SRCPATH/specific_ip-qnet"
install_tree "$SRCPATH/specific_keepalive"
install_tree "$SRCPATH/specific_launchmenu"
# just for information - we won't install any QNX license automatically!
if [ -e "$NODE/etc/qnx/license/licenses" ]; then
  msg "INFO Installed QNX license found."
else
  msg "WARN No installed QNX license found, some SCADA functionality mustn't work."
fi
msg "INFO Currently running QNX microkernel: $(uname -r)"
msg "INFO Available licenses for manual installation:"
ls -1 "$SRCPATH"/specific_license/etc/qnx/license/licenses* 2>/dev/null | {
  found=
  while read l; do
    found='yes'
    msg "  $l"
  done
  [ -z "$found" ] && msg "  <none>"
}
#FIXME detect (otherwise it could break display.conf)
msg "WARN Not installing $SRCPATH/specific_matrox because of possible display.conf issues."
#install_tree "$SRCPATH/specific_matrox"
ls -1 -d "$NODE"/usr/qnx6*/target/qnx6/usr/include 2>/dev/null |
  while read d; do
    cp_n_backup "$SRCPATH/specific_symlinks/usr/include/drt" "$d/drt"
  done
ls -1 -d "$NODE"/usr/qnx6*/target/qnx6/usr/help/product 2>/dev/null |
  while read d; do
    cp_n_backup \
      "$SRCPATH/specific_symlinks/usr/help/product/Disam" "$d/Disam"
    cp_n_backup \
      "$SRCPATH/specific_symlinks/usr/help/product/Disam.toc" "$d/Disam.toc"
  done


msg "INFO Adding groups (if needed)."


# formerly GID=100
msg "INFO   groupadd \`drt'"
gid_drt="$(group_add 'drt' '' '' '')" || {
  # ignore "Group already exists" error
  [ $? -eq 2 ] || ask_ignore "$gid_drt" || exit 1
  # ensure gid_drt is set (in case of $?==2 or if user ignored some ERR)
  gid_drt="$(get_id gid 'drt')" || exit_msg 1 "$gid_drt"
}


# formerly GID=101
msg "INFO   groupadd \`mcs03'"
gid_mcs03="$(group_add 'mcs03' '' '' '')" || {
  [ $? -eq 2 ] || ask_ignore "$gid_mcs03" || exit 1
  gid_mcs03="$(get_id gid 'mcs03')" || exit_msg 1 "$gid_mcs03"
}


msg "INFO Adding users (if needed)."


# formerly UID=100 GID=100
msg "INFO   useradd \`kost'"
ret="$(user_add 'kost' '' "$gid_drt" "$gid_drt" \
  'V. Košťál' '/home/kost' '/bin/sh')" ||
# ignore "User already exists" error
[ $? -eq 2 ] || ask_ignore "$ret" || exit 1
if [ -z "$MEA_ST" ]; then
  install_tree "$SRCPATH/specific_home_ms/home/ms/.ph" \
    "/home/kost" "$gid_drt:$gid_drt"
else
  install_tree "$SRCPATH/specific_home_ps/home/ps/.ph" \
    "/home/kost" "$gid_drt:$gid_drt"
fi


# formerly UID=101
msg "INFO   useradd \`$PROJECT'"
uid_pm="$(user_add "$PROJECT" '' '' "$gid_drt" \
  'Správce projektu' "/libr/$PROJECT" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_pm" || exit 1
  uid_pm="$(get_id uid "$PROJECT")" || exit_msg 1 "$uid_pm"
}
for d in bin gbin source include ph_app; do
  mkdir "$NODE/libr/$PROJECT/$d"
  chown "$uid_pm:$gid_drt" "$NODE/libr/$PROJECT/$d"
  echo "/libr/$PROJECT/$d" >> "$BACKUP_DIR/installed_files"
done
if [ -z "$MEA_ST" ]; then
  install_tree "$SRCPATH/specific_home_ms/home/ms/.ph/" \
    "/libr/$PROJECT" "$uid_pm:$gid_drt"
else
  install_tree "$SRCPATH/specific_home_ps/home/ps/.ph/" \
    "/libr/$PROJECT" "$uid_pm:$gid_drt"
fi


# formerly UID=200
msg "INFO   useradd \`ms'"
# SCADA software runs from here
uid_ms="$(user_add 'ms' '' '' "$gid_mcs03" \
  'MS-SCADA-QNX' '/home/ms' '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_ms" || exit 1
  uid_ms="$(get_id uid 'ms')" || exit_msg 1 "$uid_ms"
}
# FIXME adjust according to /home/ms/.ph/README.txt
install_tree "$SRCPATH/specific_home_ms/home/ms/" "/home" "$uid_ms:$gid_mcs03"


# formerly UID=201
msg "INFO   useradd \`ps'"
# SCADA HMI software runs from here
uid_ps="$(user_add 'ps' '' '' "$gid_mcs03" \
  'PS-SCADA-QNX' '/home/ps' '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_ps" || exit 1
  uid_ps="$(get_id uid 'ps')" || exit_msg 1 "$uid_ps"
}
# FIXME adjust according to /home/ps/.ph/README.txt
install_tree "$SRCPATH/specific_home_ps/home/ps/" "/home" "$uid_ms:$gid_mcs03"


# formerly UID=202
msg "INFO   useradd \`rps'"
uid_rps="$(user_add 'rps' '' '' "$gid_mcs03" \
  'Remote PS' '/home/rps' '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_rps" || exit 1
  uid_rps="$(get_id uid 'rps')" || exit_msg 1 "$uid_rps"
}


# formerly UID=(200 + $NODE_ID)
msg "INFO   useradd \`$TECH_USER'"
uid_tu="$(user_add "$TECH_USER" '' '' "$gid_mcs03" \
  'Stand. uživatel' "/home/$TECH_USER" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_tu" || exit 1
  uid_tu="$(get_id uid "$TECH_USER")" || exit_msg 1 "$uid_tu"
}
for d in exe err litters; do
  mkdir -p "$NODE/home/$TECH_USER/$d"
  chown "$uid_tu:$gid_mcs03" "$NODE/home/$TECH_USER/$d"
  echo "/home/$TECH_USER/$d" >> "$BACKUP_DIR/installed_files"
done
if [ "$NODE_ID" -eq 1 ]; then
  for d in alr arc kfg server graf eng monitor; do
    mkdir "$NODE/home/$TECH_USER/$d"
    chown "$uid_tu:$gid_mcs03" "$NODE/home/$TECH_USER/$d"
    echo "/home/$TECH_USER/$d" >> "$BACKUP_DIR/installed_files"
  done
  #FIXME cp -rp "$SRCPATH/common/home/~ms/.ph/ ...
  install_tree "$SRCPATH/specific_home_ms/home/ms/.ph/" \
    "/home/$TECH_USER" "$uid_tu:$gid_mcs03"
else
  #FIXME cp -rp "$SRCPATH/common/home/~ps/.ph/ ...
  install_tree "$SRCPATH/specific_home_ps/home/ps/.ph/" \
    "/home/$TECH_USER" "$uid_tu:$gid_mcs03"
fi


if [ "$NODE_ID" -eq 1 ]; then
  user_drt="ms_drt"
else
  user_drt="ps_drt"
fi
# formerly UID=199
msg "INFO   useradd \`$user_drt'"
uid_drt="$(user_add "$user_drt" '' '' "$gid_mcs03" \
  'Drt-service' "/home/$user_drt" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_drt" || exit 1
  uid_drt="$(get_id uid "$user_drt")" || exit_msg 1 "$uid_drt"
}
for d in exe err litters; do
  ln -s "/home/$TECH_USER/$d" "$NODE/home/$user_drt"
  echo "/home/$user_drt/$d" >> "$BACKUP_DIR/installed_files"
done
[ "$NODE_ID" -eq 1 ] && {
  for d in alr arc kfg server graf eng monitor; do
    ln -s "/home/$TECH_USER/$d" "$NODE/home/$user_drt"
    echo "/home/$user_drt/$d" >> "$BACKUP_DIR/installed_files"
  done
}
# do not chown -R "$uid_drt:$gid_mcs03" "$NODE/home/$user_drt"
#   because symlinks are always ugo+rwx and chown -R do work only
#   with the symlink destination (there is no way to change owner
#   of symlink in shell under QNX)


# formerly UID=200
msg "INFO   useradd \`admin'"
uid_admin="$(user_add 'admin' '' '' "$gid_mcs03" \
  '' "/home/admin" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_admin" || exit 1
  uid_admin="$(get_id uid 'admin')" || exit_msg 1 "$uid_admin"
}
if [ -z "$MEA_ST" ]; then
  install_tree "$SRCPATH/specific_home_ms/home/ms/.ph/" \
    "/home/admin" "$uid_admin:$gid_mcs03"
else
  install_tree "$SRCPATH/specific_home_ps/home/ps/.ph/" \
    "/home/admin" "$uid_admin:$gid_mcs03"
fi


msg 'WARN Please set passwords to the newly created accounts. Currently'
msg '     all passwords are empty and thus everybody can login.'
msg 'INFO Installation of Disam RT SCADA successful.'
