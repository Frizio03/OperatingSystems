## Testo di esame parte SHELL
*12 Luglio 2023* 

La parte in Shell deve prevedere un numero variabile di parametri **Q+1** (con **Q** maggiore o uguale a 2): il primo parametro deve essere considerato un numero intero (**X**) strettamente positivo e strettamente minore di 200, mentre gli altri **Q** devono essere nomi assoluti di directory che identificano **Q** gerarchie (**G1, G2, …**) all’interno del file system.
Il comportamento atteso dal programma, dopo il controllo dei parametri, è organizzato in **Q** fasi, una per ogni gerarchia.
Il programma, per ognuna delle **Q** fasi, deve esplorare la gerarchia **G** corrispondente - tramite un file comandi ricorsivo, FCR.sh – e deve contare globalmente tutti i file leggibili **F** aventi estensione `.txt` la cui lunghezza in linee sia strettamente minore di **X**; contestualmente* si deve anche calcolare la lunghezza (in linee) massima **L** dei file trovati.
Al termine di tutte le Q fasi, si deve riportare sullo standard output il numero totale di tali file trovati globalmente e la lunghezza (in linee) massima L.
Quindi, solo nel caso siano stati trovati almeno due file, si deve invocare la parte in C, passando come parametri L e i nomi dei file trovati (F1, F2, ...).

### NOTA BENE NEI DUE FILE COMANDI SI USI OBBLIGATORIAMENTE:
* una variabile di nome **X** per contenere il primo parametro di FCP.sh;
* una variabile di nome **G** per le singole gerarchie di ognuna delle **Q** fasi;
* la stringa `/tmp/nomiAssoluti` per la parte iniziale dei nomi dei file temporanei;
* una variabile di nome **L** per la lunghezza in linee massima;
* una variabile di nome **F** per identificare, via via, i singoli file delle directory esplorate.

Execute using
```console
$ ./FCP.sh 10 `pwd`/ger1 `pwd`/ger2
```