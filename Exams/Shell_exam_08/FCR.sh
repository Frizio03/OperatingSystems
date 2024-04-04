#!/bin/sh

#Mi sposto nella directory interessata
cd $1

#Inizializzazione contatore su cartella
curCount=0

#Scorro ogni elemento della directory
for e in *
do
	#Se l'elemento è un file leggibile
	if test -f $e -a -r $e
	then
		#Conto le linee del file
		NL=`wc -l < $e`
		#Conto le linee che iniziano per 'a'
		NS=`grep '^a' $e | wc -l`

		#Se tutte le linee finiscono per 'a'
		if test $NL -eq $NS
		then
			#Incremento il contatore
			curCount=`expr $curCount + 1`
			#Aggiungo il nome assoluto del file all'elenco
			echo `pwd`/$e >> $3
		fi
	fi
done

#Aggiornamento del file di conteggio
num=`cat $2`
num=`expr $num + $curCount`
echo $num > $2

#Esplorazione ricorsiva delle sotto-gerarchie

#Scorro ogni elemento della directory
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		#Continuo l'esplorazione
		echo "ESPLORAZIONE DI `pwd`/$i"
		FCR.sh `pwd`/$i $2 $3
	fi
done