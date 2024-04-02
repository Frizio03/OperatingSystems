#!/bin/sh

#Salvataggio parametri in variabili con nomi sensati
G=$1; N=$2; H=$3 Cx=$4; fileTemp=$5

#Mi sposto all'interno della directory selezionata
cd $G

#Inizializzazione contatore
count=0

#Scorro ogni elemento della directory corrente
for e in *
do
	#Se l'elemento è un file leggibile...
	if test -f $e -a -r $e
	then
		#Salvo il numero linee e il numero di linee con Cx
		nLines=`wc -l < $e`
		nChar=`grep $Cx $e | wc -l`

		echo "DEBUG-File $e con $nLines linee e $nChar linee con Cx"

		#Se il file ha H linee e almeno una occorrenza di Cx (in una sua linea)
		if test $nLines -eq $H -a $nChar -ne 0
		then
			#Aggiorno il contatore
			count=`expr $count + 1`
			echo "DEBUG-Il contatore vale $count"
		fi
	fi
done

#Se ci sono N file idonei aggiungo la directory all'elenco
if test $count -eq $N
then
	echo `pwd` >> $fileTemp
fi

#Esplorazione ricorsiva
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		echo "DEBUG-Procedo ad esplorare la directory $i"
		FCR.sh $i $2 $3 $4 $5
	fi
done