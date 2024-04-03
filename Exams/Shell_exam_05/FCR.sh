#!/bin/sh

#Mi sposto nella directory corrente
cd $2

#Scorro ogni elemento della directory
for e in *
do
	#Se l'elemento è un file leggibile
	if test -f $e -a -r $e
	then
		#Estraggo il numero di linee che terminano con il carattere 't'
		nLines=`grep 't$' $e | wc -l`
		#Se ci sono X linee che terminano con 't'
		if test $nLines -eq $1
		then
			#Aggiungo il percorso del file all'elenco
			echo `pwd`/$e >> $3
		fi
	fi
done

#Esplorazione ricorsiva
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		FCR.sh $1 `pwd`/$i $3
	fi
done