#!/bin/sh

#Mi sposto nella directory di interesse
cd $1

#Inizializzo un contatore e una variabile di risultato
count=0
trovato=false

#Scorro tutti gli elementi della directory corrente
for file in *
do
	#Controllo se l'elemento è un file
	if test -f $file
	then
		#Aggiorno il contatore dei file
		count=`expr $count + 1`
		
		#Controllo se il file è il quarto file della directory
		if test $count -eq 4
		then
			#Aggiorno la variabile di controllo
			trovato=true
			#Aggiungo il nome assoluto del file all'elenco file
			echo `pwd`/$file >> $3
		fi
	fi
done

#Controllo se non ho torvato il 4° file
if test $trovato = false
then
	#Aggiungo il nome assoluto della directory all'elenco directory
	pwd >> $2
	#Messaggio per spiegare che non ci sono file in quarta posizione nella dir corrente
	echo ATTENZIONE nessun file in quarta posizione in: `pwd`
fi

#Esplorazione ricorsiva delle sotto-directory
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		#Invocazione ricorsiva passando il nome assoluto della directory
		#e con messaggio che indica la directory che sarà esplorata
		echo Esplorazione di $i
		FCR.sh `pwd`/$i $2 $3
	fi
done