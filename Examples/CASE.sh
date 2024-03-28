#!/bin/sh

echo ----------------------------------------------------------------------
echo Controllo di ciascun parametro passato al file comandi
echo ----------------------------------------------------------------------

#Ciclo for per iterare sui parametri passati
count=0
for par in $*
do
	count=`expr $count + 1`
	
	#Controllo sul tipo di percorso
	case $par in
	/*)
		#Se il parametro inizia con / allora ho un nome assoluto
		echo Il parametro $count ha nome ASSOLUTO: $par
	;;
	*/*)
		#Se il parametro contiene / al suo interno allora ho nome relativo
		echo Il parametro $count ha nome RELATIVO: $par
	;;
	*)
		#Giunti a questo caso allore il parameto non contiene / quindi è semplice
		echo Il parametro $count ha nome RELATIVO SEMPLICE: $par
	;;
	esac
done