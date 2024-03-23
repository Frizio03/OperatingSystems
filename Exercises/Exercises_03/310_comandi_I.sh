#!/bin/sh

#17 - mostro solo i nomi di file nascosti nella cartella
for nomeFile in dataFiles/.*
do
	echo $nomeFile
done

#18 - Visualizzo solo file o directroy il cui nome inizia per p
for nomeFile in dataFiles/p*
do
	#Controllo se è una directory
	if test -d $nomeFile
	then
		echo $nomeFile ---- DIRECTORY
	else
		echo $nomeFile ---- FILE
	fi
done

#19 - Visualizzo solo file o directroy il cui nome inizia per 3
count=1
for nomeFile in ./3*
do
	#Controllo se è una directory
	if test -d $nomeFile
	then
		echo $nomeFile ---- DIRECTORY
	#Stampo con ridirezione
	else
		echo Stampo il percorso del file N° $count: `pwd`/$nomeFile >> dataFiles/tmp/elenco.txt
		count=`expr $count + 1`
	fi
done

#20 - 

#Controllo il numero di parametri
if test $# -ne 1
then
	echo Numero parametri errato - passati: $#, richiesti: 1
	exit 1
fi

#Controllo se il percorso è relativo semplice
case $1 in
/*)	echo Questo è un percorso ASSOLUTO.
	exit 2 ;;
*/*) echo Questo è un percorso RELATIVO.
	exit 3 ;;
*)	echo OK: questo è un percorso RELATIVO SEMPLICE.
	
	c=0
	for i in `cat ./dataFiles/file20/$1`
	do
	 	c=`expr $c + 1`
	        if test `expr $c % 2` -eq 1
		then 
			echo Elemento dispari, quindi nome di file: $i
			if test -f $i
			then
				cat $i
			fi
	        else
				echo Elemento pari, quindi lunghezza in linee del file appena visualizzato = `wc -l < ./dataFiles/file20/$i`
	        fi
	done
	
;;
esac