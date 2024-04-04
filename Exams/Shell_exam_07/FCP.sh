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

#Inizializzazione varibili conteggio e fase
count=0; step=1

#Inserisco il valore 0 nel file temporaneo
echo $count > /tmp/levelCount

#FASE A: conteggio dei livelli della gerarchia utilizzando valore di ritorno di FCR.sh

#Invocazione ricorsiva
FCR.sh $1 $count $step /tmp/levelCount

#Lettura del numero di livelli
NL=`cat /tmp/levelCount`

echo "DEBUG - Fine fase A - livelli rilevati: $NL"

#Richiesta all'utente di un valore numerico
val=0
while test $val -eq 0
do
	echo
	echo Inserire un valore numerico PARI compreso fra 1 e $NL inclusi
	read val

	#Controllo se è intero positivo
	case $val in
	*[!0-9]*)
		echo "Inserire un intero positivo!"
		val=0;;
	*)
		#Controllo se il valore è minore di NL
		if test $val -gt $NL
		then
			echo "Inserire un valore <= $NL!"
			val=0
		fi

		#Controllo se il numero è pari
		if test `expr $val % 2` -ne 0
		then
			echo "Inserire un numero PARI!"
			val=0
		fi
	;;
	esac
done

#FASE B: Ricerca delle directory al livello scelto

#Inizializzazione varibili conteggio e fase
count=0; step=2

#Invocazione del file comandi ricorsivo
FCR.sh $1 $count $step $val

#La stampa avviene grazie al file comandi ricorsivo

#Rimozione file temporaneo
rm /tmp/levelCount