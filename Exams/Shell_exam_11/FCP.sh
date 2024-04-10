#!/bin/sh

#Controllo il numero dei parametri
if test $# -lt 3
then
	echo ERRORE: inserire almeno 3 parametri
	exit 1
fi

#Controllo se il primo parametro è nome relativo semplice
case $1 in
*/*)
	echo ERRORE: $1 non relativo semplice
	exit 2;;
*)
	echo "DEBUG-ok nome relativo semplice";;
esac

#Salvo il primo parametro ed eseguo uno shift della lista
F=$1
shift

#Controllo degli N parametri successivi
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

#Creazione di un file temporaneo vuoto per salvare i percorsi dei file idonei
> /tmp/contafile$$

#Eplorazione delle gerarchie in N fasi
for ger in $*
do
	echo GERARCHIA $ger
	FCR.sh $ger $F /tmp/conta$$
done

#Salvo il numero di linee del file temporaneo
NF=`wc -l < /tmp/conta$$`

#Stampo il numero di file trovati
echo
echo ---------------------------------------------------------
echo Sono stati trovati $NF file che soddisfano i requisiti
echo ---------------------------------------------------------

#Inizializzazione di un contatore per controllo su linea pari/dispari
c=0

#Scorro ogni linea del file temporaneo (ogni file trovato)
for line in `cat /tmp/conta$$`
do
	#Aggiornamento del contatore
	c=`expr $c + 1`
	
	#Se la linea è dispari --> nome file
	if test `expr $c % 2` -ne 0
	then
		#Salvo il nome del file
		fileName=$line

		#Stampo il nome assoluto del file
		echo 
		echo ">>>> FILE: $line"
	#Se la linea è pari --> numero linee
	else
		#Stampo le linee del file
		echo ">>>> Lines: $line"
		echo

		#Inizializzazione variabile
		X=0

		while test $X -eq 0
		do
			echo Inserire un numero compreso fra 1 e $line:
			read X

			case $X in
			*[!0-9]*)
				echo ERRORE: $X non numerico
				X=0
			;;
			*)
				#Se X è maggiore del numero di linee
				if test $X -gt $line
				then
					echo ERRORE: numero fuori range
					X=0
				fi
			;;
			esac
		done


		#Stampa delle prime X linee del file
		echo
		echo "---- Stampo le prime $X linee del file $filename"
		echo `head -$X $fileName`
	fi
done


#Rimozione del file temporaneo
rm /tmp/conta$$