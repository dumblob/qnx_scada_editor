#!/bin/sh

# TODO
#   rozbehnout sit (touch ... a restart)
#   nastavit fonty ???

# FIXME create README.new with new stuff (how the behavior changed)
#   new users and groups get GID and UID automatically chosen
#   ~dir and non-tilda dirs divided into two separate subfolders (common & specific)
#   common/etc/qnx/ (containing license stuff) moved to specific/etc/
#   no need to replace symlinks, because they are (have to be!) relative in SRCPATH
#   assume no special files like *.RLnk are present in SRCPATH
#   not needed, because the permissions are already in SRCPATH
#     chown -R root:root "$NODE/usr/disam/"
#     chown -R root:root "$NODE/usr/include/drt"
#     chmod -R ugo+r "$NODE/usr/include/drt"

# FIXME
# formerly
#   home/~ms_admin/.ph/
#   home/~ps_admin/.ph/
# but in the isis.gtar is only
#   home/ms/.ph/
#   home/ps/.ph/
# FIXME
#   preset passwords for newly created accounts

# /etc/group
# mcs-03:x:101:
# /etc/passwd
# vilem:x:100:0:Vilem Srovnal:/home/vilem:/bin/sh
# ms::200:101:MS-SCADA-QNX:/home/ms:/bin/sh
# ps::201:101:PS-SCADA-QNX:/home/ps:/bin/sh
# rps:x:202:101:Remote PS:/home/rps:/bin/sh

# /etc/group
# mcs-03:x:101:
# /etc/passwd
# ms::200:101:MS-SCADA-QNX:/home/ms:/bin/sh
# ps::201:101:PS-SCADA-QNX:/home/ps:/bin/sh
# rps:x:202:101:Remote PS:/home/rps:/bin/sh

print_help() {
  echo "SYNOPSIS"
  echo "  Installs SCADA system-wide on measure- or work- station node."
  echo "  Owerwritten files are backed-up."
  echo "USAGE"
  echo "  install.sh help"
  echo "  install.sh install [-m] [-s <src_path>] [-n <node_path>] [-l <license_path>]"
  echo "                     -p <project_name> -i <node_id> -t <tech_user>"
  echo "  install.sh uninstall [-n <node_path>] <dir_with_backup>"
  #FIXME
  echo "  install.sh useradd"
  #FIXME
  echo "  install.sh userdel"
  #FIXME
  echo "  install.sh groupadd name pass GID user_list"
  #FIXME
  echo "  install.sh groupdel {-n <name>|-gid <GID>}"
  echo "OPTIONS"
  echo "  -m"
  echo "    install measurement station (aka \`Merici stanice')"
  echo "    default: install workstation (aka \`Pracovni stanice')"
  echo "  -s <src_path>"
  echo "    path to dir with all SCADA files (those, which will be installed)"
  echo "    default: ./"
  echo "  -n <node_path>"
  echo "    path to node we will operate on (e.g. /net/node_name)"
  echo "    default: /"
  echo "  -l <license_path>"
  echo "    (re)write SCADA license"
  echo "  -p <project_name>"
  echo "    name of the project"
  echo "  -i <node_id>"
  echo "    node number (ID)"
  echo "  -t <tech_user>"
  echo "    technological user name"
  exit $1
}

exit_msg() {
  echo "$2" >&2
  exit $1
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
LICENSE=
PROJECT=
NODE_ID=
TECH_USER=

# inst uninst uadd udel gadd gdel
STATE=

while [ $# -gt 0 ]; do
  case "$1" in
    'help')
      print_help 0 ;;
    'install')
      STATE='inst' ;;
    'uninstall')
      echo 'ERR Not implemented yet!' >&2; exit 1
      STATE='uninst' ;;
    'useradd')
      echo 'ERR Not implemented yet!' >&2; exit 1
      STATE='uadd' ;;
    'userdel')
      echo 'ERR Not implemented yet!' >&2; exit 1
      STATE='udel' ;;
    'groupadd')
      echo 'ERR Not implemented yet!' >&2; exit 1
      STATE='gadd' ;;
    'groupdel')
      echo 'ERR Not implemented yet!' >&2; exit 1
      STATE='gdel' ;;
    '-m')
      [ "$STATE" = 'inst' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      MEA_ST='yes' ;;
    '-s')
      [ "$STATE" = 'inst' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -s demands <src_path>."
      SRCPATH="$(full_path "$1")" ;;
    '-n')
      [ "$STATE" = 'inst' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -n demands <node_path>."
      NODE="$(full_path "$1")" ;;
    '-l')
      [ "$STATE" = 'inst' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -l demands <license_path>."
      LICENSE="$1" ;;
    '-p')
      [ "$STATE" = 'inst' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -p demands <project_name>."
      PROJECT="$1" ;;
    '-i')
      [ "$STATE" = 'inst' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -i demands <node_num>."
      NODE_ID="$1"
      # be aware of newline characters!
      echo "$NODE_ID" | awk 'BEGIN { RS="" }
        { if ($0 !~ /^[0-9]+$/) { exit 1 } }' ||
          exit_msg 1 'ERR <node_num> has to match [0-9]+' ;;
    '-t')
      [ "$STATE" = 'inst' ] || exit_msg 1 "ERR Unknown argument \`$1'."
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -t demands <tech_user>."
      TECH_USER="$1"
      # be aware of newline characters!
      echo "$TECH_USER" | awk 'BEGIN { RS="" }
        { if ($0 !~ /^[a-z_][a-z0-9_-]*[$]?$/) { exit 1 } }' ||
          exit_msg 1 'ERR <tech_user> has to match [a-z_][a-z0-9_-]*[$]' ;;
    *)
      exit_msg 1 "ERR Unknown argument \`$1'." ;;
  esac
  shift
done

[ -z "$STATE" ]      && exit_msg 1 "ERR Mode (help|install|...) required."
[ -z "$PROJECT" ]    && exit_msg 1 "ERR Non-empty <project_name> required."
[ -z "$NODE_ID" ]    && exit_msg 1 "ERR Non-empty <node_id> required."
[ -z "$TECH_USER" ]  && exit_msg 1 "ERR Non-empty <tech_user> required."
[ "$(id -u)" -eq 0 ] || exit_msg 1 "ERR Root privileges needed."
[ -d "$NODE" ]       || exit_msg 1 "ERR Directory $NODE doesn't exist."

ask() {
  while printf '%s' "$1"; do
    read x
    case "$x" in
      y|Y) return 0;;
      n|N) return 1;;
      *) echo "Please choose either \`y' as \`yes' or \`n' as \`no'";;
    esac
  done
}

ask_ignore() {
  [ -n "$1" ] && echo "$1" >&2
  ask "Would you like to ignore it? [y/n] "
}

# <full_path_of_file_or_dir_to_copy> <dst_path_relative_to_/_of_the_target_system>
# NOTE
#   directories are not copied recursively (thus they will be empty)
#   no checks for path existence are done (path should already exist -
#     ensure it using e.g. `find . | sort')
cp_n_backup() {
  [ -z "$_BACKUP_DIR" ] && {
    _BACKUP_DIR="$NODE/backup_$TSTAMP"
    # in case we are in subshell => this is to prevent >1 outputs
    [ -e "$_BACKUP_DIR" ] || {
      mkdir -p "$_BACKUP_DIR"
      echo "INFO Backuped files are stored in $_BACKUP_DIR"
    }
  }

  if [ -d "$1" ]; then
    [ -e "$NODE/$2" ] && {
      # `copy' while preserving permissions, ownership, mtime and ctime
      (cd "$(dirname "$NODE/$2")" && tar --no-recursion -c "$(basename "$2")") |
      # overwrite metadata if file|dir exists
      (cd "$(dirname "$_BACKUP_DIR/$2")" && tar --same-owner -xp) || {
        echo "ERR Backup failed, original file \`$NODE/$2' preserved."
        return 1
      }
    }
    (cd "$(dirname "$1")" && tar --no-recursion -c "$(basename "$1")") |
    (cd "$(dirname "$NODE/$2")" && tar --same-owner -xp)
  else
    [ -e "$NODE/$2" ] && {
      mv "$NODE/$2" "$_BACKUP_DIR/$2" 2> /dev/null || {
        echo "ERR Backup failed, original file \`$NODE/$2' preserved."
        return 1
      }
    }
    cp -p "$1" "$NODE/$2"
  fi
}

install_tree() {
  _len="$(echo "$1" | wc -c)"
  find "$1" | sort | while read f; do
    cp_n_backup "$f" "$(echo "$f" | cut -b $_len-)"
  done
}

# name pass GID user_list
group_add() {
  (
  [ $# -eq 4 ] || return 1
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
  echo "$1:$2:$_uid:$4:$5:$6:$7" >> "$NODE/etc/passwd"
  [ -d "$6" ] || mkdir -p "$6"
  chown "$_uid":"$4" "$6"
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
  awk -F: "{ if (\$1 == \"$2\") { print \$3; x = 1; exit 0 } }
    END { if (! x) { print \"$_m\"; exit 1 } }" < "$_f"
  )
}


echo 'INFO Installing system files...'


install_tree "$SRCPATH/common"
ls -1 -d "$NODE"/usr/qnx6*/target/qnx6/usr/include/ 2>/dev/null |
  while read d; do
    ln -s '/usr/include/drt' "$d"
  done
ls -1 -d "$NODE"/usr/qnx6*/target/qnx6/usr/help/product 2>/dev/null |
  while read d; do
    ln -s '/usr/help/product/Disam' "$d"
    ln -s '/usr/help/product/Disam.toc' "$d"
  done


echo 'INFO Installing specific/optional system files...'


install_tree "$SRCPATH/specific_dll"
install_tree "$SRCPATH/specific_etc"
install_tree "$SRCPATH/specific_io-audio"
install_tree "$SRCPATH/specific_launchmenu"
# just for information - we won't install any QNX license automatically!
if [ -e "$NODE/etc/qnx/license/licenses" ]; then
  echo "INFO QNX license found."
else
  echo "WARN No QNX license found."
fi
echo "INFO Version of the currently running QNX microkernel is \`$(uname -r)'."
echo "INFO Available licenses for manual installation:"
found=
ls -1 "$SRCPATH"/specific_license/etc/qnx/license/licenses* 2>/dev/null |
  while read l; do
    found='yes'
    printf "  %s\n" "$l"
  done
[ -z "$found" ] && echo "  <none>"


echo 'INFO Setting hostname to ?????'


#FIXME (ps|ms)[0-9]+ WS|ES|MS
#printf '%s\n%s\n%s\n' \
#  "ORIG_HOSTNAME=ps1" \
#  "ALIAS_HOSTNAME=ps1" \
#  "HOST_FCE=WS" > "$NODE/etc/hostnames.run"


echo "INFO Adding groups (if needed)."


# formerly GID=100
echo "INFO   groupadd \`drt'"
gid_drt="$(group_add 'drt' '' '' '')" || {
  # ignore "Group already exists" error
  [ $? -eq 2 ] || ask_ignore "$gid_drt" || exit 1
  # ensure gid_drt is set (in case of $?==2 or if user ignored some ERR)
  gid_drt="$(get_id gid 'drt')" || exit_msg 1 "$gid_drt"
}

# formerly GID=101
echo "INFO   groupadd \`mcs03'"
gid_mcs03="$(group_add 'mcs03' '' '' '')" || {
  [ $? -eq 2 ] || ask_ignore "$gid_mcs03" || exit 1
  gid_mcs03="$(get_id gid 'mcs03')" || exit_msg 1 "$gid_mcs03"
}


echo "INFO Adding users (if needed)."


# formerly UID=100 GID=100
echo "INFO   useradd \`kost'"
ret="$(user_add 'kost' '' "$gid_drt" "$gid_drt" \
  'V. Košťál' '/home/kost' '/bin/sh')" ||
# ignore "User already exists" error
[ $? -eq 2 ] || ask_ignore "$ret" || exit 1
if [ -n "$MEA_ST" ]; then
  cp -rp "$SRCPATH/common/home/ms/.ph/" "$NODE/home/kost/"
else
  cp -rp "$SRCPATH/common/home/ps/.ph/" "$NODE/home/kost/"
fi
chown -R "$gid_drt:$gid_drt" "$NODE/home/kost/.ph/"


# formerly UID=101
echo "INFO   useradd \`$PROJECT'"
uid_pm="$(user_add "$PROJECT" '' '' "$gid_drt" \
  'Správce projektu' "/libr/$PROJECT" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_pm" || exit 1
  uid_pm="$(get_id uid "$PROJECT")" || exit_msg 1 "$uid_pm"
}
for d in bin gbin source include ph_app; do
  mkdir -p "$NODE/libr/$PROJECT/$d"
done
if [ -n "$MEA_ST" ]; then
  cp -rp "$SRCPATH/common/home/ms/.ph/" "$NODE/libr/$PROJECT"
else
  cp -rp "$SRCPATH/common/home/ps/.ph/" "$NODE/libr/$PROJECT"
fi
chown -R "$uid_pm:$gid_drt" "$NODE/libr/$PROJECT/"


# formerly UID=(200 + $NODE_ID)
echo "INFO   useradd \`$TECH_USER'"
uid_tu="$(user_add "$TECH_USER" '' '' "$gid_mcs03" \
  'Stand. uživatel' "/home/$TECH_USER" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_tu" || exit 1
  uid_tu="$(get_id uid "$TECH_USER")" || exit_msg 1 "$uid_tu"
}
for d in exe err litters; do
  mkdir -p "$NODE/home/$TECH_USER/$d"
done
if [ "$NODE_ID" -eq 1 ]; then
  for d in alr arc kfg server graf eng monitor; do
    mkdir -p "$NODE/home/$TECH_USER/$d"
  done
  #FIXME cp -r "$SRCPATH/common/home/~ms/.ph/ ...
  cp -rp "$SRCPATH/common/home/ms/.ph/" "$NODE/home/$TECH_USER"
else
  #FIXME cp -r "$SRCPATH/common/home/~ps/.ph/ ...
  cp -rp "$SRCPATH/common/home/ps/.ph/" "$NODE/home/$TECH_USER"
fi
chown -R "$uid_tu:$gid_mcs03" "$NODE/home/$TECH_USER"


if [ "$NODE_ID" -eq 1 ]; then
  user_drt="ms_drt"
else
  user_drt="ps_drt"
fi
# formerly UID=199
echo "INFO   useradd \`$user_drt'"
uid_drt="$(user_add "$user_drt" '' '' "$gid_mcs03" \
  'Drt-service' "/home/$user_drt" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_drt" || exit 1
  uid_drt="$(get_id uid "$user_drt")" || exit_msg 1 "$uid_drt"
}
for d in exe err litters; do
  ln -s "/home/$TECH_USER/$d" "$NODE/home/$user_drt"
done
[ "$NODE_ID" -eq 1 ] && {
  for d in alr arc kfg server graf eng monitor; do
    ln -s "/home/$TECH_USER/$d" "$NODE/home/$user_drt"
  done
}
#FIXME this recursively overwrites owner from "$uid_tu:$gid_mcs03" of
#  "$NODE/home/$TECH_USER"
#chown -R "$uid_drt:$gid_mcs03" "$NODE/home/$user_drt"


# formerly UID=200
echo "INFO   useradd \`admin'"
uid_admin="$(user_add 'admin' '' '' "$gid_mcs03" \
  '' "/home/admin" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_admin" || exit 1
  uid_admin="$(get_id uid 'admin')" || exit_msg 1 "$uid_admin"
}
if [ -n "$MEA_ST" ]; then
  cp -rp "$SRCPATH/common/home/ms/.ph/" "$NODE/home/admin/"
else
  cp -rp "$SRCPATH/common/home/ps/.ph/" "$NODE/home/admin/"
fi
chown -R "$uid_admin:$gid_mcs03" "$NODE/home/admin/"
