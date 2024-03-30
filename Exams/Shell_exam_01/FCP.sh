#!/bin/sh

#Controllo il numero di parametri
if test $# -lt 3
then
	echo Numero parametri insufficienti: 2 richiesti
	exit 1
fi

#Controllo il primo parametro
echo DEBUG-Controllo primo parametro:
case $1 in
/*)
	echo ERRORE: $1 nome assoluto
	exit 2 ;;
*/*)
	echo ERRORE: $1 nome relativo NON semplice
	exit 3 ;;
*)
	echo OK: $1 nome RELATIVO SEMPLICE ;;
esac

#Salvo il primo parametro
primoPar=$1
#Eseguo lo shift per controlli sui parametri successivi
shift

#Controllo sui parametri successivi
echo DEBUG-Controllo parametri successivi:
for par in $*
do
	case $par in
	/*)
		echo OK: $par nome assoluto
		#Controllo se è directory traversabile
		if test ! -d $par -o ! -x $par
		then
			echo ERRORE: $par non directory o directroy non traversabile
			exit 4
		fi
	;;
	*)
		echo ERRORE: $par NON nome assoluto
		exit 5 ;;
	esac
done

#In questo punto tutti i controlli sui parametri sono superati
PATH=`pwd`:$PATH
export PATH

#Creazione del file temporaneo
> /tmp/conta$$

#Invocazione del file ricorsivo per ogni gerarchia passata
for i in $*
do
	echo Eseguo la fase su $i
	FCR.sh $primoPar $i /tmp/conta$$
done

#Terminate le N fasi si riposrta il numero totale di file che soddisfano la specifica
echo ---------------------------------------------------------------------
echo Il numero di file che soddisfano la specifica: `wc -l < /tmp/conta$$`
echo ---------------------------------------------------------------------

#Stampa dei vari file che soddisfano la specifica
echo ---------------------------------------------------------------------
echo Stampa del contenuto dei vari file
echo ---------------------------------------------------------------------

for line in `cat /tmp/conta$$`
do
	echo
	echo Stampo il contenuto di: $line
	cat $line
done

#Rimozione del file temporaneo
rm /tmp/conta$$