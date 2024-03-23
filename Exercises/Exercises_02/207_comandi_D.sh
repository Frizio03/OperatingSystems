#!/bin/sh

#47-50

#Memorizzo in x il numero di linee di provaSort.txt
x=`wc -l < proveSort`

#Stampa del valore di x
echo stampo il valore di x\: $x

#Memorizzo in x il numero di linee di provaSort.txt
y=`wc -l < proveSort`

#Stampa del valore di x
echo stampo il valore di y\: $y

#Sommo in z il valori di x e y
z=`expr $x + $y`

#Stampa del valore di z
echo stampo il valore di z somma\: $z

#Moltiplico in z il valori di x e y
z=`expr $x \* $y`

#Stampa del valore di z
echo stampo il valore di z prodotto\: $z

#Tengo il resto di x diviso y
z=`expr $x % $y`

#Stampa del valore di z
echo stampo il valore di z prodotto\: $z