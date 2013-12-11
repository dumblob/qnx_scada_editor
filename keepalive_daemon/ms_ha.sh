#!/bin/sh

# ifconfig...
[ -z "$PATH" ] && PATH="/sbin" || PATH="/sbin:$PATH"
export PATH

FIP="192.168.0.155"  # floating IP
DAEMON='./keepalive_daemon'

SBIP="192.168.0.129"  # side B ("other station")
INT="wm0"             # network interface for IP alias
NM="255.255.255.0"    # netmask
PORT="23455"          # port on which master runs

# for sure
ifconfig "$INT" "$FIP" netmask "$NM" -alias 2>/dev/null

# sleep randomly for 0..20s
sleep "$(echo "$(tr -c -d 0-9 < /dev/urandom | head -c -n 4) * 20 / 9999" | bc)"

cd "$(dirname "$0")"
if ping -c 1 "$FIP" > /dev/null; then
  # start "slave"
  "$DAEMON" -s "exec ./wrap.sh $INT $FIP $NM" "$SBIP" "$PORT" &
else
  # start "master"
  "$DAEMON" -m "$PORT" &
  #. /home/ms/.profile  # not needed if the `-' arg is given to su
  #su - ms -c master -d sys  # run SCADA in measuring mode
  . /etc/profile.d/scada.sh
  master -w sys &
  sleep 10
  on -w photon rep01 -c &  # wait for photon to exist
fi
