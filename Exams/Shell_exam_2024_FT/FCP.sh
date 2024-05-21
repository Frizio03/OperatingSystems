#!/bin/sh

#Controllo del numero dei parametri
if test $# -lt 3
then
	echo ERRORE: inserire almeno 3 parametri, passati: $#
	exit 1
fi

#Controllo del primo parametro (nome relativo semplice)
case $1 in
*/*)
	echo ERRORE: $1 non nome relativo semplice
	exit 2;;
*)
	echo "DEBUG-Ok primo parametro";;
esac

#Salvo il primo parametro in una varaiabile
N=$1

#Eseguo uno shift sulla lista parametri per non considerare il primo
shift

#Controllo sui parametri succcessivi (scorro la lista dei parametri)
for par in $*
do
	case $par in
	/*)
		echo "DEBUG-ok nome assoluto"
		#Se il percorso non è una direcotory o non è traversabile
		if test ! -d $par -o ! -x $par
		then
			echo ERRORE: $par non è directory o directory non traversabile
			exit 3
		fi
	;;
	*)
		echo ERRORE: $par non nome assoluto
		exit 4
	;;
	esac
done

#Controlli superati - PATH setting
PATH=`pwd`:$PATH
export PATH

#Creazione di file temporanei per contenere i nomi assoluti
> /tmp/absoluteFiles

#Esecuzione delle Z fasi
for dirAss in $*
do
	echo GERARCHIA: $dirAss
	#Inizio dell'esplorazione ricorsiva di una gerachia passando come parametri la gerarchia e i nomi dei file temporanei
	FCR.sh $dirAss /tmp/absoluteFiles
done

#Conteggio dei file in 4 posizione
SOMMA=`wc -l < /tmp/absoluteFiles`

#Controllo se non ci sono file in 4 posizione
if test $SOMMA -eq 0
then
	#Stampa del messaggio di errore
	echo ERRORE: nessun file trovato in 4° posizione!

	#Rimozione de file temporanei e uscita
	rm /tmp/absoluteFiles
	exit 5
else
	#Stampa del numero di file idonei trovati
	echo OK numero di file in quarta posizione: $SOMMA
fi

#Creazione di una nuova directory nella directory corrente
mkdir $N

#Scorro i nomi (assoluti) di ogni file idoneo trovato
for file in `cat /tmp/absoluteFiles`
do
	#Creazione del link software nella directory con nome N per ogni file idoneo
	#Passando come
	ln -s $file $N/
done

#Messaggio di fine programma
echo FINE

#Rimozione del file temporaneo
rm /tmp/absoluteFiles