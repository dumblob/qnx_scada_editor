#!/bin/sh

#TODO
#  rozbehnout sit (touch ... a restart)
#  nastavit fonty ???

# FIXME create README.new with new stuff (how the behavior changed)
#   new users and groups get GID and UID automatically chosen

# FIXME
# formerly
#   home/~ms_admin/.ph/
#   home/~ps_admin/.ph/
# but in the isis.gtar is only
#   home/ms/.ph/
#   home/ps/.ph/

TSTAMP="$(date '+%Y%m%d-%H%M%S')"
SRCPATH='./'
#FIXME $project
PROJECT=
#FIXME target_node=/net/$tnode
NODE=
#FIXME tnode_id
NODE_ID=
MEA_ST=

TECH_USER=

print_help() {
  echo "SYNOPSIS"
  echo "  Installs SCADA system-wide on measure- or work- station node."
  echo "  Owerwritten files are backed-up."
  echo "USAGE"
  echo "  install.sh [-h]"
  echo "  install.sh [-m] [-s <src_path>] -p <project_name> -n <node> -i <node_id> -t <tech_user>"
  echo "OPTIONS"
  echo "  -m"
  echo "    install measurement station (aka \`Merici stanice')"
  echo "    default: install workstation (aka \`Pracovni stanice')"
  echo "  -s <src_path>"
  echo "    path to dir with all SCADA files (those, which will be installed)"
  echo "    default: ./"
  echo "  -p <project_name>"
  echo "    name of the project"
  echo "  -n <node>"
  echo "    path to current node (e.g. /net/local_node_name)"
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

while [ $# -gt 0 ]; do
  case "$1" in
    '-h')
      print_help 0 ;;
    '-m')
      MEA_ST='yes' ;;
    '-s')
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -s demands <src_path>."
      SRCPATH="$1" ;;
    '-p')
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -p demands <project_name>."
      PROJECT="$1" ;;
    '-n')
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -n demands <node>."
      NODE="$1" ;;
    '-i')
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -i demands <node_num>."
      NODE_ID="$1" ;;
      echo "$NODE_ID" | awk 'BEGIN { RS="" }
        { if ($0 !~ /^[0-9]+$/) { exit 1 } }' ||
          exit_msg 1 'ERR <node_num> has to match [0-9]+'
    '-t')
      shift
      [ $# -ge 1 ] || exit_msg 1 "ERR -t demands <tech_user>."
      TECH_USER="$1"
      echo "$TECH_USER" | awk 'BEGIN { RS="" }
        { if ($0 !~ /^[a-z_][a-z0-9_-]*[$]$/) { exit 1 } }' ||
          exit_msg 1 'ERR <tech_user> has to match [a-z_][a-z0-9_-]*[$]'
      ;;
    *)
      echo "ERR Unknown argument $1" >&2
      exit 1
  esac
  shift
done

shift
[ $# -gt 0 ]         && exit_msg 1 "ERR Unknown arguments given."
[ -z "$PROJECT" ]    && exit_msg 1 "ERR Non-empty <project_name> desired."
[ -z "$NODE" ]       && exit_msg 1 "ERR Non-empty <node> desired."
[ -z "$NODE_ID" ]     && exit_msg 1 "ERR Non-empty <node_id> desired."
[ -z "$TECH_USER" ]  && exit_msg 1 "ERR Non-empty <tech_user> desired."
[ "$(if -u)" -eq 0 ] && exit_msg 1 "ERR root privileges needed."

for d in "$NODE" "$NODE/usr/qnx630/"; do
  [ -d "$d" ] || exit_msg 1 "ERR $NODE doesn't exist."
done

# interactively ask user if he wants to ignore $1
ask_ignore() {
  [ -n "$2" ] && echo "$1" >&2
  while echo "Would you like to ignore it? [y/n] "; do
    read x
    case "$x" in
      y|Y) return 0;;
      n|N) return 1;;
      *) echo "Please choose either \`y' as \`yes' or \`n' as \`no'";;
    esac
  done
}

backup() {
  [ -e "$1" ] || return
  echo "INFO Backing up $1."
  mv "$1" "${1}.bak$TSTAMP"
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
    f="$NODE/etc/group"
    m="ERR Group $2 not found."
  else
    f="$NODE/etc/passwd"
    m="ERR User $2 not found."
  fi
  awk -F: "{ if (\$1 == \"$2\") { print \$3; x = 1; exit 0 } }
    END { if (! x) { print \"$m\"; exit 1 } }" < "$f"
  )
}

echo "INFO Extension for backups is \`.bak$TSTAMP'"
echo 'INFO Installing system files...'

cp -rp "$SRCPATH"/common/* "$NODE"
chown -R root:root "$NODE/usr/disam/"
chown -R root:root "$NODE/usr/include/drt"
chmod -R ugo+r "$NODE/usr/include/drt"
ls -1 -d "$NODE"/usr/qnx6*/target/qnx6/usr/include/ 2>/dev/null | while read d; do
  cd "$d"
  ln -s '/usr/include/drt'
  cd "$OLDPWD"
done

echo "INFO Adding groups (if needed) to \`$NODE/etc/group'."

# formerly GID=100
gid_drt="$(group_add 'drt' '' '' '')" || {
  # ignore "Group already exists" error
  [ $? -eq 2 ] || ask_ignore "$gid_drt" || exit 1
  # ensure gid_drt is set (in case of $?==2 or if user ignored some ERR)
  gid_drt="$(get_id gid 'drt')" || exit_msg 1 "$gid_drt"
}

# formerly GID=101
gid_mcs03="$(group_add 'mcs03' '' '' '')" || {
  [ $? -eq 2 ] || ask_ignore "$gid_mcs03" || exit 1
  gid_mcs03="$(get_id gid 'mcs03')" || exit_msg 1 "$gid_mcs03"
}

echo "INFO Adding users (if needed) to \`$NODE/etc/passwd'."


# formerly UID=100 GID=100
ret="$(user_add 'kost' '' "$gid_drt" "$gid_drt"
  'V. Košťál' '/home/kost' '/bin/sh')" ||
# ignore "User already exists" error
[ $? -eq 2 ] || ask_ignore "$ret" || exit 1
if [ -n "$MEA_ST" ]; then
  cp -r "$SRCPATH/common/home/ms/.ph/" "$NODE/home/kost/"
else
  cp -r "$SRCPATH/common/home/ps/.ph/" "$NODE/home/kost/"
fi
chown -R "$gid_drt:$gid_drt" "$NODE/home/kost/.ph/"


# formerly UID=101
uid_pm="$(user_add "$PROJECT" '' '' "$gid_drt"
  'Správce projektu' "/libr/$PROJECT" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_pm" || exit 1
  uid_pm="$(get_id uid "$PROJECT")" || exit_msg 1 "$uid_pm"
}
for d in bin gbin source include ph_app; do
  mkdir -p "$NODE/libr/$PROJECT/$d"
done
if [ -n "$MEA_ST" ]; then
  cp -r "$SRCPATH/common/home/ms/.ph/" "$NODE/libr/$PROJECT"
else
  cp -r "$SRCPATH/common/home/ps/.ph/" "$NODE/libr/$PROJECT"
fi
chown -R "$uid_pm:$gid_drt" "$NODE/libr/$PROJECT/"


# formerly UID=(200 + $NODE_ID)
uid_tu="$(user_add "$TECH_USER" '' '' "$gid_mcs03"
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
  cp -r "$SRCPATH/common/home/ms/.ph/" "$NODE/home/$TECH_USER"
else
  #FIXME cp -r "$SRCPATH/common/home/~ps/.ph/ ...
  cp -r "$SRCPATH/common/home/ps/.ph/" "$NODE/home/$TECH_USER"
fi
chown -R "$uid_tu:$gid_mcs03" "$NODE/home/$TECH_USER"


if [ "$NODE_ID" -eq 1 ]; then
  user_drt="ms_drt"
else
  user_drt="ps_drt"
fi
# formerly UID=199
uid_drt="$(user_add "$user_drt" '' '' "$gid_mcs03"
  'Drt-service' "/home/$user_drt" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_drt" || exit 1
  uid_drt="$(get_id uid "$user_drt")" || exit_msg 1 "$uid_drt"
}
cd "$NODE/home/$user_drt"
for d in exe err litters; do
  ln -s "/home/$TECH_USER/$d"
done
[ "$NODE_ID" -eq 1 ] && {
  for d in alr arc kfg server graf eng monitor; do
    ln -s "/home/$TECH_USER/$d"
  done
}
cd "$OLDPWD"
#FIXME this recursively overwrites owner from "$uid_tu:$gid_mcs03" of
#  "$NODE/home/$TECH_USER"
#chown -R "$uid_drt:$gid_mcs03" "$NODE/home/$user_drt"


# formerly UID=200
uid_admin="$(user_add 'admin' '' '' "$gid_mcs03"
  '' "/home/admin" '/bin/sh')" || {
  [ $? -eq 2 ] || ask_ignore "$uid_admin" || exit 1
  uid_admin="$(get_id uid 'admin')" || exit_msg 1 "$uid_admin"
}
if [ -n "$MEA_ST" ]; then
  cp -r "$SRCPATH/common/home/ms/.ph/" "$NODE/home/admin/"
else
  cp -r "$SRCPATH/common/home/ps/.ph/" "$NODE/home/admin/"
fi
chown -R "$uid_admin:$gid_mcs03" "$NODE/home/admin/"














#FIXME presunout presne tam, kde se instaluje launchmenu na zadost
backup "$NODE/etc/photon/launchmenu"

find ./ -level 1 -name "~*" | while read a; do
  tpath=${a%~*}
  tpath=${tpath#./}
  tdir=${1##*~}
  while read x?"Copy (a) or install (i) $1 [a/i/N]: "; do
    case "$x" in
      a|A|y|Y)
        break ;;
      i|I)
        tdir=${tdir%%-*}
        break ;;
      *)
        echo "ERR Unkown answer, try it again! [y/N/i]" >&2 ;;
    esac
  done
  cp -Mqnx -rv $1 $target_node/$tpath$tdir
done


# ...................... licence

echo ""
echo "Prepis licence :"
echo "    0) skip (bez prepisu)"
echo "    1) licence qnx 6.3.0"
echo "    2) licence qnx 6.3.2"
echo "    3) licence qnx 6.5.0"
let done=0
src_license=""
while test $done -eq 0
do
	read input?" Volba nastaveni licence : "
	case $input in
		0) let done=1;;
 		1) src_license=etc/qnx/license/licenses-630.drt; let done=1;;
 		2) src_license=etc/qnx/license/licenses-632.drt; let done=1;;
 		3) src_license=etc/qnx/license/licenses-650.drt; let done=1;;
		*) ;;
    esac
done
if test "$src_license" != ""; then
	mv -v $target_node/etc/qnx/license/licenses $target_node/etc/qnx/license/licenses.old
	cp -v $src_license $target_node/etc/qnx/license/licenses
fi

# help (the rest is copied through symlinks)
ln -s '/usr/help/product/Disam' "$target_node/usr/qnx630/target/qnx6/usr/help/product/"
ln -s '/usr/help/product/Disam.toc' "$target_node/usr/qnx630/target/qnx6/usr/help/product/"

echo
echo "Replacing symlink dirs with their actual content..."
find ./ -type l | while read a; do
  [ "$(echo "$a" | cut -f1 -d~)" = "$a" ] && {
    [ "$(find "$a" -L -type d | wc -l)" -gt 0 ] && {
      dst="$target_node/$a"
      [ -d "$dst" ] || mkdir -p "$dst"
      #echo "cp -rpcv $1/* $target_node/$dest"
      cp -rpv "$a"/* "$dst"
    }
  }
done

# FIXME don't use *.RLnk any more!
echo
echo "Replacing symlinked files with files they point to..."
find ./ -type l -name "*.RLnk" | while read a; do
  rm -v -f "$target_node/$a"
  cp -v "$a" "$target_node/$(echo "$a" | sed -e 's|[.]RLnk$||')"
done
