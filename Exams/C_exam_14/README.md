## Testo di esame parte C
*07 Settembre 2022*

La parte in C accetta un numero variabile di parametri **N** maggiore o uguale a 2 che rappresentano nomi di file (**F1, …FN**), che hanno tutti la stessa lunghezza dispari (da non controllare).

Il processo padre deve, per prima cosa, creare nella directory corrente un file **fcreato**. Il processo padre deve generare un numero di processi figli pari a **N**: ogni processo figlio **Pn** è associato ad uno dei file **F1, …FN** (in ordine).

Ognuno di tali processi figli **Pn** esegue concorrentemente e legge tutti i caratteri del proprio file associato operando una opportuna selezione come indicato nel seguito. I processi figli **Pn** devono usare uno *schema di comunicazione a pipeline*: il figlio P0 comunica con il figlio P1 che comunica con il figlio P2 etc. fino al figlio PN-1 che comunica con il padre.

Questo schema a pipeline deve prevedere l’invio in avanti, per ognuno dei caratteri dispari, di un array di grandezza **N** e in cui ogni elemento dell’array corrisponde al carattere corrente **car** di posizione dispari (il primo carattere ha posizione 1) letto dal corrispondente processo figlio **Pn**.

Quindi, il generico processo **Pn**, dopo aver letto il carattere dispari corrente, deve ricevere dal figlio precedente (a parte il processo P0) l’array di caratteri dispari e, dopo aver inserito il proprio carattere dispari corrente nella posizione giusta dell’array, deve inviare l’array di caratteri dispari al figlio successivo, con **PN-1** che manda al padre.

Quindi al padre deve arrivare, per ognuno dei caratteri dispari, un array di caratteri dispari, ognuno dei quali deve essere scritto sul file **fcreato**.

Al termine dell’esecuzione, ogni figlio **Pn** ritorna al padre l’ultimo carattere dispari letto dal proprio file (sicuramente minore di 255); il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile **fdout** per il file descriptor del file creato;
- una variabile di nome **car** per il carattere letto correntemente dai figli dal proprio file;
- una variabile di nome **cur** per l’array da passare fra i vari figli fino al padre.

Execute using:
```console
$ make
$ ./main fileA fileB fileC
```