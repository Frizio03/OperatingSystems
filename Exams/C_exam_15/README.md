## Testo di esame parte C
*25 Gennaio 2023*

La parte in C accetta un numero variabile di parametri **N+1** (con N maggiore o uguale a 2) che rappresentano nomi di file (**F1, …FN, FN+1**).

Il processo padre deve, per prima cosa, aprire il file **FN+1** in scrittura e quindi deve generare un numero di processi figli pari a **N**: ogni processo figlio **Pn** è associato ad uno dei primi **N** file **F1, …FN** (in ordine).

Ognuno di tali processi figli **Pn** esegue concorrentemente e legge il proprio file associato, come specificato in seguito.

Ogni figlio **Pn** deve leggere 2 caratteri alla volta dal proprio file associato, fino alla fine di tale file, e deve mandare i caratteri correntemente letti al padre; il padre deve ricevere i caratteri inviati via via dai figli: prima i 2 caratteri inviati dal figlio P0, poi i 2 caratteri inviati dal figlio P1 e via via fino ai 2 caratteri inviati dal figlio PN-1, per poi ricominciare a ricevere altri 2 caratteri inviati dal figlio P0, e così via fino a che non ci saranno più caratteri da ricevere dai figli; fare attenzione che se la dimensione del file associato ad un figlio non è un multiplo intero di 2, tale figlio manderà nell’ultimo invio un numero minore di 2 caratteri al padre!

Il padre deve scrivere tutti i caratteri ricevuti dai figli alla fine del file **FN+1**.

Al termine dell’esecuzione, ogni figlio **Pn** ritorna al padre il numero totale di caratteri inviati (supposto minore di 255); il
padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di processi figli;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile **fdw** per il file descriptor del file aperto in scrittura;
- una variabile di nome **chs** per l’array da passare dai figli al padre.

Execute using:
```console
$ make
$ ./main fileA fileB fileC fout.txt
```