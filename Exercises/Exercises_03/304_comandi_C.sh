#!/bin/sh

#Controllo sul numero di parametri
if test $# -ne 1
then exit 1
fi

#09 - Controllo del tipo di percorso passato
case $1 in
/*) echo Questo è un percorso ASSOLUTO.
	
	#10 - Controllo se è un file o una directory traversabile
	if test ! -d $1
	then
		echo Questa percorso indica un FILE
	else
		echo Questo percorso indica una DIRECTORY
		if test -x $1
		then echo Directory TRAVERSABILE
		else echo Directory NON TRAVERSABILE
		fi
	fi
;;
*/*) echo Questo è un percorso RELATIVO. ;;
*) echo Questo è un percorso RELATIVO SEMPLICE. ;;
esac