#!/bin/sh

#16
if test $# -lt 1
then 
	echo Errore: Parametri insufficienti
	exit 1
fi

if test ! -f $1 -o ! -r $1
then 
	echo Errore: $1 non è un file oppure non leggibile
	exit 2
fi

for nomeFile in $*
do
	echo "vuoi visualizzare il file $1 (si/no)?" > /dev/tty
	read risposta
	case $risposta in
	S* | s* | Y* | y*)
		echo OK Visualizzo le info del file $nomeFile
		ls -al $nomeFile
		echo contenuto del file $nomeFile
		cat $nomeFile
	;;
	*)
		echo Non visualizzo le info ed il contenuto del file $nomeFile
	;;
	esac
done