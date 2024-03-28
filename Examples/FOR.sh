#!/bin/sh

echo ----------------------------------------------------------------------
echo Stampo i parametri passati a questo file comandi
echo ----------------------------------------------------------------------

#Ciclo for per iterare sui parametri passati
count=0
for par in $*
do
	count=`expr $count + 1`
	echo Stampo il parametro $count: $par
done
echo
#Ciclo for per iterare sui parametri passati
count=0
for par
do
	count=`expr $count + 1`
	echo Stampo il parametro $count: $par
done

echo ----------------------------------------------------------------------
echo Ciclo for su lista vuota: non stampa nulla
echo ----------------------------------------------------------------------

#ATTENZIONE: ciclo for su lista vuota
for i in
do
	echo Stampo $i #ATTENZIONE: nessuna stampa!!
done

echo ----------------------------------------------------------------------
echo Stampo i nomi di tutti gli elementi di questa directory non nascosti
echo ----------------------------------------------------------------------

#Ciclo for su tutti gli elementi (*) della directory
for e in *
do
	count=`expr $count + 1`
	echo Stampo elemento $count: $e
done

echo ----------------------------------------------------------------------
echo Stampo i nomi di tutti gli elementi nascosti di questa directory
echo ----------------------------------------------------------------------

#Ciclo for su elementi nascosti (iniziano per .) della directory
for e in .*
do
	count=`expr $count + 1`
	echo Stampo elemento $count: $e
done