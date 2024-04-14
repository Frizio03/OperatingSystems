## Testo di esame parte SHELL
*03 Giugno 2020* 

La parte in Shell deve prevedere un numero variabile di parametri **N+1** (con **N** maggiore o uguale a 2): il primo parametro deve essere considerato un singolo carattere **C**, mentre gli altri **N** devono essere nomi assoluti di directory che identificano **N** gerarchie (**G1, G2, …**) all’interno del file system.
Il comportamento atteso dal programma, dopo il controllo dei parametri, è organizzato in **N** fasi, una per ogni gerarchia.
Il programma, per ognuna delle **N** fasi, deve esplorare la gerarchia **Gi** corrispondente - tramite un file comandi ricorsivo, FCR.sh - e deve contare globalmente tutte le directory **D** che hanno le caratteristiche di seguito specificate.
Il file comandi ricorsivo FCR.sh deve cercare tutte le directory **D** il cui nome sia di 3 caratteri e i cui caratteri dispari, sempre nel nome, siano uguali al carattere **C**.
Al termine di tutte le **N** fasi, si deve riportare sullo standard output:
* il numero totale di directory **D** trovate globalmente (che soddisfano la condizione sopra indicata);
* i nomi assoluti delle directory **D** trovate e, contestualmente, per ogni directory **D** si deve chiedere all’utente, chiamandolo Elena, se vuole o meno visualizzare il contenuto della directory **D** compresi anche gli elementi nascosti.

Execute using
```console
$ ./FCP.sh a `pwd`/ger1 `pwd`/ger2 `pwd`/aha
```