#!/bin/sh

#Si definisce la variabile i e le si assegna un valore (nessuno spazio!)
i=12

#i vale la stringa 12
echo Variabile i vale $i

#si definisce la variabile j come concatenazione della stringa
#contenuta nella variabile i e della stringa 1 (nessuno spazio!)
j=$i+1 

echo Variabile j vale $j
#se si vuole ottenere l'operazione + bisogna usare il comando expr, usando la valutazione di comandi (backquote!)
j=`expr $i + 1`

#si chiede di assegnare alla variabile j la somma del contenuto di $i considerato come numero e 1!
#NOTA BENE: nel comando expr sono vitali gli SPAZI!
echo Variabile j vale $j

#ne scrivo un po' qui oppure (ALT + 96) --> ```````````````````