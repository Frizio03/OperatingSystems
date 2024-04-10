## Testo di esame parte SHELL
*05 Aprile 2019* 

La parte in Shell deve prevedere un numero variabile di parametri **N+1** (con **N** maggiore o uguale a 2): il primo parametro deve essere considerato un nome relativo semplice **F**, mentre gli altri **N** devono essere nomi assoluti di directory che identificano **N** gerarchie (**G1, G2, …**) all’interno del file system.
Il comportamento atteso dal programma, dopo il controllo dei parametri, è organizzato in **N** fasi, una per ogni gerarchia.
Il programma, per ognuna delle **N** fasi, deve esplorare la gerarchia **Gi** corrispondente - tramite un file comandi ricorsivo, FCR.sh - e deve contare globalmente tutti i file che hanno le caratteristiche di seguito specificate.
Il file comandi ricorsivo FCR.sh deve cercare in tutte le directory ogni file leggibile con nome relativo semplice **F** e con lunghezza in linee maggiore o uguale 4; si dovrà quindi salvare in modo opportuno il nome assoluto del file trovato contestualmente alla sua lunghezza in linee.
Al termine di tutte le **N** fasi, si deve riportare sullo standard output il numero totale di file trovati globalmente.
In seguito, per ogni file trovato, si deve riportare sullo standard output il suo nome assoluto, e quindi la sua lunghezza in linee (entrambe queste informazioni precedentemente salvate): quindi si deve richiedere all’utente un numero intero **X** compreso fra 1 e il numero corrispondente alla lunghezza in linee e, se tale numero è corretto, si devono riportare su standard output le prime **X** linee del file con opportune frasi che facciano capire all’utente che cosa si sta visualizzando.

Execute using
```console
$ ./FCP.sh target `pwd`/ger1 `pwd`/ger2
```