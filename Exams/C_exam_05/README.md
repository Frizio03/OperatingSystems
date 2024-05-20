## Testo di esame parte C
*15 Giugno 2022*

La parte in C accetta un numero variabile di parametri **N** (con **N** maggiore o uguale a 2) che rappresentano nomi di file (**F1, …FN**). Il processo padre deve generare **N** processi figli (**P0 … PN-1**): i processi figli **Pi** (con i variabile da **0** a **N-1**) sono associati agli **N** file **Ff** (con **f = i+1**).

Ogni processo figlio **Pi** deve leggere tutti i caratteri del file associato **Ff** e appena viene trovata una occorrenza di un carattere alfabetico minuscolo, il processo figlio **Pi** deve comunicare al padre tale carattere e deve ricevere dal padre l’indicazione di stampare o meno su standard output delle informazioni (vedi dopo *).

Il padre deve ricevere, rispettando l’ordine dei file **Ff**, da ogni figlio via via i caratteri di tipo alfabetico minuscolo letti dai figli. Quindi, al processo padre deve arrivare un insieme di char (che via via potrebbe diminuire in quantità in dipendenza della terminazione dei figli): sicuramente il primo insieme (questo viene garantito dalla parte Shell) è costituito dal primo carattere alfabetico minuscolo inviato dal figlio **P0**, dal primo carattere alfabetico minuscolo inviato dal figlio **P1**, etc., dal primo carattere alfabetico minuscolo inviato dal figlio **PN-1**.

Per ogni insieme ricevuto, il padre deve determinare il valore massimo e, **SOLO AL PROCESSO FIGLIO CHE HA INVIATO TALE CARATTERE**, deve indicare (*) di stampare su standard output il carattere trovato, assieme all’indice d’ordine del processo, il suo pid, la posizione all’interno del file e il nome del file associato, mentre a tutti gli altri processi figli deve indicare di non stampare.

Al termine, ogni processo figlio **Pi** deve ritornare al padre il numero di stampe effettuate (supposto strettamente minore di 255); il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **N** per il numero di file;
- una variabile di nome **i** per l’indice dei processi figli;
- una variabile di nome **ch** per il carattere letto dai file dai figli;
- una variabile di nome **cur** per la posizione all’interno del file (di tipo long int).