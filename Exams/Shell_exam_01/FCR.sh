#!/bin/sh

#Ci si posiziona all'inizio della gerarchia
cd $2

#Se trovo un file con nome specificato e leggibile
if test -f $1 -a -r $1
then
	#Eseguo il sort senza contare maiuscole e minuscole e salvo in ridirezione
	sort -f $1 > sorted

	#Salvo il nome del file sorted in append
	echo `pwd`/sorted >> $3
fi

#Se non supero il controllo procedo ricorsivamente nelle sottodirectory
for e in *
do
	#Per ogni sottodirectory traversabile
	if test -d $e -a -x $e
	then
		#Chiamata ricorsiva
		FCR.sh $1 `pwd`/$e $3
	fi
done