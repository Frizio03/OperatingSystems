#!/bin/sh

#Mi sposto nella directory di interesse
cd $1

#Variabile per salvare i nomi relativi semplici dei file
F=
X=$2
L=$3

#Scorro ogni elemento .txt della directory
for file in *.txt
do
	echo Esamino il file $file
	#Se l'elemento è un file leggibile
	if test -f $file -a -r $file
	then
		#Ricavo il numero di linee del file
		NL=`wc -l < $file`

		#Se le linee sono strettamente minori del numero X
		if test $NL -lt $X
		then
			#Aggiungo in append il percorso assoluto del file
			echo `pwd`/$file >> $4

			#Aggiorno la lunghezza massima se necessario
			if test $NL -gt $L
			then
				L=$NL
			fi
		fi
	fi
done

#Esplorazione ricorsiva delle sotto-directory
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		#Invocazione ricorsiva
		echo "-- Esplorazione di $i"
		FCR.sh `pwd`/$i $X $L $4

		#Aggiorno il valore di L dopo la ricorsione
		L=$?
	fi
done

#Fornisco la lunghezza massima come valore di ritorno
exit $L