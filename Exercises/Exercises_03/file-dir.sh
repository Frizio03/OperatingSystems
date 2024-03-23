#!/bin/sh

cd $1								#Mi spospo nella directory passata

#Inizializzo i contatori
countF=0
countD=0

for e in *							#Scorro ciascun elemento della directory
do
	if test -f $e
	then
		echo "[F] `pwd`/$e"			#Stampo il nome assoluto del file
		countF=`expr $countF + 1`	#Incremento contatore file
	else
		echo "[D] `pwd`/$e"			#Stampo il nome assoluto della directory
		countD=`expr $countD + 1`	#Incremento contatore directory
	fi
done

#Stampa dei contatori
echo ---------------------------------------------
echo I file stampati sono: $countF
echo Le directory stampate sono: $countD
echo ---------------------------------------------