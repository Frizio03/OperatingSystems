#!/bin/sh

#Esempio del conteggio dei file in una gerarchia
#Metodo C: utilizzo di una variabile di shell locale e il valore di ritorno di un file comandi

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

#Definiamo una variabile per il conteggio
conta=0

#Scriviamo il valore di questa variabile in un file temporaneo
echo $conta > /tmp/contatmp

#Invocazione del file comandi ricorsivo
FCR.sh $*   #$* ===> $1

#Lettura dal file temporaneo il conteggio e inserimento nella variabile conta
read conta < /tmp/contatmp

echo Il numero di file totale della gerarchia = $conta 
#OSSERVAZIONE: Non si possiede nessuna informazione riguardo i nomi dei file trovati

#Rimozione del file temporaneo
rm /tmp/contatmp
