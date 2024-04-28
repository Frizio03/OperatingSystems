#!/bin/sh

#Mi sposto nella directory di interesse
cd $1

#Variabile per identificare livello corrente
curLevel=`expr $2 + 1`
returnValue=$curLevel

#Scorro ciascun elemento della directory
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		echo ESPLORAZIONE DI $i
		FCR.sh `pwd`/$i $curLevel $3
		
		#Salvo il valore di ritorno del file
		returnValue=$?
	fi
done

#Leggo il valore nel file temporaneo
read previous < $3

#Se il valore di ritorno è > del valore precedente...
if test $returnValue -gt $previous
then
	#...aggiorno il numero totale di livelli
	echo $returnValue > $3
fi

#Fornisco come valore di ritorno l'indice di livello corrente
exit $curLevel