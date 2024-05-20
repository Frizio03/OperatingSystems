## Testo di esame parte C
*13 Luglio 2022*

La parte in C accetta un numero variabile di parametri **Q+2** (con **Q** maggiore o uguale a 2), **F**, **L** e **C1, …CQ**, che rappresentano rispettivamente le seguenti informazioni: il primo il nome assoluto di un file (**F**), il secondo la lunghezza in linee di **F** (**L**, strettamente positivo da controllare) e gli ultimi **Q** devono essere considerati singoli caratteri (da controllare).

Il processo padre deve generare un numero di processi figli pari a **Q**: ogni processo figlio **Pq** è associato ad uno dei caratteri **C1, …CQ** (in ordine).

Ognuno di tali processi figli **Pq** esegue concorrentemente e calcola il numero di occorrenze del proprio carattere associato, per ognuna delle **L** linee del file **F**.

I processi padre e figli devono sincronizzarsi strettamente a vicenda utilizzando uno *schema di sincronizzazione a ring* che deve comprendere anche il padre in modo che, per ognuna delle **L** linee del file **F**, sullo standard output siano scritte le seguenti informazioni: il padre deve riportare il numero di linea correntemente analizzata da tutti i processi figli.
(**NB**: la numerazione delle linee deve essere fatta partire da 1), insieme con il nome del file; il primo figlio (P0) deve riportare il numero di occorrenze del proprio carattere (C1) trovate nella linea corrente, insieme con il proprio indice e il proprio PID; il secondo figlio (P1) deve riportare il numero di occorrenze del proprio carattere (C1) trovate nella linea corrente, insieme con il proprio indice e il proprio PID; così via per tutti i figli e per tutte le L linee del file F.

### ESEMPIO DI OUTPUT, SUPPONENDO DI AVERE 2 CARATTERI PASSATI E 2 LINEE NEL FILE F:
```
Linea 1 del file /home/soELab/prova.txt
Figlio con indice 0 e pid 13290 ha letto 1 caratteri S nella linea corrente
Figlio con indice 1 e pid 13291 ha letto 2 caratteri n nella linea corrente
Linea 2 del file /home/soELab/prova.txt
Figlio con indice 0 e pid 13290 ha letto 2 caratteri S nella linea corrente
Figlio con indice 1 e pid 13291 ha letto 7 caratteri n nella linea corrente
```

Al termine dell’esecuzione, ogni figlio Pq ritorna al padre il numero (supposto strettamente minore di 255) di occorrenze trovate nell’ultima linea del file F; il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **Q** per il numero di processi figli;
- una variabile di nome **q** per l’indice dei processi figli;
- una variabile di nome **L** per la lunghezza in linee del file F.