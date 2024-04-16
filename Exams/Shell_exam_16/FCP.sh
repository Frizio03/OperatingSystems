#!/bin/sh

#Controllo del numero dei parametri
if test $# -lt 4
then
	echo ERRORE: inserire almeno 4 parametri
	exit 1
fi

#Controllo del primo parametro
case $1 in
*/*)
	#Se il nome fa match con / è un percorso
	echo ERRORE: $1 non stringa semplice ma percorso
	exit 2;;
*)
	echo "DEBUG-Ok primo parametro"
	#Salvo il primo parametro in una variabile
	S1=$1
esac

#Controllo del secondo parametro
case $2 in
*/*)
	#Se il nome fa match con / è un percorso
	echo ERRORE: $2 non stringa semplice ma percorso
	exit 2;;
*)
	echo "DEBUG-Ok secondo parametro"
	#Salvo il secondo parametro in una variabile
	S2=$2
esac

#Se ho superato i controlli sui primi parametri (e li ho salvati)
#eseguo due shift della lista parametri per non considerarli
shift; shift;

#Controllo sui successivi Q parametri (percorsi assoluti)
for par in $*
do
	#Controllo se il parametro è un percorso assoluto
	case $par in
	/*)
		#Controllo se il percorso si riferisce ad una directory traversabile
		if test ! -d $par -o ! -x $par
		then
			echo ERRORE: $par directory inesistente o non traversabile
			exit 3
		fi;;
	*)
		echo ERRORE: $par non percorso assoluto
		exit 4;;
	esac
done

#Controlli superati - PATH setting
PATH=`pwd`:$PATH
export PATH

#Creazione di file temporanei per salvare nomi assoluti di file idonei
> /tmp/nomiAssoluti$$-$S1
> /tmp/nomiAssoluti$$-$S2

#Esecuzione delle Q fasi di ricerca del programma
for G in $*
do
	echo GERARCHIA: $G
	FCR.sh $S1 $S2 /tmp/nomiAssoluti$$-$S1 /tmp/nomiAssoluti$$-$S2 $G
done

#Ricavo i conteggi globali e li salvo in variabili
TOT1=`wc -l < /tmp/nomiAssoluti$$-$S1`
TOT2=`wc -l < /tmp/nomiAssoluti$$-$S2`

#Stampa dei conteggi totali dei file
echo
echo ----------------------------------------------------------
echo File con estensione $S1: $TOT1
echo File con estensione $S2: $TOT2
echo ----------------------------------------------------------

if test $TOT1 -le $TOT2
then
	X=0
	while test $X -eq 0
	do
		#Chiedo all'utente il valore
		echo
		echo -n "Fabrizio, inserisci un valore X compreso fra 1 e $TOT1: "
		read X
		
		#Controllo che X sia compreso fra 1 e TOT1
		case $X in
		*[!0-9]*)
			X=0
			echo ERRORE: inserito valore non numerico!;;
		*)
			#Controllo che il numero sia compreso fra 1 e TOT1
			if test $X -lt 1 -o $X -gt $TOT1
			then
				echo ERRORE: valore numerico inserito non valido!
				X=0
			fi
		;;
		esac
	done

	#Ottenuto un valore valido
	
	#Stampo nome assoluto del X-esimo file con S1
	echo
	echo "Nome assoluto del $X-esimo file con estensione $S1:"
	head -$X < /tmp/nomiAssoluti$$-$S1 | tail -1

	#Stampo nome assoluto del X-esimo file con S2
	echo
	echo "Nome assoluto del $X-esimo file con estensione $S2:"
	head -$X < /tmp/nomiAssoluti$$-$S2 | tail -1
fi

#Rimozione dei file temporanei
rm /tmp/nomiAssoluti$$-$S1
rm /tmp/nomiAssoluti$$-$S2