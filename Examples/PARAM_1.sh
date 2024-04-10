#!/bin/sh

#Controllo se il primo parametro è intero positivo
case $1 in
*[!0-9]*)
	echo ERRORE: primo parametro non numero;;
*)
	echo OK primo parametro;;
esac

#Salvataggio del primo parametro in caso di riutilizzo
X=$1

#Shift a sx della lista dei parametri
shift

#Controllo dei parametri restanti
for par in $*
do
	case $par in
		/*)
			echo NOME ASSOLUTO $par;;
		*/*)
			echo NOME RELATIVO $par;;
		*)
			echo NOME RELATIVO SEMPLICE $par;;
	esac
done