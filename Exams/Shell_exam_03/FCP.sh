#!/bin/sh

#Controllo sul numero di parametri
if test $# -ne 2
then
	echo "ERRORE: parametri passati: $#, richiesti: 2"
	exit 1;
fi

#Controllo sul primo parametro
case $1 in
/*)
	echo "OK nome assoluto: $1"
	if test ! -d $1 -o ! -x $1
	then
		echo "ERRRORE: non directory o directorynon traversabile"
		exit 2;
	fi
;;
*)
	echo "ERRORE: $1 non nome assoluto"
	exit 3
;;
esac

#Controllo sul secondo parametro
case $2 in
*/*)
	echo "ERRORE: $2 non nome relativo semplice"
	exit 4
;;
*)
	echo "OK: $2 nome relativo semplice"
;;
esac

#Path setting
PATH=`pwd`:$PATH
export PATH

#Creazione di un file temporaneo per salvare i percorsi delle directory idonee
> /tmp/dir$$

#Invocazione del file comandi ricorsivo
FCR.sh $1 $2 /tmp/dir$$

#Stampa del contenuto del file temporaneo (nomi assoluti directory)
echo
echo ---------------------------------------------------
echo Elenco directory idonee
echo ---------------------------------------------------
cat /tmp/dir$$

#Rimozione file temporaneo
rm /tmp/dir$$