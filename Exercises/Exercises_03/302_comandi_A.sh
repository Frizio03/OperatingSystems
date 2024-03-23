#!/bin/sh

#05 - exit con valore -1
#exit -1		#Valore non contemplato exit [0-254]

#06
if test $# -ne 1
then exit 1
fi

echo 'Il valore di $0 ===>' $0
echo 'Il valore di $1 ===>' $1

#07
if test -d $1
then
	echo "DEBUG-Ora eseguo per directory $1"
	ls -dl $1
else
	echo "DEBUG-Ora eseguo per file $1"
	ls -al $1
fi

echo "DEBUG-Fine Esecuzione"