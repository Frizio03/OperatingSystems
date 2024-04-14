#!/bin/sh

#Controllo il numero di parametri
if test $# -lt 3
then
	echo ERRORE: inserire almeno 3 parametri
	exit 1
fi

#Controllo sul primo parametro (numero positivo)
case $1 in
*[!0-9]*)
	echo ERRORE: $1 non numero positivo
	exit 2 ;;
*)
	echo echo "DEBUG-Ok primo parametro" ;;
esac

#Controllo superato: salvo il primo parametro
X=$1
#Eseguo lo shift della lista parametri per non considerare $1
shift

#Controllo i successivi Q parametri
for par in $*
do
	case $par in
	/*)
		#Controllo se è una directory traversabile
		if test ! -d $par -o ! -x $par
		then
			echo ERRORE: $par non directory o directory non traversabile
			exit 3
		fi;;
	*)
		echo ERRORE: $par non nome assoluto
		exit 4;;
	esac
done

#Controlli sui parametri superati: PATH setting
PATH=`pwd`:$PATH
export PATH

#Creazione di un file tmporaneo per salvare i nomi file idonei
> /tmp/nomiAssoluti

#Esplorazione delle Q gerarchie
for G in $*
do
	#Inizializzazione di una variabile di conteggio
	conta=0

	#Esplorazione ricorsiva della gerarchia
	echo
	echo Gerarchia $G
	FCR.sh $G $X $conta /tmp/nomiAssoluti

	#Aggiornamento e stampa del conteggio
	conta=$?

	#Stampa del numero di file trovati
	echo File trovati: $conta
done

#Controllo sui file della prima gerarchia G1
G1=$1

#Conto i file della prima gerarchia
NF1=`grep $G1 < /tmp/nomiAssoluti | wc -l`

#Per ogni file di G1 (con percorso assoluto che inizia con G1)
for file1 in `grep $G1 < /tmp/nomiAssoluti`
do
	#Ricavo i nomi assoluti dei file restanti
	for file2 in `grep -v $G1 < /tmp/nomiAssoluti`
	do
		#Controllo se ci sono differenze fra i file
		#Ridireziono stdout e stderr su dev/null per non avere stampe a terminale
		diff $file1 $file2 > /dev/null 2> /dev/null

		#Salvo il risultato del controllo in una variabile
		res=$?

		echo
		if test $res -eq 0
		then
			echo "-- UGUALI: $file1 e $file2"
		else
			echo "-- DIVERSI: $file1 e $file2"
		fi
	done
done

#Rimozione del file temporaneo
rm /tmp/nomiAssoluti