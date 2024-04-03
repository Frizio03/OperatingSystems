#!/bin/sh

#Controllo del numero di parametri passati
if test $# -ne 2
then
	echo ERRORE: parametri passati: $#, richiesti: 2
	exit 1;
fi

#Controllo sul primo parametro (percorso assoluto)
case $1 in 
/*)
	echo "DEBUG-ok nome assoluto";;
*)
	echo ERRORE: $1 non è nome assoluto
	exit 2;;
esac

#Controllo sul secondo parametro (numero intero positivo)
case $2 in
*[!0-9]*)
	echo ERRORE: $2 non è numero intero positivo
	exit 3;;
*)
	echo "DEBUG-ok numero intero positivo";;
esac

#Controlli superati - Path setting
PATH=`pwd`:$PATH
export PATH

#Creazione dei file temporanei
> /tmp/dir$$
> /tmp/files$$

#Avvio della ricerca ricorsiva
FCR.sh $1 $2 /tmp/dir$$ /tmp/files$$

#Stampa delle directory che contengono file idonei
echo
echo -----------------------------------------------------------
echo "Nomi (assoluti) delle directory che contengno file idonei"
echo -----------------------------------------------------------

for line in `cat /tmp/dir$$`
do
	echo $line
done

#Considero ogni file idoneo
for f in `cat /tmp/files$$`
do
	#Chiedo all'utente un numero di linea
	X=0
	until test $X -gt 0 -a $X -le $2
	do
		echo
		echo Inserire un valore compreso fra 1 e $2
		read X
	done

	#Stampa della riga X del file e del suo nome assoluto
	echo
	echo "---> File $f"
	echo --- `head -$X $f | tail -1`
done

#Eliminazione file temporanei
rm /tmp/dir$$
rm /tmp/files$$