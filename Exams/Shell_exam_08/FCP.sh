#!/bin/sh

#Controllo sul numero di parametri
if test $# -lt 2
then
	echo ERRORE: passare almeno 2 parametri
	exit 1
fi

#Scorro ciascun parametro
for par in $*
do
	#Controllo sul nome (assoluto)
	case $par in
	/*)
		echo "DEBUG-Ok nome asosluto"
		if test ! -d $par -o ! -x $par
		then
			echo ERRORE: $par directory inesistente o non traversabile
			exit 2
		fi
		;;
	*)
		echo ERRORE: $par non è nome assoluto
		exit 3;;
	esac
done

#Controlli superati - path setting
PATH=`pwd`:$PATH
export path

#File temporaneo per salvare i nomi dei file
> /tmp/files$$

#Inizializzazione contatrore gerarchie
count=1

#Esplorazione ricorsiva di ogni gerarchia
for i in $*
do
	#Creazione di un file temporaneo per salvare il numero file della gerarchia corrrente
	> /tmp/conta$$-$count

	echo ESPLORAZIONE DI $i
	FCR.sh $i /tmp/conta$$-$count /tmp/files$$

	#Aggiorno il contatore
	count=`expr $count + 1`
done

#Stampa delle gerarchie e del numero di file idonei per ciascuna di esse
echo
echo -------------------------------------------------------------
echo Risultati esplorazione
echo -------------------------------------------------------------
echo

count=1
for ger in $*
do
	echo GERARCHIA: $ger
	echo File idonei trovati: `cat /tmp/conta$$-$count`
	echo

	#Aggiorno il contatore
	count=`expr $count + 1`
done

#Stampa del contenuto dei file con linea X

#Per ogni file idoneo
for f in `cat /tmp/files$$`
do
	#Richiesta e controllo dell'input
	X=0
	until test $X -gt 0
	do
		echo
		echo Inserire il numero di linee da visualizzare: 
		read X
		#Contollo che l'input sia numerico
		case $X in
		*[!0-9]*)
			echo ERRORE: inserire un valore numerico
			x=0 ;;
		esac
	done

	#Stampa delle prime X linee
	echo "---> FILE: $f"
	echo `head -$X $f`
done


#Rimozione file temporanei
rm /tmp/conta*
rm /tmp/files$$