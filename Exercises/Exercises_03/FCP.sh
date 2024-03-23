#!/bin/sh

#Controllo il numero di parametri
if test $# -ne 1
then
	echo Si richiede 1 parametro
	exit 1
fi

#Controllo sul parametro
case $1 in
/*)
	echo OK: percorso assoluto
	if test ! -d $1 -o ! -x $1
	then
		echo ERRORE: directory inesistente o non traversabile
		exit 2
	fi
;;
*)
	echo ERRORE: percorso relativo
	exit 3
;;
esac

#A questo punto è possibile invocare il file con il parametro passato
./file-dir.sh $1