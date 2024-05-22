## Testo di esame parte C
*07 Giugno 2023*

La parte in C accetta un numero variabile di parametri **N** maggiore o uguale a 2 che rappresentano nomi di file (**F1, …, FN**): si assuma (senza bisogno di effettuare alcun controllo) che i file NON siano vuoti.

Il processo padre deve generare un numero di processi figli pari a **N**: ogni processo figlio **Pn** è associato ad uno dei file **F1, …, FN** (in ordine). Ognuno di tali processi figli **Pn** esegue concorrentemente e legge tutte le linee del proprio file associato operando una opportuna selezione come indicato nel seguito.

Dopo la lettura di ogni linea, il processo figlio **Pn** deve comunicare al padre l’ultimo carattere della linea corrente (prima del terminatore di linea) e deve ricevere dal padre l’indicazione di stampare o meno su standard output delle informazioni (vedi dopo *).

Il padre deve ricevere, rispettando l’ordine dei file **F1, …, FN**, da ogni figlio via via i caratteri che rappresentano l’ultimo carattere della linea corrente.

Quindi, al processo padre deve arrivare un insieme di caratteri (che via via potrebbe diminuire in quantità in dipendenza della terminazione dei figli): sicuramente il primo insieme (questo viene garantito dalla parte Shell) è costituito dall’ultimo carattere della prima linea inviato dal figlio **P0**, dall’ultimo carattere della prima linea inviato dal figlio **P1**, … , dall’ultimo carattere della prima linea inviato dal figlio **PN-1**.

Per ogni insieme ricevuto, il padre deve determinare il valore massimo e, SOLO AL PROCESSO FIGLIO CHE HA INVIATO TALE VALORE, deve indicare (*) di stampare su standard output l’indice d’ordine del processo, il suo PID, il carattere identificato come massimo e quindi la linea corrente, mentre a tutti gli altri processi figli deve indicare di non stampare.

Al termine, ogni processo figlio Pn deve ritornare al padre il numero di linee che ha scritto sullo standard output (supposto minore di 255) e il padre deve stampare su standard output i PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **linea** per le linee che via via devono leggere i processi figli;
- una variabile di nome **car** per i caratteri che via via deve leggere il processo padre.

Execute using:
```console
$ make
$ ./main fileA fileB fileC
```