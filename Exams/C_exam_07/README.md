## Testo di esame parte C
*09 Giugno 2021*

La parte in C accetta un numero variabile N di parametri maggiore o uguale a 2 (da controllare) che rappresentano nomi assoluti di file F1, ... FN (che hanno la stessa lunghezza in linee che non va controllata).

Il processo padre deve, per prima cosa, creare nella directory di sistema `/tmp` un file **fcreato** con nome corrispondente alla concatenazione (senza spazi) del proprio nome e del proprio cognome (in caso di più nomi o più cognomi se ne usi solo uno, inserendo un opportuno commento).
Quindi deve generare un processo figlio ‘speciale’ che deve calcolare la lunghezza in linee del file F1 (che sarà poi la lunghezza in linee di tutti i file) usando in modo opportuno il comando `wc` di UNIX/Linux.

Il padre deve convertire in termini di valore intero quanto scritto in termini di caratteri sullo standard output dal comando `wc` eseguito dal figlio speciale: tale valore servirà al padre per recuperare le informazioni inviate dagli ulteriori figli.

Il processo padre poi deve generare **N** processi figli: i processi figli **Pn** sono associati agli **N** file **Ff** (con **f = n+1**). Ognuno di tali figli esegue concorrentemente leggendo dal file associato tutte le linee (ogni linea si può supporre che abbia una lunghezza massima di 200 caratteri, compreso il terminatore di linea): ogni figlio **Pn**, per ogni linea letta, deve comunicare al padre prima la lunghezza (come `int`) della linea corrente compreso il terminatore di linea e quindi la linea stessa.

Il padre deve ricevere rispettando l’ordine delle linee e quindi l'ordine dei file, utilizzando in modo opportuno le informazioni inviate dai figli: il padre deve scrivere tutte le linee inviate sul file **fcreato**.

Quindi in tale file al termine si dovrà trovare una sorta di merge linea-linea dei file passati come parametri e quindi si dovrà trovare la prima linea inviata dal processo figlio **P0** (letta dal file F1), quindi la prima linea inviata dal processo figlio **P1** (letta dal file F2) e così via.

Al termine, ogni processo figlio **Pn** deve ritornare al padre la lunghezza dell’ultima linea inviata al padre compreso il terminatore di linea e il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di file/processi figli;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **linea** per memorizzare la linea (sia da parte dei figli che da parte del padre).

Execute using:
```console
$ make
$ ./main fileA fileB fileC
```