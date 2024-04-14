## Testo di esame parte SHELL
*22 Aprile 2021*

La parte in Shell deve prevedere un numero variabile di parametri **Q+2** (con **Q** maggiore o uguale a 2): il primo parametro deve essere considerato un numero intero strettamente positivo (**W**), il secondo parametro deve essere una stringa (**S**) mentre gli altri **Q** devono essere nomi assoluti di directory che identificano **Q** gerarchie (**G1, G2, …**) all’interno del file system.
Il comportamento atteso dal programma, dopo il controllo dei parametri, è organizzato in **Q** fasi, una per ogni gerarchia.
Il programma, per ognuna delle Q fasi, deve esplorare la gerarchia **Gi** corrispondente - tramite un file comandi ricorsivo, FCR.sh – e deve contare globalmente tutte le directory (inclusa la radice della gerarchia) che contengono almeno un file il cui nome abbia terminazione `.S` dove **S** è la stringa passata come parametro.
Al termine di tutte le **Q** fasi, si deve riportare sullo standard output il numero totale di directory trovate globalmente (che soddisfano la condizione sopra indicata).
Se tale conteggio risulta maggiore o uguale a **W**, si deve chiedere all’utente, chiamandolo con il proprio nome di battesimo (in caso di nome multipli, se ne scelga uno) un numero intero **X** compreso fra 1 e **W**; quindi, si deve selezionare la directory corrispondente al numero **X** e di tale directory si deve riportare su standard output il nome assoluto.

### NOTA BENE NEI DUE FILE COMANDI SI USI OBBLIGATORIAMENTE:
* Una variabile di nome **W** per contenere il primo parametro di FCP.sh;
* Una variabile di nome **S** per contenere il secondo parametro di FCP.sh;
* Il nome `/tmp/nomiAssoluti` per il file temporaneo
* Una variabile di nome **G** per le singole gerarchie di ognuna delle **Q** fasi;
* Una variabile di nome **F** per identificare, via via, i singoli file delle directory esplorate e della directory selezionata;
* Una variabile di nome **X** per contenere il numero letto.

Execute using
```console
$ ./FCP.sh 3 idoneo `pwd`/ger1 `pwd`/ger2
```