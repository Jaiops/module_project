#!/bin/bash
COMFILE=/proc/hashmap

if [ $1 = "load" ]; then
	 cat $2 >> $COMFILE
fi