## Testo di esame parte C
*12 Luglio 2023*

La parte in C accetta un numero variabile di parametri **N+1** (con **N** maggiore o uguale a 2): il primo parametro rappresenta un numero intero strettamente positivo **L**, mentre gli altri **N** rappresentano nomi di file (**F1, …FN**): si assuma (senza bisogno di effettuare alcuncontrollo) che il formato di tali file sia tale che abbiano lunghezza in linee minore o uguale a **L**.

Il processo padre deve, per prima cosa, inizializzare il seme per la generazione random di numeri (come illustrato nel retro del foglio), quindi, deve generare **N** processi figli: ogni processo figlio **Pn** è associato ad uno dei file **F1, …FN** (in ordine).

Ognuno di tali processi figli **Pn** esegue concorrentemente e, per prima cosa, deve calcolare la lunghezza in linee del proprio file associato. Quindi, i processi figli **Pn** e il processo padre devono attenersi a questo schema di comunicazione a *pipeline*: il figlio **P0** comunica con il figlio **P1** che comunica con il figlio **P2** etc. fino al figlio **PN-1** che comunica con il padre.

Questo schema a pipeline deve prevedere l’invio in avanti di una singola struttura dati, che deve contenere due campi:  
1. c1, di tipo int, che deve contenere il valore massimo di linee calcolato dal processo **Pn**;
2. c2, di tipo int, che deve contenere l’indice d’ordine **n** del processo che ha calcolato il massimo (come visto in altri testi di esame) e perciò, al processo padre deve arrivare una singola struttura: il padre deve riportare i dati di tale struttura su standard output insieme al nome del file per cui sono stati calcolati (inserendo opportune spiegazioni per l’utente).

Esaurita questa fase a *pipeline*, ogni processo figlio **Pn** deve aspettare un numero di linea inviato dal padre (secondo quanto di seguito specificato) e, nel caso tale numero sia ammissibile per la lunghezza in linee del proprio file associato, deve stampare su standard output il suo indice d’ordine, il suo PID, il numero di linea inviato dal padre, il nome del proprio file associato e quindi la linea selezionata; in caso contrario, dovrà stampare una indicazione di NON ammissibilità.

Il padre, dopo aver verificato che il numero **L** sia uguale (*) al campo c1 della struttura ricevuta, deve applicare su tale valore la funzione `mia_random()` (riportata nel retro del foglio) per individuare un intero (r) che rappresenterà un numero di linea che il padre deve comunicare a tutti i figli **Pn**: tale intero **r**, se ammissibile, produrrà
una stampa sullo standard output da parte dei figli, come spiegato precedentemente.

Al termine dell’esecuzione, ogni figlio **Pn** ritorna al padre il valore 0 se il numero ricevuto era ammissibile per il proprio file associato (semantica di successo di UNIX), altrimenti il valore 1; il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

>[!NOTE]
>(*) Nella soluzione viene controllato che il campo della struct sia <= **L** poichè i file posso contenere linee <= L ed il massimo non sarà necessariamente **L**! (Errore in testo di esame?)

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **L** per il numero passato come primo parametro;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **linea** per l’array di 250 caratteri per le linee lette dai figli dal proprio file; si supponga che 250 caratteri siano sufficienti per ogni linea, compreso il terminatore di linea/stringa.
- una variabile di nome **r** per il valore random calcolato dal padre.

Execute using:
```console
$ make
$ ./main 10 fileA fileB
```