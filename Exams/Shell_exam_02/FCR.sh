#!/bin/sh

#Mi sposto nella directory in cui effettuare la ricerca
cd $1

#Variabile vuota per contenere i nomi dei file
fileList=

#Scorro ciascun elemento della directory
for e in *
do
	#Se l'elemento è un file leggibile...
	if test -f $e -a -r $e
	then
		#Ricavo il numero di linee del file
		lineNumber=`wc -l < $e`

		#Ricavo il numero di linee che contengono un carattere numerico
		N=`grep '[0-9]' $e | wc -l`

		#Controllo se ci sono $2 linee ciascuna con carattere numerico
		if test $lineNumber -eq $2 -a $N -eq $lineNumber
		then
			#Salviamo il nome relativo del file
			fileList="$fileList $e"
			echo "DEBUG-Trovato file $e idoneo"
		fi
	fi
done

#Se ho trovato almeno un file
if test "$fileList"
then 
	echo "DEBUG-Trovata directory con file idonei: `pwd`";
	echo "DEBUG-Elenco file idonei: $fileList";

	#Aggiorno l'elenco delle directory idonee
	echo `pwd` >> $3

	#Aggiorno l'elenco dei file idonei
	echo $fileList >> $4
fi

#Esplorazione ricorsiva: per ogni elemento e della directory corrente...
for e in *
do
	#Se è una directory traversabile --> esplorazione
	if test -d $e -a -x $e
	then
		FCR.sh `pwd`/$e $2 $3 $4
	fi
done