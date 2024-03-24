#!/bin/sh

#26

#Ricerca in breadth-first

#Spostamentoi nella directory in cui eseguire la ricerca
cd $1

#Verifico se esiste una directory con nome fornito
if test -d $2
then
echo La directory $2 si trova in `pwd`	#Directory trovata
fi

#Passiamo ora alla parte ricorsiva
for i in *
do
if test -d $i -a -x $i 
then
	#Stampa di debugging
	#echo DEBUG-Stiamo per esplorare la directory `pwd`/$i
	#Invocazione ricorsiva: ATTENZIONE passo il nome assoluti di dir da esplorare!
	CercaDir.sh `pwd`/$i $2
fi
done
