#!/bin/sh

#Mi sposto nella directory passata come parametro
cd $1

#Inizializzazione variabile di controllo
trovato=0

#Scorro ciascun elemento della directory
for e in *
do
	#Se l'elemento è un file leggibile
	if test -f $e -a -r $e
	then
		#Salvo le linee del file
		nLines=`wc -l < $e`
		#Se il file ha K linee
		if test $nLines -eq $2
		then
			#Aggiungo il file all'elenco
			trovato=1
			echo "DEBUG-aggiunta file"
			echo `pwd`/$e >> $4
		fi
	fi
done

#Se ho trovato almeno un file idoneo nella diorectory
if test $trovato -eq 1
then
	#Aggiungo la directory all'elenco
	echo "DEBUG-aggiunta directory"
	echo `pwd` >> $3
fi

#Esplorazione ricorsiva delle sotto-directory
for i in `ls -A`
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		#Esplorazione della directory
		FCR.sh `pwd`/$i $2 $3 $4
	fi
done