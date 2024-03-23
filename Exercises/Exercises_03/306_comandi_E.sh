#!/bin/sh

#12
#Controllo sul numero di parametri con il case
case $# in
0)
	echo Parametri insufficienti
	exit 1
;;
1)
	echo Inserisci una stringa da scrivere:
	read  nomeFile
	cat $nomeFile >> $1
;;
2)
	echo Inserisci una stringa da scrivere:
	read  nomeFile
	cat $nomeFile >> $2
;;
*)
	echo Troppi parametri
	exit 2
;;
esac