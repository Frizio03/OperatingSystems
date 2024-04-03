#!/bin/sh

#Controllo sul numero di parametri
if test $# -lt 3
then
	echo "ERRORE: parametri passati: $#, richiesti: >= 3"
	exit 1;
fi

#Salvo il primo parametro ed eseguo lo shift sulla lista
X=$1
shift

#Controllo sul primo parametro
case $X in
*[!0-9]*)
	echo ERRORE: il parametro deve essere intero positivo
	exit 2;;
*)
	echo DEBUG-OK primo parametro;;
esac


#Controllo sugli N parametri successivi
for par in $*
do
	case $par in
	/*)
		echo DEBUG-OK parametro assoluto $par ;;
	*)
		echo ERRORE: $par non nome assoluto
		exit 3;;
	esac
done

#Path setting
PATH=`pwd`:$PATH
export PATH

#Creazione file temporaneo
> /tmp/files$$

#Esplorazione di ciascuna gerarchia
for ger in $*
do
	FCR.sh $X $ger /tmp/files$$
done

#Stampa del numero di file idonei
echo
echo -----------------------------------------------
echo Numero di file idonei trovati: `wc -l < /tmp/files$$`
echo -----------------------------------------------

#Stampa della k-esima linea
echo
echo -----------------------------------------------
echo Stampa della linea richiesta per ciascun file
echo -----------------------------------------------

for f in `cat /tmp/files$$`
do
	#Inizializzazione variabile
	K=0

	#Richiesta di intero positivo all'utente
	until test $K -gt 0 -a $K -le $X
	do
		echo "Inserire un numero intero positivo e <= $X: "
		read K
	done

	#Visualizzazione della k-esima riga
	echo 
	echo ----- Visualizzazione della riga $K del file $f
	echo `head -$k < $f | tail -1`
	echo

done

#Rimozione file temporano
rm /tmp/files$$