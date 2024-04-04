#!/bin/sh

#Mi sposto nella directory di interesse
cd $1

#Variabile per identificare livello corrente
curLevel=`expr $2 + 1`
returnValue=$curLevel

#Controllo della fase
if test $3 -eq 1
then
	#Scorro ciascun elemento della directory
	for i in *
	do
		#Se l'elemento è una directory traversabile
		if test -d $i -a -x $i
		then
			echo ESPLORAZIONE DI $i
			FCR.sh `pwd`/$i $curLevel $3 $4

			#Salvo il valore di ritorno del file
			returnValue=$?
		fi
	done

	#Leggo il valore nel file temporaneo
	read previous < $4

	#Se il valore di ritorno è > del valore precedente...
	if test $returnValue -gt $previous
	then
		#...aggiorno il numero totale di livelli
		echo $returnValue > $4
	fi
fi

#Controllo della fase
if test $3 -eq 2
then

	#Se sono al livello specificato
	if test $4 -eq $curLevel
	then
		#Stampo gli elementi del livello
		echo
		echo ">>>>>> Elementi della directory `pwd`"
		ls -l
	fi

	#Esplorazione ricorsiva delle sotto-directory
	for i in *
	do
		#Se l'elemento è una directory traversabile
		if test -d $i -a -x $i
		then
			#Chiamata ricorsiva
			FCR.sh `pwd`/$i $curLevel $3 $4
		fi
	done
fi

#Fornisco come valore di ritorno l'indice di livello corrente
exit $curLevel