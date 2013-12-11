#!/bin/sh

[ $# -eq 3 ] || {
  echo "USAGE: <net_interface> <ipv4_addr> <netmask>"
  exit 1
}

INT="$1"
IP="$2"
NM="$3"

x() {
  # remove alias
  ifconfig "$INT" "$IP" netmask "$NM" -alias
  # simulate `killall'
  ps -Af | awk '{
    if ($8 = "master" && $9 == "-d") { print $2 }
  }' | while read pid; do
    kill "$pid"
  done
  exit
}
trap x SIGTERM

ifconfig "$INT" "$IP" netmask "$NM" alias
master -d sys

while :; do sleep 32767; done
