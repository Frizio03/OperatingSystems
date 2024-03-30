## Testo di esame parte SHELL
*17 Gennaio 2018*

La parte in Shell deve prevedere 4 parametri: il primo deve essere il nome assoluto di una directory che identifica una gerarchia (**G**) all’interno del file system, il secondo e il terzo devono essere considerati numeri interi strettamente positivi (**N** e **H**), mentre il quarto deve essere considerato un singolo carattere (**Cx**).
Il programma deve cercare nella gerarchia **G** specificata tutte le directory che contengono esattamente **N** file che abbiano un numero di linee uguale ad **H** e che contengano (nel loro contenuto) almeno una occorrenza del carattere **Cx**.
Si riporti il nome assoluto di tali directory sullo standard output al termine della ricorsione (facendo uso di un file temporaneo).

Execute using:
```console
$ ./FCP.sh `pwd`/ger 2 5 .
```