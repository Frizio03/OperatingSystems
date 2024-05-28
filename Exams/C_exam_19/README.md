## Testo di esame parte C
*16 Febbraio 2022*

La parte in C accetta un numero variabile di parametri **N+1** con **N** maggiore o uguale a 2: i primi **N** rappresentano nomi di file (**F1, …FN**), mentre l’ultimo parametro **C** rappresenta un singolo carattere alfabetico minuscolo (da controllare).

Il processo padre deve generare N processi figli (**P0 … PN-1**): i processi figli **Pi** (con i variabile da 0 a N-1) sono associati agli **N** file **Fk** (con **k = i+1**).

Ogni processo figlio **Pi** deve leggere i caratteri del file associato **Fk** cercando il carattere **C**. I processi figli e il processo padre devono attenersi a questo *schema di comunicazione a pipeline*: il figlio P0 comunica con il figlio P1 che comunica con il figlio P2 etc. fino al figlio PN-1 che comunica con il padre.

Questo schema a pipeline deve prevedere l’invio in avanti di un array di strutture dati ognuna delle quali deve contenere due campi:
1. c1, di tipo int, che deve contenere il pid di un processo;
2. c2, di tipo long int, che deve contenere il numero di occorrenze del carattere C calcolate dal corrispondente processo.

Gli array di strutture DEVONO essere creati da ogni figlio della dimensione minima necessaria per la comunicazione sia in ricezione che in spedizione. Quindi la comunicazione deve avvenire in particolare in questo modo: il figlio P0 passa in avanti (cioè comunica con una singola write) un array di strutture A1, che contiene una sola struttura con c1 uguale al proprio pid e con c2 uguale al numero di occorrenze del carattere **C** trovate da P0 nel file F1; il figlio seguente P1, dopo aver calcolato numero di occorrenze del carattere **C** nel file F2, deve leggere (con una singola read) l’array A1 inviato da P0 e quindi deve confezionare l’array A2 che corrisponde all’array A1 aggiungendo all’ultimo posto la struttura con i propri dati e la passa (con una singola write) al figlio seguente P2, etc. fino al figlio PN-1, che si comporta in modo analogo, ma passa al padre.

Quindi, il processo padre deve allocare l’array **AN** per ricevere quanto inviato dall’ultimo figlio e cioè l’array di **N** strutture (uno per ogni processo P0 … PN-1).

Il padre deve leggere (con una singola read) l’array **AN** e, quindi, deve riportare i dati di ognuna delle **N** strutture su standard output insieme al numero d’ordine del processo corrispondente, al nome del file associato a tale processo e al carattere C.

Al termine, ogni processo figlio **Pi** deve ritornare al padre il valore intero corrisponde al proprio indice d’ordine; il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di file;
- una variabile di nome **i** per l’indice dei processi figli;
- una variabile di nome **ch** per il carattere letto dai file dai figli;
- una variabile di nome **cur** per l’array dinamico creato da ogni figlio (della dimensione minima necessaria) e dal padre.

Execute using:
```console
$ make
$ ./main f1.txt f2.txt f3.txt f4.txt h
```