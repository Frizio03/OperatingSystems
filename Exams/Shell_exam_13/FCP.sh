#!/bin/sh

#Controllo il numero di parametri
if test $# -lt 3
then
	echo ERRORE: inserire almeno 3 parametri
	exit 1
fi

#Controllo sul primo parametro
case $1 in
*[!0-9]*)
	echo ERRORE: $1 non numero positivo
	exit 2 ;;
*)
	if test $1 -eq 0
	then
		echo ERRORE: primo parametro uguale a 0
		exit 3
	fi
	echo echo "DEBUG-Ok primo parametro" ;;
esac

#Controllo sul secondo parametro
case $2 in
*/*)
	#Se contiene / non è un nome assoluto o relativo
	echo ERRORE: $2 non stringa
	exit 4 ;;
*)
	echo echo "DEBUG-Ok secondo parametro" ;;
esac

#Salvataggio dei primi due parametri in variabili
W=$1
S=$2

#Esgeuo 2 shift sulla lista parametri per non considerare i primi 2
shift; shift;

#Controllo sui successivi Q parametri
for par in $*
do
	case $par in
	/*)
		#Controllo sei il nome assoluto si riferisce ad una directory traversabile
		if test ! -d $par -o ! -x $par
		then
			echo ERRORE: $par non directory o directory non traversabile
			exit 5
		fi
	;;
	*)
		echo ERRORE: $par non nome assoluto
		exit 6 ;;
	esac
done

#Controlli sui parametri superati: PATH setting
PATH=`pwd`:$PATH
export PATH

#Creazione di un file temporaneo per contare i file idonei
> /tmp/nomiAssoluti

#Esecuzione delle Q fasi del programma principale
for G in $*
do
	#Invocazione del file comandi ricosivo sulla gerarchia G
	echo GERARCHIA: $G
	FCR.sh $G $S /tmp/nomiAssoluti
done

#Ricavo il numero di directory trovate
ND=`wc -l < /tmp/nomiAssoluti`

#Stampa del numero totale di directory idonee
echo -----------------------------------------------------
echo Numero directory idonee: $ND
echo -----------------------------------------------------

#Se il numero è maggiore di W
if test $ND -ge $W
then
	echo -n "Mario, inserireun nuemero compreso fra 1 e $W: "
	read X

	case $X in
	*[!0-9]*)
		echo ERRORE: $X non numero positivo
		exit 7 ;;
	*)
		#Controllo che X sia compreso fra 1 e W
		if test $X -lt 1 -o $X -gt $W
		then
			echo ERRORE: $X non compreso fra 1 e $W
			exit 8
		fi
	;;
	esac

	#Stampo la X-esima directory dell'elenco
	echo
	echo "La directory in pozionie $X dell'elenco:"
	head -$X < /tmp/nomiAssoluti | tail -1
else
	echo $ND non maggiore o uguale a $W. FINE!
fi

#Rimozione del file temporaneo
rm /tmp/nomiAssoluti