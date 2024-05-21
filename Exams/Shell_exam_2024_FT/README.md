## Testo di esame parte SHELL
*17 Aprile 2024*

La parte in Shell deve prevedere un numero variabile di parametri **Q+1** (con **Q** maggiore o uguale a 2): il primo parametro deve essere un nome relativo semplice (*N**) mentre gli altri **Q** devono essere nomi assoluti di directory che identificano **Q** gerarchie (**G1, G2, …**) all’interno del file system.

Il comportamento atteso dal programma, dopo il controllo dei parametri, è organizzato in **Q** fasi, una per ogni gerarchia.
Il programma, per ognuna delle **Q** fasi, deve esplorare la gerarchia G corrispondente - tramite un file comandi ricorsivo, FCR.sh – e deve contare globalmente tutti i file che rispettano la seguente specifica: in ogni directory, si deve individuare il file (escludendo che sia una directory) che si trova nella 4° posizione*; nel caso in cui in una directory non si trovi un file nella 4° posizione, va riportato il suo nome assoluto con una opportuna spiegazione.

Al termine di tutte le **Q** fasi, nel caso il numero totale dei file trovati nella 3° posizione all'interno delle varie directory delle varie gerarchie sia uguale a zero, si deve dare una indicazione di errore; in caso contrario, si deve riportare sullo standard output tale numero. Quindi, va creata nella directory corrente una nuova directory che deve avere nome relativo semplice quello passato come primo parametro (**N**); in questa nuova directory, per ogni file trovato nella 4° posizione all'interno delle varie directory delle varie gerarchie deve essere creato un link hardware che abbia lo stesso nome relativo semplice del file trovato.

**NB**: Se una directory della gerarchia contiene: a (file), b (file), d1 (dir), f1 (file), etc. allora il file nella 3° posizione che sarebbe individuato è f1; mentre se contiene: a (file), b (file), d1 (dir), allora non esiste un file nella 3° posizione.

### NOTA BENE NEI DUE FILE COMANDI SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per contenere il primo parametro di FCP.sh;
- la stringa `/tmp/absolutFiles` per il nome del file temporaneo;
- una variabile di nome **dirAss** per le singole gerarchie di ognuna delle Q fasi;
- una variabile di nome **file** per identificare, via via, i singoli file delle directory esplorate e poi quelli trovati;
- una variabile di nome **SOMMA** per contenere il numero totale dei file trovati.