#!/bin/sh

#Controllo il numero di parametri
if test $# -lt 3
then
	echo ERRORE: inserire almeno 3 parametri
	exit 1
fi

#Controllo sul primo parametro
case $1 in
?)
	echo echo "DEBUG-Ok primo parametro" ;;
*)
	echo ERRORE: $1 non singolo crattere
	exit 2 ;;
esac

#Controllo superato: salvo il primo parametro
C=$1
#Eseguo lo shift della lista parametri per non considerare $1
shift

#Controllo degli N parametri seguenti
for par in $*
do
	case $par in
	/*)
		#Controllo sei il nome assoluto si riferisce ad una directory traversabile
		if test ! -d $par -o ! -x $par
		then
			echo ERRORE: $par non directory o directory non traversabile
			exit 3
		fi
	;;
	*)
		echo ERRORE: $par non nome assoluto
		exit 4 ;;
	esac
done

#Controlli superati: PATH setting
PATH=`pwd`:$PATH
export PATH

#Creazione di un file temporaneo per salvare le directory idonee
> /tmp/conta$$

#Svolgimento delle N fasi per ogni gerarchia
for ger in $*
do
	echo GERARCHIA: $ger
	FCR.sh $ger $C /tmp/conta$$
done

#Stampo il numero totale delle directory idonee
echo ---------------------------------------------------------------
echo Numero di directory idonee: `wc -l < /tmp/conta$$`
echo ---------------------------------------------------------------

#Stampa dei percorsi assoluti
for line in `cat /tmp/conta$$`
do
	echo
	echo ">>>> Directory $line"
	
	#Chiedo all'utente se vuole visualizzare il contenuto
	echo
	echo "Elena, vuoi visulizzare il contenuto della directory (y/n)? "
	#Leggo il comando inserito dall'utente
	read option

	#Se l'utente vuole visualizzare il contenuto
	case $option in
	s*|S*|y*|Y*)
		#Stampo gli elementi della direcotry (anche nascosti)
		ls -a $line ;;
	*)
		#Altrimenti non faccio nulla
		;;
	esac
done

#Eliminazione file temporaneo
rm /tmp/conta$$