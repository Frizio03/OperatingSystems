## Testo di esame parte SHELL
*15 Febbraio 2017*

La parte in Shell deve prevedere due parametri: il primo deve essere il nome assoluto di una directory che identifica una gerarchia (G) all’interno del file system; il secondo parametro deve essere considerato un numero intero X strettamente positivo. Il programma deve cercare nella gerarchia G specificata tutte le directory che contengono almeno un file leggibile con lunghezza in linee pari  X e il cui contenuto del file sia tale per cui tutte le sue linee contengano almeno un carattere numerico. Al termine della ricorsione si riporti il nome assoluto di tali directory sullo standard output e quindi i nomi di tutti i file trovati (F1, F2, … FN).
(Eventualmente facendo uso di un file temporaneo per salvare i dati).

**ATTENZIONE**: il comando `wc -l` conta il  numero di caratteri `\n` che trova in un file, verificare come il proprio editor di testo utilizzato gestisce gli endline dell'ultima riga!

L'editor di testo può:
* inserire autmaticamente e nascondere `\n` al termine dell'ultima riga
* non inserire automaticamente `\n` al termine dell'utlima riga e, in caso di inserimento manuale mostrare una ultreriore riga vuota all'utente (che non sarà considerata dal `wc -l`)

Execute using:
```console
$ ./FCP.sh `pwd`/ger 5
```
