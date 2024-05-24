## Testo di esame parte C
*06 Settembre 2023*

La parte in C accetta un numero variabile di parametri **N** (con **N** maggiore o uguale a 2) che rappresentano nomi di file (**F1, …FN**).

Il processo padre deve generare **N** processi figli: ogni processo figlio **Pn** è associato ad uno dei file **F1, …FN** (in ordine). Ognuno di tali processi figli **Pn** esegue concorrentemente e, per prima cosa, deve calcolare la lunghezza in linee del proprio file associato in termini di long int. Quindi, i processi figli **Pn** e il processo padre devono attenersi a questo *schema di comunicazione a pipeline*: il figlio P0 comunica con il figlio P1 che comunica con il figlio P2 etc. fino al figlio PN-1 che comunica con il padre.

Questo schema a pipeline deve prevedere l’invio in avanti di una singola struttura dati, che deve contenere due campi:
1. c1, di tipo long int, che deve contenere il valore minimo di linee calcolato dal processo **Pn**;
2. c2, di tipo int, che deve contenere l’indice d’ordine (**n**) del processo che ha calcolato il minimo e perciò, al processo padre deve arrivare una singola struttura: il padre deve riportare i dati di tale struttura su standard output insieme al PID del processo che ha calcolato il minimo numero di linee e al nome del file per cui sono stati calcolati (inserendo opportune spiegazioni per l’utente).

Esaurita questa fase a pipeline, ogni processo figlio **Pn** deve aspettare di ricevere il numero minimo di linee (di tipo long int) inviato dal padre (secondo quanto specificato in seguito) e, una volta selezionata la linea corrispondente a tale numero, deve stampare su standard output: il suo indice d’ordine, il suo PID, il numero di linea inviato dal padre (che rappresenta il minimo calcolato nella fase precedente), il nome del proprio file associato e quindi la linea selezionata.

Il padre, dopo aver ricevuto la struttura, deve comunicare a tutti i figli **Pn** il numero minimo di linee ricevuto (che si ricorda è un long int): tale valore produrrà una stampa sullo standard output da parte dei figli, come spiegato precedentemente.

Al termine dell’esecuzione, ogni figlio **Pn** ritorna al padre il valore intero corrispondente al proprio indice d’ordine (**n**); il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **linea** per l’array di 250 caratteri per le linee lette dai figli dal proprio file; si supponga che 250 caratteri siano sufficienti per ogni linea, compreso il terminatore di linea/stringa.
- una variabile di nome **cur_tot** per il calcolo del numero di linee effettuato dai figli;
- una variabile di nome **min** per il valore minimo ricevuto dai figli da parte del padre.

Execute using:
```console
$ make
$ ./main 
```