#!/bin/sh

#11

#Controllo sul numero di parametri
if test $# -ne 1
then exit 1
fi

echo "DEBUG: Stampo il valore del parametreo 1: $1"

#Controllo se il percorso è relativo semplice
case $1 in
/*)	echo Questo è un percorso ASSOLUTO.
	exit 2 ;;
*/*) echo Questo è un percorso RELATIVO.
	exit 3 ;;
*)	echo OK: questo è un percorso RELATIVO SEMPLICE.
	if test -f $1
	then echo Il file ha percorso assoluto: `pwd`/$1
	else echo NON ESISTE in questa directory un file nominato $1
	fi
;;
esac