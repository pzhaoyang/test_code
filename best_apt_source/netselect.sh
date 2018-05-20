#!/bin/sh

serv=$(curl -s -L http://mirrors.ubuntu.com/mirrors.txt)
block="-r 0-102400"
output="-w %{speed_download}"
trash=" -o /dev/null"
download_file="ls-lR.gz"
connect_timeout="--connect-time 5"
max_timeout="-m 30"
[ -z "$serv" ] && echo "serv is null, please try again." && exit 0

fast_speed=0
source=""
for s in $serv
do
	speed=$(curl $connect_timeout $max_timeout -s $block ${output} $s/$download_file $trash)
	[ $(echo "$fast_speed == 0"|bc) = 1 ] && fast_speed=$speed

	if [ $(echo "$speed > 0"|bc) = 1 ] && [ $(echo "$fast_speed <= $speed"|bc) = 1 ]; then
		fast_speed=$speed;
		source=$s
    fi
done

echo -e "The fast source is:\n$s"