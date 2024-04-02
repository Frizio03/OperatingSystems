#!/bin/sh

#Controllo stretto del numero di parametri
if $# -ne 4
then
	echo "ERRORE: numero parametri errato: passati $#, richiesti 4"
	exit 1;
fi

#Controllo sul primo parametro
case $1 in
/*)
	echo "OK: nome assoluto $1"
;;
*)
	echo "ERRORE: $1 non è nome assoluto"
	exit 2
;;
esac

#Controllo sul secondo parametro (intero positivo)
case $2 in
*[!0-9]*)
	echo "ERRORE: $2 non numerico o non positivo"
	exit 3;;
*)
	if test $2 -eq 0
  then
  	echo "ERRORE: $2 nullo, deve essere positivo"
  	exit 4;
  else
  	echo "DEBUG-Secondo parametro ok";
  fi
;;
esac

#Controllo sul secondo parametro (intero positivo)
case $3 in
*[!0-9]*)
	echo "ERRORE: $3 non numerico o non positivo"
	exit 3;;
*)
	if test $3 -eq 0
  then
  	echo "ERRORE: $3 nullo, deve essere positivo"
  	exit 4;
  else
  	echo "DEBUG-Secondo parametro ok";
  fi
;;
esac

#Controllo sul terzo parametro (singolo carattere)
case $4 in
[a-z])
	echo "DEBUG-Secondo parametro ok";;
[A-Z])
	echo "DEBUG-Secondo parametro ok";;
[0-9])
	echo "DEBUG-Secondo parametro ok";;
*)
	echo "ERRORE: $4 non singolo carattere"
	exit 5;;
esac

#Path setting
PATH=`pwd`:$PATH
export PATH

#Creazione file temporaneo per salvare percorsi directory
> /tmp/dir$$

#Invocazione del file comandi ricorsivo per esplorazione
FCR.sh $* /tmp/dir$$

#Stampa dell'elenco directory idonee
echo
echo -------------------------------------------------------
echo Elenco directory idonee - nomi assoluti -
echo -------------------------------------------------------
echo

for line in `cat /tmp/dir$$`
do
	echo $line
done

#Rimozione del file temporaneo
rm /tmp/dir$$