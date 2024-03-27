#!/bin/sh

#Mi sposto nella directory di inizio della gerarchia
cd $1

#Scorro tutti gli elementi della directory
for f in *
do
	if test -f $f
	then
		#Se l'elemento è un file aggiungo una linea al file conta$$
		echo `pwd`/$f >> $2
	fi
done

#Parte ricorsiva - per ogni directory invoco nuovamente il file
for d in *
do
	#Se posso traversare la directory eseguo nuovamente il file ricorsivo
	if test -d $d -a -x $d
	then
		FCR.sh `pwd`/$d $2
	fi
done