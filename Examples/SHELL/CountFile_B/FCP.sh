#!/bin/sh

#Esempio del conteggio dei file in una gerarchia
#Metodo B: utilizzo di una variabile di ambiente e del valore di ritorno di un file comandi

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

#Inizializzazione variabile per il conteggio ed export
conta=0
export conta
#Ora la variabile conta e' una variabile copiata in ogni sottoshell

#Invocazione del file comandi ricorsivo
FCR.sh $*   #$* ===> $1

echo Il numero di file totale della gerarchia = $?
#N.B. Non si puo' usare il valore di conta che in questo shell è uguale a 0!!!
#OSSERVAZIONE: Non si possiede nessuna informazione riguardo i nomi dei file trovati