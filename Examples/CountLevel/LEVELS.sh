#!/bin/sh

#Controllo del numero di parametri
if test $# -ne 1
then
	echo ERRORE: numero parametri errato: richiesti 1, passati $#
	exit 1
fi

#Controllo sul primo parametro (nome assoluto)
case $1 in
/*)
	echo "DEBUG-OK nome assoluto"
	if test ! -d $1 -o ! -x $1
	then
		echo ERRORE: $1 directory inesistente o non traversabile
		exit 2;
	fi;;
*)
	echo ERRORE: $1 non è nome assoluto
	exit 3;;
esac

#Controlli superati - path setting
PATH=`pwd`:$PATH
export PATH

#Inizializzazione del contatore
count=0

#Creazione del file temporaneo per salvare il numero livelli
echo $count > /tmp/levelCount

#Invocazione ricorsiva
FCR.sh $1 $count /tmp/levelCount

#Stampa del risultato
echo -----------------------------------------
echo La gerarchia possiede `cat /tmp/levelCount` livelli
echo -----------------------------------------
echo

#Rimozione del file temporaneo
rm /tmp/levelCount