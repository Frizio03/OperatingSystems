#!/bin/sh

#14-15
#Controllo sui parametri
if test $# -lt 1
then
	echo Numero di parametri insufficienti: inserire almeno 1 parametro
	exit 1
fi

#Stampo l'elenco dei parametri
echo I parametri passati sono: $*

#Stampo un parametro per volta
count=1
for param in $*
do
	echo Stampo il parametro \$$count: $param
	count=`expr $count + 1`
done

echo DEBUG: fine dello script