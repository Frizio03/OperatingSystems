## Testo di esame parte SHELL
*12 Aprile 2023*

La parte in Shell deve prevedere un numero variabile di parametri **Q+2** (con **Q** maggiore o uguale a 2): i primi due parametri devono essere semplici stringhe (**S1** e **S2**) mentre gli altri **Q** devono essere nomi assoluti di directory che identificano **Q** gerarchie (**G1, G2, …**) all’interno del file system.
Il comportamento atteso dal programma, dopo il controllo dei parametri, è organizzato in **Q** fasi, una per ogni gerarchia. Il programma, per ognuna delle **Q** fasi, deve esplorare la gerarchia **G** corrispondente - tramite un file comandi ricorsivo, FCR.sh – e deve contare globalmente sia tutti file leggibili con estensione* **S1** sia tutti i file leggibili con estensione **S2**.
Al termine di tutte le **Q** fasi, si deve riportare sullo standard output il numero totale (**TOT1**) dei file leggibili con estensione **S1** e il numero totale (**TOT2**) dei file leggibili con estensione **S2** trovati.
Se risulta che **TOT1** <= **TOT2**, si deve chiedere all’utente, chiamandolo con il proprio nome** (in caso di nome multipli, se ne scelga uno) un numero intero **X** compreso fra 1 e **TOT1**; quindi, si deve selezionare sia il nome del file leggibile con estensione **S1** sia il nome del file leggibile con estensione **S2** corrispondente al numero **X** fornito e di tali due file si deve riportare su standard output il nome assoluto.

### NOTA BENE NEI DUE FILE COMANDI SI USI OBBLIGATORIAMENTE:
* una variabile di nome **S1** per contenere il primo parametro di FCP.sh;
* una variabile di nome **S2** per contenere il secondo parametro di FCP.sh;
* la stringa `/tmp/nomiAssoluti` per la parte iniziale dei nomi dei file temporanei
* una variabile di nome **G** per le singole gerarchie di ognuna delle **Q** fasi;
* una variabile di nome **F** per identificare, via via, i singoli file delle directory esplorate;
* una variabile di nome **TOT1** per contenere il numero totale dei file leggibili con estensione **S1**.
* una variabile di nome **TOT2** per contenere il numero totale dei file leggibili con estensione **S2**.
* una variabile di nome **X** per contenere il numero letto.

Execute using:
```console
$ FCP.sh 
```