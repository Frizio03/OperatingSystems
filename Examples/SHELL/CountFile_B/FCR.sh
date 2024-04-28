#!/bin/sh

#Mi sposto nella directory di inizio della gerarchia
cd $1

#Per ogni elemento della directory corrente...
for i in *
do
	#...se l'elemento è un file...
	if test -f $i
	then
		#...si aggiorna il contatore (variabile di ambiente)
		conta=`expr $conta + 1`
	fi
done

#Stampa di debug da commenatare dopo il test
echo DEBUG-in `pwd` conta = $conta

#Per ogni elemento della directory corrente...
for i in *
do
	#... se l'elemento è una directory traversabile...
	if test -d $i -a -x $i
        then
		#... invocazione ricorsiva del file con parametro in forma assoluta
		FCR.sh `pwd`/$i
		#Aggiornamento del contatore con il totale della sotto-gerarchia esplorata
		conta=$?
		export conta
	fi
done

#Al termine della ricorsione si ritorna come valore dello script il numero di file contati nella gerarchia
exit $conta