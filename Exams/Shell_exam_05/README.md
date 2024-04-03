## Testo di esame parte SHELL
*17 Aprile 2015*

La parte in Shell deve prevedere un numero variabile **N+1** di parametri (con **N** maggiore o uguale a 2): il primo parametro deve essere considerato un numero intero **X** strettamente positivo, mentre gli altri N devono essere nomi assoluti di directory che identificano **N** gerarchie (**G1, G2, … GN**) all’interno del file system.
Il comportamento atteso dal programma, dopo il controllo dei parametri, è organizzato in **N** fasi, una per ogni gerarchia.
Il programma, per ognuna delle **N** fasi, deve esplorare la gerarchia Gi corrispondente - tramite un file comandi ricorsivo, FCR.sh - e deve contare globalmente tutti i file leggibili che soddisfano la seguente specifica: il contenuto del file deve essere tale per cui almeno **V** linee terminino con il carattere ‘t’.
Al termine di tutte le **N** fasi, si deve riportare sullo standard output il numero totale di file trovati globalmente che soddisfano la specifica precedente (file trovati); quindi, si deve riportare sullo standard output il nome assoluto di ogni file trovato chiedendo contestualmente all’utente un numero **K** strettamente positivo e strettamente minore di **X**: tale numero deve essere usato per riportare sullo standard output la linea **K-esima** del file trovato corrente.

Execute using:
```console
$ ./FCP.sh 3 `pwd`/ger1 `pwd`/ger2
```