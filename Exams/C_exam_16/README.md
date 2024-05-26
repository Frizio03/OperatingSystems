## Testo di esame parte C
*22 Febbraio 2023*

La parte in C accetta un numero variabile di parametri **N** maggiore o uguale a 2 che rappresentano nomi di file (**F1, …FN**): si assuma (senza bisogno di effettuare alcun controllo) che il formato di tali file sia tale che i caratteri in posizione pari (offset interno al file 0, 2, 4, etc.) siano caratteri numerici, mentre i caratteri di posizione dispari siano caratteri ‘spazio’ (cioè blank).

Il processo padre deve generare un numero di processi figli pari a **N**: ogni processo figlio **Pn** è associato ad uno dei file **F1,…FN** (in ordine). 
Ognuno di tali processi figli **Pn** esegue concorrentemente e legge tutti i caratteri del proprio file associato operando una opportuna selezione come indicato nel seguito.

Ogni processo figlio **Pn** deve convertire i caratteri numerici nei corrispondenti numeri interi e calcolare via via la loro somma: tale somma deve essere memorizzata in una variabile di tipo long int (**somma**). Al termine della lettura del proprio file associato, ogni processo figlio Pn deve comunicare al processo padre **somma** (chiaramente in formato long int).

Il processo padre deve ricevere, rispettando l'ordine dei file **F1, …FN**, da ogni figlio i valori long int che rappresentano le somme calcolate dai figli e deve stamparle su standard output insieme con l’indice del processo che ha comunicato tale valore e il nome del file cui tale somma si riferisce.

Al termine dell’esecuzione, ogni figlio **Pn** ritorna al padre il numero (garantito essere minore di 255) di caratteri numerici trovati nel file associato; il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **car** per il carattere letto correntemente dai figli dal proprio file;
- una variabile di nome **somma** per il valore long int che i figli devono comunicare al padre.

Execute using:
```console
$ make
$ ./main fileA fileB fileC
```