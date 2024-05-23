## Testo di esame parte C
*10 Giugno 2020*

>[!WARNING]
>Esame molto breve ed elementare (primo dopo pandemia).

La parte in C accetta un numero variabile **Q** di parametri maggiore o uguale a 2 (da controllare) che rappresentano nomi assoluti di file **F1, ... FQ**.

Il processo padre deve, per prima cosa, creare un file con nome **out.txt** nella directory corrente, e quindi deve generare **Q** processi figli: i processi figli **Pq** sono associati ai **Q** file **Ff** (con **f = q+1**).

Ognuno di tali figli esegue concorrentemente leggendo dal file associato tutte le linee*: ogni figlio deve inviare la linea corrente al padre se e solo se il primo carattere di tale linea è un carattere numerico e se la lunghezza di tale linea compreso il terminatore di linea, è strettamente minore di 10.

Il padre deve ricevere, rispettando l'ordine dei file, tutte le linee inviate da ogni singolo figlio e deve stampare su standard output, per ogni linea ricevuta, l’indice del figlio che gliela ha inviata, il nome del file cui tale linea si riferisce e chiaramente la linea insieme ad una chiara indicazione di ciò che si sta
mostrando.

>[!NOTE] 
>Ogni linea va stampata dal padre solo DOPO aver terminato la ricezione dell’intera linea e dopo avere trasformato la linea ricevuta in una stringa!

Al termine, ogni processo figlio **Pq** deve ritornare al padre il numero di linee inviate al padre+ e il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **Q** per il numero di file/processi figli;
- una variabile di nome **q** per l’indice dei processi figli;
- una variabile di nome **linea** per memorizzare la linea (sia da parte dei figli che da parte del padre).

>[!NOTE] 
>Ogni linea ha lunghezza minore di 250 compreso il terminatore
>Ogni file ha un numero di linee minore di 255;

Execute using:
```console
$ make
$ ./main fileA fileB
```