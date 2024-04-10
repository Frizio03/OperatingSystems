#!/bin/sh

#Controllo del numero dei parametri
if test $# -lt 3
then
	echo ERRORE: inserire almeno 3 parametri
	exit 1
fi

#Controllo se il primo parametro è numerico positivo
case $1 in
*[!0-9]*)
	#Se il valore fa match con qualcsa che non è 0-9 allora non è numero
	echo ERRORE: $par non numerico
	exit 2
;;
*)
	#Ok il parametro è un numero
	echo "DEBUG-ok primo parametro"
	
	#Salvo il primo parametro nella variabile X
	X=$1
	#Eseguo lo shift della lista parametri
	shift
;;
esac

#Controllo sui parametri sucessivi (nomi assoluti)
for par in $*
do
	case $par in
	/*)
		echo "DEBUG-ok nome assoluto"
		#Se il percorso non è una direcotori o non è traversabile
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

#cìCreazione di un file temporaneo per salvare i nomi assoluti dei file
> /tmp/nomiAssoluti$$

#Inizializzazione variabile di lunghezza massima dei file idonei
L=0

#Esplorazione ricorsiva di ogni gerarchia
for ger in $*
do
	echo ">>> GERARCHIA: $ger"
	FCR.sh $ger $X $L /tmp/nomiAssoluti$$

	#Ricavo la lunghezza max fornita come valore di ritorno
	L=`echo $?`
done

#Ricavo il numero totale di file idonei trovati
NF=`wc -l < /tmp/nomiAssoluti$$`

#Stampa dei risultati di ricerca
echo
echo ----------------------------------------------------
echo Numero totale dei file trovati: $NF
echo ----------------------------------------------------
echo Lunghezza in linee massima: $L
echo ----------------------------------------------------

#Se sono stati trovati almeno due file
if test $NF -ge 2
then
	#Stampo il nome di ciascun file
	echo
	echo Elenco dei file idonei
	for line in `cat /tmp/nomiAssoluti$$`
	do
		echo "--> $line"
	done
	echo
fi

#Rimozione file temporaneo
rm /tmp/nomiAssoluti$$