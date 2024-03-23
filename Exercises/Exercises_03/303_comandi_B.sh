#!/bin/sh

#08 - controllo lasco con case

case $# in
0)
	echo "DEBUG-Nessun parametro: stampo ogni elemento"
	ls -lR | more ;;
1)
	if test -d $1
	then
		echo "DEBUG-Ora eseguo per directory $1"
		ls -dl $1
	else
		echo "DEBUG-Ora eseguo per file $1"
		ls -al $1
	fi ;;
*) exit 1;;
esac

echo "DEBUG-Fine Esecuzione"