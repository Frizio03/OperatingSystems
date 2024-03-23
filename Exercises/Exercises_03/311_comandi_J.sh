#!/bin/sh

#Controllo numero parametri
if test $# -eq 0
then
	echo Bisogna passare almeno un parametro
	exit 1
fi

#21
first=$1							#Salvo il primo parametro
shift								#Elimino il primo parametro dall lista

echo Il primo parametro: $first			#Stampo il primo parametro
echo La lista restante: $*			#Stampo lista di parametri restanti

#22
count=0
NewList= 
for par in $*
do
	count=`expr $count + 1`
	if test $count -ne $#
	then
		NewList="$NewList $par"		#Aggiungo il par alla lista
	else
		LastPar=$par				#Ho trovato ultimo par (count = $#)
	fi
done

echo "L'ultimo parametro: $LastPar"
echo Parametri restanti: $NewList