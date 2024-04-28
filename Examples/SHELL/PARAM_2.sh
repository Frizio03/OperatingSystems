#!/bin/sh

#Inizializzazione di un contatore
count=0

#Variabile per contenere i primi N parametri
paramList=

for par in $*
do
	#Aggiornamento del contatore
	count=`expr $count + 1`

	#Controllo se NON sono all'ultimo parametro
	if test $count -ne $#
	then
		#Sono fra i primi N parametri
		echo Parametro generico $par

		#Effettuo eventuali controlli sul parametro

		#Aggiungo il parametro alla lista
		paramList="$paramList $par"
	else
		#Sono giunto all'ultimo parametro
		echo Ultimo parametro $par

		#Effettuo eventuali controlli sull'ultimo parametro

		#Salvo l'ultimo parametro
		X=$par
	fi
done