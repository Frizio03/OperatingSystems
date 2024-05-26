# Testi degli esami relativi all'esercitazione sui segnali

## Esame A

Scrivere un programma C `EsameA.c` che risolva la parte C dell’Esame del 8 Giugno 2016 che si riporta qui di seguito per comodità.

La parte in C accetta un numero variabile **N+1** di parametri (con **N** maggiore o uguale a 4) che rappresentano i primi **N** nomi di file (**F1...FN**), mentre l’ultimo rappresenta un numero intero (**H**) strettamente positivo e minore di 255 (da controllare) che indica la lunghezza in linee dei file: infatti, la lunghezza in linee dei file è la stessa.

Il processo padre deve, per prima cosa, inizializzare il seme per la generazione random di numeri e deve creare un file **fcreato** di nome `files/fcreato`.
Il processo padre deve, quindi, generare **N** processi figli (**P0…PN-1**): i processi figli **Pi** (con i che varia da 0 a N-1) sono associati agli **N** file **Fk** (con **k = i+1**).

Ogni processo figlio **Pi** deve leggere le linee del file associato **Fk** sempre fino alla fine. I processi figli **Pi** e il processo padre devono attenersi a questo schema di comunicazione: per ogni linea letta, il figlio **Pi** deve comunicare al padre la lunghezza della linea corrente compreso il terminatore di linea (come int); il padre usando in modo opportuno la funzione `mia_random()` (riportata nel seguito) deve individuare in modo random quale lunghezza (come int) considerare fra quelle ricevute, rispettando l'ordine dei file. Individuata questa lunghezza, usando sempre in modo opportuno la funzione `mia_random()` deve individuare un intero che rappresenterà un indice per la linea della lunghezza considerata; il padre deve comunicare indietro a tutti i figli **Pi** tale indice.

Ogni figlio **Pi** ricevuto l’indice (per ogni linea) deve controllare se è ammissibile per la linea corrente e in tal caso deve scrivere il carattere della linea corrente, corrispondente a tale indice, sul file **Fcreato**, altrimenti non deve fare nulla e deve passare a leggere la linea successiva.

Al termine, ogni processo figlio Pi deve ritornare al padre il valore intero corrispondente al numero di caratteri scritti sul file Fcreato (sicuramente minore di 255); il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.

```C
    int mia_random(int n)
    {
    int casuale;
    casuale = rand() % n;
    return casuale;
    }
```

## Esame B

Con un editor, scrivere un programma C `EsameB.c` che risolva la parte C dell’Esame del 9 Settembre 2015 che si riporta qui di seguito per comodità.

La parte in C accetta un numero variabile **N+1** di parametri (con **N** maggiore o uguale a 2) che rappresentano **N+1** nomi di file (**F1...FN** e **AF**) con uguale lunghezza in byte (da non controllare).

Il processo padre è associato al file **AF** e deve generare **N** processi figli (**P0…PN-1**) ognuno dei quali è associato ad uno dei file **F1...FN**. Ogni processo figlio **Pi** deve leggere i caratteri del file associato sempre fino alla fine solo dopo aver ricevuto l’indicazione dal padre di procedere.

Infatti, i processi figli devono attenersi a questo schema di comunicazione/sincronizzazione con il padre: il figlio P0, ricevuta l’indicazione dal padre che può procedere, legge il primo carattere e lo comunica al padre che lo confronta con il primo carattere del file AF; il figlio P1, ricevuta l’indicazione dal padre che può procedere, legge il primo carattere e lo comunica al padre che lo confronta con il primo carattere del file AF etc. fino al figlio PN-1, ricevuta l’indicazione dal padre che può procedere, legge il primo carattere e lo comunica al padre che lo confronta con il primo carattere del file AF.

Questo schema deve continuare per gli altri caratteri solo se il confronto ha successo (cioè i due caratteri sono uguali); se un confronto non ha successo, il padre non deve inviare al corrispondente figlio l’indicazione di procedere. Una volta che il padre termina la lettura del file associato **AF**, deve terminare forzatamente (con un apposito segnale) i figli per i quali il confronto non abbia avuto successo.

Al termine, i processi figli **Pi** che non sono stati terminati forzatamente devono ritornare al padre l’indicazione di successo secondo la semantica di UNIX; il padre deve stampare su standard output il PID di ogni figlio con l’indicazione di terminazione anormale o normale e, in questo caso, il valore ritornato dal figlio insieme con il nome del file il cui contenuto risulta uguale a quello del file **AF**.

## Esame C

Con un editor, scrivere un programma in C `EsameC.c` che risolva la parte C dell’Esame del 14 Giugno 2017 che si riporta qui di seguito per comodità.

La parte in C accetta un numero variabile **N+1** di parametri (con **N** maggiore o uguale a 1, da controllare) che rappresentano i primi **N** nomi assoluti di file (**F1, F2, ... FN**) mentre l’ultimo rappresenta un singolo carattere (**Cx**) (da controllare): si può ipotizzare che la lunghezza di tutti i file sia uguale (senza verificarlo).

Il processo padre deve generare **N** processi figli (**P0, P1, … PN-1**): i processi figli **Pi** (con i che varia da 0 a N-1) sono associati agli **N** file **FK** (con **K=i+1**).

Ogni processo figlio **Pi** deve leggere i caratteri del file associato **FK** cercando le occorrenze del carattere **Cx**, sostituendole eventualmente con i caratteri inviati dal padre. Ogni figlio **Pi**, per ogni occorrenza trovata, deve comunicare al padre la posizione (in termini di long int) di tale occorrenza a partire dall’inizio del file (si assuma che il primo carattere abbia posizione 1).

Il padre deve ricevere le posizioni (come long int) inviate dai figli nel seguente ordine: prima deve ricevere dal figlio P0 la prima posizione inviata, poi deve ricevere dal figlio P1 la prima posizione inviata e così via fino a che deve ricevere dal figlio PN-1 la prima posizione inviata; quindi, deve procedere a ricevere le seconde posizioni inviate dai figli (se esistono) e così via.

La ricezione di posizioni da parte del padre deve terminare quando ha ricevuto tutte le posizioni inviate da tutti i figli **Pi**. Per ogni posizione ricevuta, il padre deve riportare sullo standard output: l’indice del figlio che gli ha inviato la posizione, il nome del file in cui è stata trovata l’occorrenza del carattere **Cx** e (naturalmente) la posizione ricevuta.

Quindi, per ogni posizione ricevuta, il padre deve chiedere all’utente il carattere con cui deve essere sostituita la specifica occorrenza; nel caso l’utente inserisca una linea vuota, questo deve essere interpretato dal padre come indicazione di non sostituire l’occorrenza corrente. Il padre, per ogni posizione, deve comunicare al figlio corrispondente o il carattere da sostituire oppure se può proseguire con la ricerca di altre occorrenze del carattere **Cx**.

Al termine, ogni processo figlio Pi deve ritornare al padre il numero di sostituzioni effettuate nel proprio file (supposto strettamente minore di 255); il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato.