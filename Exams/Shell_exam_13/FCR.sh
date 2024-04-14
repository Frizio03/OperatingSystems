#!/bin/sh

#Mi sposto nella directory di interesse
cd $1

#Inizializzazione variabile di risultato
trovato=false

#Controllo sulla radice della gerarchia (contiene file .S)
for F in *
do
	#Se l'elemento è un file
	if test -f $F
	then
		#Controllo che abbia terminazione come stringa passata
		case $F in
		*.$2)
			#La directory corrente contiene un file idoneo
			trovato=true ;;
		*)
			#Non faccio nulla
			;;
		esac
	fi
done

#Se ho trovato almeno un file idoneo nella directory corrente
if test $trovato = true
then
	#Aggiungo la directory corrente all'elenco
	pwd >> $3
fi

#Esplorazione ricorsiva delle sotto-directory
for i in *
do
	#Se l'elemento è una directory traversabile
	if test -d $i -a -x $i
	then
		#Invocazione ricorsiva passando il nome assoluto
		echo Directory: $i
		FCR.sh `pwd`/$i $2 $3
	fi
done