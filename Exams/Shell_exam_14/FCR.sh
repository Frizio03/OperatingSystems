#!/bin/sh

#Mi sposto nella directory di interesse
cd $1

#Salvo il valore iniziale del contatore
conta=$3

#Scorro tutti gli elementi della directory
for F in *
do
	#Se l'elemento è un file leggibile
	if test -f $F -a -r $F
	then
		#Ricavo il numero di linee
		NL=`wc -l < $F`

		#Se il file contiene X linee
		if test $NL -eq $2
		then
			#Aggiorno il contatore
			conta=`expr $conta + 1`

			#Aggiungo il file idoneo all'elenco
			echo `pwd`/$F >> $4
		fi
	fi
done

#Esplorazione ricorsiva delle sotto-directory
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		#Esplorazione ricorsiva
		FCR.sh `pwd`/$i $2 $conta $4

		#Aggiornamento del contatore dopo eplorazione ricorsiva
		conta=$?
	fi
done

#Fornisco il valore di conta come valore di ritorno
exit $conta