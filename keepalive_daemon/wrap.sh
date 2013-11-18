#!/bin/sh

INT="$1"
IP="$2"
NM="$3"

x() {
  ifconfig $INT $IP netmask $NM -alias
  #FIXME $8 ?????????
  kill "$(ps -Af | awk '{ if ($8 = "") { print $8 } }')"
}
trap x SIGTERM

ifconfig $INT $IP netmask $NM alias
master -d sys
while :; do sleep 999999; done
