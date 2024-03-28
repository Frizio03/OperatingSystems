#!/bin/sh

#Mi sposto nella directory di inizio della gerarchia
cd $1

#Lettura dal file temporaneo il conteggio e inserimento nella variabile conta
read conta < /tmp/contatmp

#Per ciascun elemento della directory...
for i in *
do
	#...se l'elemento è un file...
	if test -f $i
	then 	
		#...incremento il contatore
		conta=`expr $conta + 1`
	fi
done

#Stampa di debug da commenatare dopo il test
echo DEBUG-in `pwd` conta = $conta

#Aggiornamento variabile nel file temporaneo
echo $conta > /tmp/contatmp

#Per ogni elemento della directory corrente...
for i in *
do
	#... se l'elemento è una directory traversabile...
	if test -d $i -a -x $i
	then
		#... invocazione ricorsiva del file con parametro in forma assoluta
		FCR.sh `pwd`/$i
		#Lettura del conteggio della sotto-gerarchia esplorata
		read conta < /tmp/contatmp
	fi
done
