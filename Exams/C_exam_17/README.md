## Testo di esame parte C
*08 Settembre 2021*

La parte in C accetta un numero variabile di parametri **N** maggiore o uguale a 1 che rappresentano **N** nomi di file (**F1, …FN**). Il processo padre deve, per prima cosa, creare nella directory corrente un file **fcreato** e terminazione `.log`.

Il processo padre deve generare un numero di processi figli pari a **2N**: i processi figli devono essere considerati a coppie e ogni coppia è costituita dai processi **Pi(2)** (detti processi pari) e **Pi(2+1)** (detti processi dispari) (con i che varia da 0 a N-1).

Ogni coppia così determinata è associata ad uno dei file F1, …FN (in ordine): quindi ogni processo pari **Pn** (cioè con indice n pari) identificherà il file associato come **Fn/2+1**, mentre ogni processo dispari **Pn** (cioè con indice n dispari) identificherà il file associato come **F(n+1)/2**.

Ogni processo pari è associato alle linee pari del proprio file e ogni processo dispari è associato alle linee dispari del proprio file.

>[!NOTE]
>Si consideri che la prima linea dei file abbia numero 1 e quindi sia dispari!

Ognuno di tali processi figli **Pn** esegue concorrentemente e legge tutte le linee del proprio file associato: per ogni linea dispari, ogni processo figlio dispari **Pn** calcola la lunghezza della linea corrente compreso il terminatore di linea e la invia al padre e per ogni linea pari, ogni processo figlio pari **Pn** calcola la lunghezza della linea corrente compreso il terminatore di linea e la invia al padre.

Il padre deve ricevere per ogni file **F1, …FN** da ognuna delle coppie tutte le lunghezze delle linee dispari/pari e le deve stampare su standard output in ordine di linea, come mostrato nell’esempio (riportato di seguito).

Al termine dell’esecuzione, ogni figlio **Pn** ritorna al padre il massimo delle lunghezze calcolate (sicuramente minore di 255); il padre deve scrivere sul file fcreato il PID di ogni figlio e il valore ritornato.

>[!NOTE]
>Si supponga che una linea abbia lunghezza massima di 250 caratteri compreso il terminatore.

### OUTPUT DEL PADRE

Le lunghezze delle linee del file f1.txt sono:
linea numero 1 lunga 41
linea numero 2 lunga 42
Le lunghezze delle linee del file f2.txt sono:
linea numero 1 lunga 43
linea numero 2 lunga 30
linea numero 3 lunga 38

### CONTENUTO FINALE DEL FILE CON TERMINAZIONE log

Figlio con pid=105897 ha ritornato 42 (255 = errore)
Figlio con pid=105898 ha ritornato 41 (255 = errore)
Figlio con pid=105899 ha ritornato 30 (255 = errore)
Figlio con pid=105900 ha ritornato 43 (255 = errore)

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di file;
- una variabile di nome **n** per l’indice dei processi figli;
- una variabile di nome **linea** per la linea corrente (pari/dispari) letta dai figli dal proprio file;
- una variabile di nome **nro** per il valore massimo della lunghezza delle linee pari/dispari dei file;
- una variabile di nome **fcreato** per il file descriptor del file creato dal padre.

Execute using:
```console
$ make
$ ./main f1.txt f2.txt
```