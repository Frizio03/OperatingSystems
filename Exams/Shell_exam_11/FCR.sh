#!/bin/sh

#Mi sposoto nella directory corrente
cd $1

#Se trovo un file leggibile di nome F
if test -f $2 -a -r $2
then
	#Conto le righe del file
	NL=`wc -l < $2`
	#Se il file ha almeno 4 linee
	if test $NL -ge 4
	then
		#Aggiungo il nome assoluto all'elenco assieme al numero linee
		echo `pwd`/$2 $NL >> $3
	fi
fi

#Parte ricorsiva: scorro ogni elemento della directory
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		#Esplorazione ricorsiva della sotto-gerarchia
		echo "DEBUG-Esplorazione di $i"
		FCR.sh `pwd`/$i $2 $3
	fi
done