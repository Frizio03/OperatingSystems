#!/bin/sh

#51-53

# ------ Su schell eseguo i seguenti comandi

#Definizione delle variabili
x="I_am_X"
y="I_am_Y"
z="I_am_Z"

#Stampa delle variabili
echo Il valore della variabile x = $x
echo Il valore della variabile y = $y
echo Il valore della variabile z = $z

#Rendo le varaiabili, variabili di ambiente
export x
export y
export z

#Verifico la presenza delle variabili con env
env

#54
# ------ Su nuova schell eseguo lo script
./209_comandi.sh

#Verifico di nuovo il valore di z (non cambia!!!)
echo Il valore della variabile z = $z