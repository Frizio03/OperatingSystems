## Testo di esame parte C
*14 Luglio 2021*

La parte in C accetta un numero variabile di parametri **N+1** maggiore o uguale a 2 che rappresentano i primi **N** nomi di file (**F1, …FN**), mentre l’ultimo rappresenta un numero intero strettamente positivo (n**roLinee**) (da controllare) che indica la lunghezza in linee dei file.

Il processo padre deve, per prima cosa, creare nella directory corrente un file di nome **fcreato**.
Il processo padre deve generare un numero di processi figli pari a **N**: ogni processo figlio **Pn** è associato ad uno dei file **F1, …FN** (in ordine); la lunghezza in linee di tutti i file è uguale a **nroLinee** e non deve essere controllata.

Ognuno di tali processi figli **Pn** esegue concorrentemente e legge tutte le linee del proprio file associato: le linee lette devono essere scritte nel file fcreato seguendo le indicazioni fornite nel seguito.

I processi figli Pn devono usare uno *schema di comunicazione a pipeline*: il figlio **P0** comunica con il figlio **P1** che comunica con il figlio **P2** etc. fino al figlio **PN-1** che comunica con il padre. Questo schema a pipeline deve prevedere l’invio in avanti, per ognuna delle **nroLinee** linee dei file, di un array di grandezza **N** e in cui ogni elemento dell’array corrisponde alla linea corrente (supposta lunga 250 caratteri compreso il terminatore di linea) letta dal corrispondente processo figlio **Pn**.

Quindi, il generico processo **Pn**, dopo aver letto la linea corrente, deve ricevere dal figlio precedente (a parte il processo P0) l’array di linee e, dopo aver inserito la linea corrente nella posizione giusta dell’array di linee, deve inviare l’array di linee al figlio successivo, con **PN-1** che manda al padre.

Quindi al padre deve arrivare, per ognuna delle **nroLinee** linee un array di grandezza N e in cui ogni elemento dell’array corrisponde alla linea corrente letta dai figli **Pn**: il padre deve scrivere le linee correnti sul file **fcreato**.

Al termine dell’esecuzione, ogni figlio **Pn** ritorna al padre la lunghezza dell’ultima linea letta dal proprio file compreso il terminatore di linea (sicuramente minore di 255); il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **nroLinee** per la lunghezza in linee dei file;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **linea** per la linea letta correntemente dai figli dal proprio file;
- una variabile di nome **tutteLinee** per l’array con tutte le linee lette correntemente dai figli.