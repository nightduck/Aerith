##Aerith v1.0
##Author - nightduck
##
##	This is the initially called script. It checks that its being run with root
##	priviliges, configures a couple GPIO pins, then launches the fetch.sh and
##	sendData programs

if (($EUID != 0))
	then echo "Must be run as root"
	exit
fi

gpio -g mode 22 output
gpio -g mode 25 input
./fetch.sh
./sendData
