#!/bin/sh

#Controllo il numero di parametri
if test $# -lt 3
then
	echo ERRORE: passare almeno 3 parametri
	exit 1
fi

#Salvo il primo parametro ed eseguo uno shift della lista
Y=$1
shift

#Controllo sul primo parametro
case $Y in
*[!0-9]*)
	echo ERRORE: $Y non numerico positivo
	exit 2;;
*)
	echo "DEBUG-ok parametro numerico";;
esac

#Controllo sui parametri successivi (nomi assoluti)
for par in $*
do
	case $par in
	/*)
		echo "DEBUG-ok nome assoluto"
		if test ! -d $par -o ! -x $par
		then
			echo ERRORE: $par directory inesistente o non traversabile
			exit 3
		fi;;
	*)
		echo ERRORE: $par non è nome assoluto
		exit 4;;
	esac
done

#Controlli superati - path setting
PATH=`pwd`:$PATH
export PATH

#Creazione file temporanei
> /tmp/conta$$
> /tmp/files$$

#Esplorazione ricorsiva di ogni gerarchia
for ger in $*
do
	echo ESPLORAZIONE DI $ger
	FCR.sh $ger $Y /tmp/conta$$ /tmp/files$$
done

#Stampa dei risultati
echo
echo -------------------------------------------------
echo Numero di file creati: `cat /tmp/conta$$`
echo -------------------------------------------------

for f in `cat /tmp/files$$`
do
	echo
	echo "----------- FILE: $f"
	echo `cat $f`
done

#Rimozione file temporanei
rm /tmp/conta$$
rm /tmp/files$$