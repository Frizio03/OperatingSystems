## Testo di esame parte SHELL
*18 Gennaio 2017*

La parte in Shell deve prevedere 2 parametri: il primo deve essere il nome assoluto di una directory che identifica una gerarchia (**G**) all’interno del file system, mentre il secondo deve essere il nome relativo semplice di una directory (**DR**). Il programma deve cercare nella gerarchia **G** specificata
tutte le directory di nome **DR** che contengono almeno un file che contenga (al suo interno) almeno una occorrenza di un carattere numerico.
Si riporti il nome assoluto di tali directory sullo standard output al termine della ricorsione. (Fare uso di un file temporaneo).

Execute using:
```console
$ ./FCP.sh `pwd`/ger targetDir
```