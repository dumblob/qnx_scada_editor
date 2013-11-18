#!/bin/sh

#TODO
#  rozbehnout sit (touch ... a restart)
#  nastavit fonty ???

TSTAMP="$(date '+%Y%m%d-%H%M%S')"
SRCPATH='./'
#FIXME $project
PROJECT=
#FIXME target_node=/net/$tnode
NODE=
#FIXME tnode_id
NODEID=

print_help() {
  echo "SYNOPSIS"
  echo "  Installs SCADA system-wide on measure- or work- station node."
  echo "  Owerwritten files are backed-up."
  echo "USAGE"
  echo "  install.sh [-h]"
  echo "  install.sh [-s <src_path>] -p <project_name> -n <node> -i <node_id>"
  echo "OPTIONS"
  echo "  -s <src_path>"
  echo "    path to dir with all SCADA files (those, which will be installed)"
  echo "    default: ./"
  echo "  -p <project_name>"
  echo "    name of the project"
  echo "  -n <node>"
  echo "    path to current node (e.g. /net/local_node_name)"
  echo "  -i <node_id>"
  echo "    node number (ID)"
  exit $1
}

while [ $# -gt 0 ]; do
  case "$1" in
    '-h')
      print_help 0 ;;
    '-s')
      shift
      [ $# -ge 1 ] || {
        echo "ERR -s demands <src_path>." >&2
        exit 1
      }
      SRCPATH="$1" ;;
    '-p')
      shift
      [ $# -ge 1 ] || {
        echo "ERR -p demands <project_name>." >&2
        exit 1
      }
      PROJECT="$1" ;;
    '-n')
      shift
      [ $# -ge 1 ] || {
        echo "ERR -p demands <node>." >&2
        exit 1
      }
      NODE="$1" ;;
    '-i')
      shift
      [ $# -ge 1 ] || {
        echo "ERR -p demands <node_num>." >&2
        exit 1
      }
      NODEID="$1" ;;
    *)
      echo "ERR Unknown argument $1" >&2
      exit 1
  esac
  shift
done

shift
if [ $# -gt 0 ]; then
  echo "ERR Unknown arguments given." >&2
  exit 1
elif [ -z "$PROJECT" ]; then
  echo "ERR Non-empty <project_name> desired." >&2
  exit 1
elif [ -z "$NODE" ]; then
  echo "ERR Non-empty <node> desired." >&2
  exit 1
elif [ -z "$NODEID" ]; then
  echo "ERR Non-empty <node_id> desired." >&2
  exit 1
elif [ $(if -u) -eq 0 ]; then
  echo "ERR root privileges needed." >&2
  exit 1
fi

for f in "$NODE" "$NODE/usr/qnx630/"; do
  [ -d "$f" ] || {
    echo "ERR $NODE doesn't exist." >&2
    exit 1
  }
done

# name pass GID user_list
group_add() {
  [ $# -eq 4 ] || return 1
  grep -E "^$1" < "$NODE/etc/group" > /dev/null 2>&1 && {
    echo "ERR User \`$1' already exists."
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
  unset _gid
}

# login pass UID GID comment home_dir cmd_interpret
user_add() {
  [ $# -eq 7 ] || return 1
  grep -E "^$1" < "$NODE/etc/passwd" > /dev/null 2>&1 && {
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
  unset _uid
}

echo "INFO Extension for backups is \`.bak$TSTAMP'"
#FIXME presunout tam, kde se instaluje launchmenu (je to v nasledujicim cyklu?)
echo "INFO Backing up files..."
mv "$NODE/etc/photon/launchmenu" "$NODE/etc/photon/launchmenu.bak$TSTAMP"

echo 'INFO Installing system files...'
for f in "$SRCPATH"/*/; do
  basename "$f" | awk '/^~\*/ { exit(1) }' && cp -rp "$f" "$NODE"
done
chown -R root:root "$NODE/usr/disam/"

# Photon extension
for f in "$NODE"/usr/qnx6*/target/qnx6/usr/include/; do
  cp -rpv "$SRCPATH/usr/include/drt/" "$f"
done

echo "INFO Adding groups (if needed) to \`$NODE/etc/group'."
gid_drt="$(group_add 'drt' '' '' '')"
[ $? -eq 3 ] { echo "$gid_drt" >&2; exit 1; }
gid_mcs03="$(group_add 'mcs03' '' '' '')"
[ $? -eq 3 ] { echo "$gid_mcs03" >&2; exit 1; }

echo "INFO Adding users (if needed) to \`$NODE/etc/passwd'."
ret="$(user_add 'kost' '' "$gid_drt" "$gid_drt"
  'V.Košťál' '/home/kost' '/bin/sh')" || { echo "$ret" >&2; exit 1; }
mkdir -p "$NODE/home/kost/.ph/"
#FIXME merici stanice
#cp -r "$SRCPATH/home/ms/.ph/" "$NODE/home/kost/.ph/"
#FIXME pracovni stanice
cp -r "$SRCPATH/home/ps/.ph/" "$NODE/home/kost/.ph/"
chown -R "$gid_drt":"$gid_drt" "$NODE/home/kost/.ph/"

[ -d "$NODE/libr/$PROJECT" ] || mkdir -p "$NODE/libr/$PROJECT"
# project manager
grep -e "$project" $target_node/etc/passwd > /dev/null
if test $? -ne 0; then
	puid=`grep -e $project /etc/passwd | cut -f3 -d:`
	if test -z "$puid"; then
		puid="101"
	fi
	mkdir $target_node/libr/$project/bin
	mkdir $target_node/libr/$project/gbin
	mkdir $target_node/libr/$project/source
	mkdir $target_node/libr/$project/include
	mkdir $target_node/libr/$project/ph_app
	mkdir $target_node/libr/$project/.ph
	if test $inode -eq 1; then
		cp -rc home/~ms_admin/.ph/* $target_node/libr/$project/.ph/
	else
		cp -rc home/~ps_admin/.ph/* $target_node/libr/$project/.ph/
	fi
	chown -R $puid:100 $target_node/libr/$project
	echo "$project::$puid:100:Správce-projectu:/libr/$project:/bin/sh" >> $target_node/etc/passwd
else
	echo "user $project already defined"
fi
#
echo "User name ?"
read tuser
let ipom=200+$tnode_id
grep -e "$tuser" $target_node/etc/passwd > /dev/null		# technologicky uzivatel

if test $? -ne 0; then
	echo "$tuser::$ipom:101:Std. uživatel:/home/$tuser:/bin/sh" >> $target_node/etc/passwd
	if test ! -d $target_node/home/$tuser; then
		mkdir $target_node/home/$tuser
	fi
else
	echo "uzivatel $tuser already defined"
fi
mkdir $target_node/home/$tuser/exe
mkdir $target_node/home/$tuser/err
mkdir $target_node/home/$tuser/litters
mkdir $target_node/home/$tuser/.ph
if test $tnode_id -eq 1; then
	mkdir $target_node/home/$tuser/alr
	mkdir $target_node/home/$tuser/arc
   	mkdir $target_node/home/$tuser/kfg
   	mkdir $target_node/home/$tuser/server
   	mkdir $target_node/home/$tuser/graf
	mkdir $target_node/home/$tuser/eng
	mkdir $target_node/home/$tuser/monitor
	cp -rc home/~ms/.ph/* $target_node/home/$tuser/.ph/
else
	cp -rc home/~ps/.ph/* $target_node/home/$tuser/.ph/
fi
chown -R $ipom:101 $target_node/home/$tuser
#chmod -R g+wx $target_node/home/$tuser

grep -e "drt" $target_node/etc/passwd > /dev/null		# drt-service
if test $? -ne 0; then
	if test $tnode_id -eq 1; then
		service="ms_drt"
	else
		service="ps_drt"
	fi
	echo "$service::199:101:Drt-service:/home/$service:/bin/sh" >> $target_node/etc/passwd
	if test ! -d $target_node/home/$service; then
		mkdir $target_node/home/$service
	fi
else
	echo "uzivatel $service already defined"
fi
mkdir $target_node/home/$tuser/.ph
tmp="$PWD"
cd $target_node/home/$home/$service
ln -s /home/$tuser/err .
ln -s /home/$tuser/exe .
ln -s /home/$tuser/litters
[ $tnode_id -eq 1 ] && {
  ln -s /home/$tuser/alr .
  ln -s /home/$tuser/arc .
  ln -s /home/$tuser/kfg
  ln -s /home/$tuser/server .
  ln -s /home/$tuser/graf .
  ln -s /home/$tuser/eng .
  ln -s /home/$tuser/monitor .
}
chown -R 199:101 $target_node/home/$service
cd "$tmp"
#	chmod -R g+wx $target_node/home/$service

if grep -e "admin" $target_node/etc/passwd > /dev/null; then
	mkdir $target_node/home/admin
	mkdir $target_node/home/admin/.ph
	if test $inode -eq 1; then
		cp -rc home/~ms_admin/.ph/* $target_node/home/admin/.ph/
	else
		cp -rc home/~ps_admin/.ph/* $target_node/home/admin/.ph/
	fi
	chown -R 200:101 $target_node/home/admin
	echo "admin::200:101:Administrator:/home/admin:/bin/sh" >> $target_node/etc/passwd
else
	echo "user admin already defined"
fi

# os release specific

find ./ -level 1 -name "~*" | while read a; do
  tpath=${a%~*}
  tpath=${tpath#./}
  tdir=${1##*~}
  read input?"Copy (a) or install (i) $1 [a/i/N]: "
  if test "$input" = "a" || test "$input" = "A" || test "$input" = "y" || test "$input" = "Y"; then
    echo "cp -Mqnx -rv $1 $target_node/$tpath$tdir"
    cp -Mqnx -rv $1 $target_node/$tpath$tdir
  elif test "$input" = "i" || test "$input" = "I"; then
    tdir=${tdir%%-*}
    echo "cp -Mqnx -rv $1 $target_node/$tpath$tdir"
    cp -Mqnx -rv $1 $target_node/$tpath$tdir
  fi
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
