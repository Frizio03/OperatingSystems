#!/bin/sh

#Controllo del numero di rarametri
if test $# -lt 2
then
	echo ERRORE: passare almeno 2 parametri
	exit 1
fi

#Controllo dei parametri successivi
N=`expr $# - 1`
count=1

#Controllo sulla tipologia dei parametri
for par in $*
do
	#Controllo se sono arrivato all'ultimo parametro
	if test $# -ne $count
	then
		#Controllo se N-esimo parametro è percorso assoluto
		case $par in
		/*)
			echo "DEBUG-ok nome assoluto"
			if test ! -d $par -o ! -x $par
			then
				echo ERRORE: $par directory inesistente o non traversabile
				exit 4
			fi;;
		*)
			echo ERRORE: $par non è nome assoluto
			exit 2;;
		esac
	else
		#Controllo se l'ultimo parametro è un numero
		case $par in
		*[!0-9]*)
			echo ERRORE: $par non è numerico positivo
			exit 3;;
		*)
			echo "DEBUG-ok numero X"
			#Salvo il valore di X
			X=$par;;
		esac
	fi

	#Incremento il contatore
	count=`expr $count + 1`
done

#Controlli superati - path setting
PATH=`pwd`:$PATH
export PATH

#Creazione file temporaneo
> /tmp/files$$
> /tmp/dir$$

#Esplorazione ricorsiva di ogni gerarchia

#Per ciascuna gerarchia passata
count=1
for ger in $*
do
	if test $count -eq $#
	then
		break;
	fi

	FCR.sh $ger $X /tmp/files$$ /tmp/dir$$

	#Aggiorno in contatore
	count=`expr $count + 1`
done

#Stampo il numero di directory idonee
echo
echo --------------------------------------------------------
echo Directory idonee trovate: `wc -l < /tmp/dir$$`
echo --------------------------------------------------------

#Stampa delle directory idonee e dei file
for dir in `cat /tmp/dir$$`
do
	echo 
	echo "-------- DIRECTORY $dir"
	for file in `grep $dir /tmp/files$$`
	do
		echo 
		echo "> FILE: $file"
		echo `tail -$X $file`
	done
done

#Rimozione file temporanei
rm /tmp/files$$
rm /tmp/dir$$