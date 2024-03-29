#!/bin/sh

#Controllo il numero di parametri
if test $# -ne 2
then
	echo "ERRORE: numero di parametri non corretto (inseriti $#, richiesti 2)"
	exit 1;
fi

#Controllo sul primo parametro
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    	echo "ERRORE: $1 non directory o non attraversabile"
    	exit 2;
    else
    	echo "DEBUG-Primo parametro ok";
    fi;;
*)
	echo "ERRORE: $1 non nome assoluto"
	exit 3;;
esac

#Controllo sul secondo parametro (intero positivo)
case $2 in
*[!0-9]*)
	echo "ERRORE: $2 non numerico o non positivo"; exit 4;;
*)
	if test $2 -eq 0
  then
  	echo "ERRORE: $2 nullo, deve essere positivo"; exit 5;
  else
  	echo "DEBUG-Secondo parametro ok";
  fi;;
esac

#Path setting
PATH=`pwd`:$PATH
export PATH

#Creo un file temporaneo per salvare nomi assoluti directory e file idonei
> /tmp/dirs$$
> /tmp/files$$

#Invocazione file comandi ricorsivo
FCR.sh $1 $2 /tmp/dirs$$ /tmp/files$$

#---------------------------------------------------------

#Stampo l'elenco delle directory idonee
echo -----------------------------------------------------
echo ELENCO DIRECTORY IDONEE
echo -----------------------------------------------------
cat /tmp/dirs$$

#Stampo i nomi (assoluti dei file idonei)
echo
echo -----------------------------------------------------
echo "ELENCO NOMI (assoluti) FILE IDONEI"
echo -----------------------------------------------------
cat /tmp/files$$

#Rimozione dei file temporanei
rm /tmp/dirs$$
rm /tmp/files$$