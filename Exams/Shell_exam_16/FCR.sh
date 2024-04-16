#!/bin/sh

#Mi sposto nella directory di interesse
cd $5

#Scorro tutti gli elementi della directory con nome che termina per S1
for F in *.$1
do
	#Se l'elemento è un file
	if test -f $F
	then
		#Aggiungo il suo nome assoluto all'elenco
		echo `pwd`/$F >> $3
	fi
done

#Scorro tutti gli elementi della directory con nome che termina per S2
for F in *.$2
do
	#Se l'elemento è un file
	if test -f $F
	then
		#Aggiungo il suo nome assoluto all'elenco
		echo `pwd`/$F >> $4
	fi
done

#Esplorazione ricorsiva delle sotto-directory
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		#esplorazione ricorsiva passando il nome assoluto
		FCR.sh $1 $2 $3 $4 `pwd`/$i
	fi
done