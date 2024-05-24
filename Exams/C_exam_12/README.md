## Testo di esame parte C
*17 Gennaio 2024*

La parte in C accetta un numero variabile di parametri **N** maggiore o uguale a 2 che rappresentano nomi di file (**F1, …FN**): si assuma (senza bisogno di effettuare alcun controllo) che i file abbiano tutti la stessa lunghezza in termini di linee.

Il processo padre deve generare un numero di processi figli pari a **N**: ogni processo figlio **Pn** è associato ad uno dei file **F1, …FN** (in ordine) Ognuno di tali processi figli **Pn** esegue concorrentemente e legge tutte le linee del proprio file associato operando una opportuna selezione come indicato nel seguito.

Dopo la lettura dell’ultima linea, il processo figlio **Pn** deve comunicare al padre il primo carattere di tale linea e deve ricevere dal padre l’indicazione di stampare o meno su standard output delle informazioni (vedi dopo *).

Il padre deve ricevere, rispettando l’ordine dei file **F1, …FN**, da ogni figlio via via i caratteri che rappresentano il primo carattere della ultima linea. Quindi, al processo padre deve arrivare un insieme di **N** caratteri: il padre deve determinare il valore minimo e, SOLO AL PROCESSO FIGLIO CHE HA INVIATO TALE VALORE, deve indicare (*) di stampare su standard output l’indice d’ordine del processo, il suo PID, il carattere identificato come minimo e quindi l’ultima linea, mentre a tutti gli altri processi figli deve indicare di non stampare.

Al termine, ogni processo figlio **Pn** deve ritornare al padre il numero di linee che ha scritto sullo standard output (quindi il valore 1 o 0) e il padre deve stampare su standard output i PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **linea** per l’array di 250 caratteri per le linee lette dai figli dal proprio file; si supponga che 250 caratteri siano sufficienti per ogni linea, compreso il terminatore di linea/stringa.
- una variabile di nome **car** per i caratteri che via via deve leggere il processo padre.

Execute using:
```console
$ make
$ ./main 
```