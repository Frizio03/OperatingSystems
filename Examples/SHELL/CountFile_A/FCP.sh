#!/bin/sh

#Esempio del conteggio dei file in una gerarchia
#Metodo A: utilizzo di un file temporaneo per salvare i percorsi

#Controllo sul numero dei parametri
case $# in
1)
	echo DEBUG-Parametri OK ;;
*)
	echo ERRORE: $# passed, 1 requested
	exit 1 ;;
esac

#Controllo che il parametro sia percorso assoluto
case $1 in
/*)
	if test ! -d $1 -o ! -x $1
	then
		echo ERRORE: il percorso $1 non è directory oppure non è traversabile
		exit 2;
	fi
;;
*)
	echo ERRORE: $1 NON è in forma assoluta
	exit 3;
;;
esac

#Superati i controlli procedo ad andare avanti
echo DEBUG-Directory traversabile OK

#Settaggio della PATH
PATH=`pwd`:$PATH
export PATH

#Creazione/azzeramento di un file temporaneo con $$ per fornire sempre un nome diverso (identificativo del processo)
> /tmp/conta$$

#Invocazione del file comandi ricorsivo
FCR.sh $1 /tmp/conta$$

#Contando le linee del file conta$$ si ricava il numero dei file
echo
echo Il numero dei file trovati: `wc -l < /tmp/conta$$`
echo

#Recupero i percorsi assoluti dei vari file
for percorso in `cat /tmp/conta$$`
do
	echo Il nome assoluto del file: $percorso
	echo
	#Se il file è leggibile ne mostro il contenuto
	if test -r $percorso
	then
		echo Mostro il contenuto del file:
		cat $percorso
	else
		echo Non posso leggere il file
	fi
	echo
done

#Rimuovo il file temporaneo
rm /tmp/conta$$