#!/bin/sh

#01
echo Stampo la posizione del file01.txt: `pwd`/file01.txt
echo

#02
a=ciao		#Assegno il valore ciao alla variabile a
echo Valore della variabile a: $a e directory corrente `pwd` e valore di asterisco *
echo

#03 - inibizione delle sostituzioni
echo 'Valore della variabile a: $a e directory corrente `pwd` e valore di asterisco *'
echo

#04 - inibizione delle sole wildcard
echo "Valore della variabile a: $a e directory corrente `pwd` e valore di asterisco *"
echo