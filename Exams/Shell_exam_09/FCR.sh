#!/bin/sh

#Mi sposto nella directroy intersssata
cd $1

#Creo una variabile per vedere se la directory è idonea
idonea=0

#Valuto ciascun elemento della directory
for e in *
do
	#Se l'elemento è una directory
	if test -d $e
	then
		#La directory corrente non è idonea
		idonea=0
		break
	fi

	#Se l'elemento è un file leggibile
	if test -f $e -a -r $e
	then
		#Ricavo la lunghezza del file
		NL=`wc -l < $e`
		#Controllo che il numero di linee sia superiore ad X
		if test $NL -le $2
		then
			idonea=0
			break
		else
			idonea=1
		fi
	fi
done

#Se la directory risulta idonea
if test $idonea -eq 1
then
	#Aggiorno il conteggio globale
	n=`cat $3`
	n=`expr $n + 1`
	echo $n > $3

	#Aggiorno l'elenco dei file
	for f in *
	do
		echo `pwd`/$f >> $4
	done

	#Aggiungo la directory all'elenco
	echo `pwd` >> $5
fi

#Esplorazione ricorsiva sotto-directory
for i in *
do
	#Controllo se l'elemento è directory traversabile
	if test -d $i -a -x $i
	then
		FCR.sh `pwd`/$i $2 $3 $4 $5
	fi
done