#!/bin/bash
COMFILE=/proc/hashmap

#Check flag
if [ $1 = "save" ]; then
	#Send code for save.
	echo -e '\x04' > $COMFILE

	#copy the data to a specified file
	cat $COMFILE > $2
elif [ $1 = "load" ]; then
	 cat $2 >> $COMFILE
else

	echo "Unknown commando usage"

fi
