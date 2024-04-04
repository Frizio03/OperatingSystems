#!/bin/sh

#Mi sposto nella directory di interessa
cd $1

#Scorro ogni elemento della directory
for e in *
do
	#Se l'elemento è un file leggibile
	if test -f $e -a -r $e
	then
		#Ricavo il numero di linee del file
		NL=`wc -l < $e`

		#Se il file ha almeno Y linee...
		if test $NL -ge $2
		then
			#Se il file ha meno di 5 linee
			if test $NL -lt 5
			then
				#Creo un file vuoto
				> $e.NOquinta
				
				#Aggiunta del file all'elenco
				echo `pwd`/$e.NOquinta >> $3
			else
				#Ricavo la quinta linea del file (se esiste)
				QUINTA=`head -5 $e | tail -1`
				
				#Creo un file e aggiungo la quinta linea
				echo $QUINTA > $e.quinta

				#Aggiunta del file all'elenco
				echo `pwd`/$e.quinta >> $3
			fi
		fi
	fi
done

#Esplorazione ricorsiva
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		echo ESPLORAZIONE DI $i
		#Invocazione ricorsiva
		FCR.sh `pwd`/$i $2 $3
	fi
done