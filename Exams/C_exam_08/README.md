## Testo di esame parte C
*16 Giugno 2021*

La parte in C accetta un numero variabile di parametri **N** maggiore o uguale a 2 che rappresentano nomi di file (**F1, …FN**).

Il processo padre deve, per prima cosa, creare nella directory corrente un file **fcreato** con nome corrispondente al proprio cognome (in caso di più cognomi se ne usi solo uno, inserendo un opportuno commento).

Il processo padre deve generare un numero di processi figli pari a **N**: ogni processo figlio **Pn** è associato ad uno dei file **F1, …FN** (in ordine); la lunghezza in linee di tutti i file è uguale e non deve essere controllata.

Ognuno di tali processi figli **Pn** esegue concorrentemente e legge tutte le linee del proprio file associato: le linee lette devono essere scritte nel file **fcreato** seguendo le indicazioni fornite nel seguito.

I processi figli **Pn** devono usare uno *schema di comunicazione a ring*: la comunicazione deve prevedere l’invio in avanti di un array di grandezza **N** e in cui ogni elemento dell’array corrisponde alla linea corrente (supposta lunga 250 caratteri compreso il terminatore di linea1) letta dal corrispondente processo figlio **Pn**. Quindi, il generico processo **Pn**, dopo aver letto la linea corrente, deve ricevere dal figlio precedente l’array di linee e, dopo aver inserito la linea corrente nella posizione giusta dell’array di linee, deve inviare l’array di linee al figlio successivo, con **PN-1** che manda a **P0**; il figlio **PN-1**, prima di riprendere il ciclo di lettura, deve scrivere le linee correnti sul file **fcreato** usando chiaramente l’array di linee corrente. Per semplicità, il primo ciclo può essere attivato dal padre che manda un array di linee (senza informazioni significative) al primo figlio **P0**.

Al termine dell’esecuzione, ogni figlio **Pn** ritorna al padre la lunghezza dell’ultima linea letta dal proprio file compreso il terminatore di linea (sicuramente minore di 255); il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **linea** per la linea letta correntemente dai figli dal proprio file;
- una variabile di nome **tutteLinee** per l’array con tutte le linee lette correntemente dai figli.

Execute using:
```console
$ make
$ ./main fileA fileB
```