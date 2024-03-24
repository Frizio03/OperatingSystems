#!/bin/sh

#26

#Inizializzazione variabili
curd=    #Memorizza la directory corrente
d=       #Memorizza il nome dela directory da cercare

#Controllo il numero di parametri
case $# in
0) echo "POCHI PARAMETRI - Usage is: $0 [directory] file" 
   exit 1;;
1) curd=`pwd`; d=$1;;
2) curd=$1; d=$2;;
*) echo "TROPPI PARAMETRI - Usage is: $0 [directory] file"
   exit 2;;
esac

#Controllo che d sia dato in forma assoluta
case $curd in
/*) ;;
*) echo Errore: $curd non in forma assoluta
   exit 3;;
esac

#Controllo che f sia dato in forma relativa semplice 
case $d in
*/*) echo Errore: $d non in forma relativa semplice 
     exit 4
;;
*)
;;
esac

#Controllo che d sia una directory e che sia traversabile 
if test ! -d $curd -o ! -x $curd
then
   echo Errore: $curd non directory o non attraversabile
   exit 5
fi

#Aggiungo il percorso del file corrente al path
PATH=`pwd`:$PATH
export PATH
echo Beginc-dir.sh: Stiamo per esplorare la directory $curd

#Invocazione file comandi ricorsivo
CercaDir.sh $curd $d

#Termine dello script
echo HO FINITO TUTTO