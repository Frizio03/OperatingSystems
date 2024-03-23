#!/bin/sh

#stampo il valore di alcune variabili
echo Valore della variabile a\: $a
echo Valore della variabile PATH\: $PATH
echo Valore della variabile HOME\: $HOME

#NB: senza assegnare nulla ad a, a non vale nulla
#utilizzando il comando env non esiste a

#Assegno e stampo un nuovo valore ad a
a="hello world"
echo Nuovo valore della variabile a\: $a

#Utilizzando il comando env viene visualizzata anche a