## Testo di esame parte C
*30 Maggio 2024*

La parte in C accetta un numero fisso di due parametri (da controllare) che rappresentano il primo un numero **Z** intero strettamente positivo (da controllare) e il secondo il nome di un file relativo semplice (da NON controllare) che andrà creato (o sovrascritto in caso di sua esistenza).

Il processo padre deve, per prima cosa, creare nella directory corrente un file con nome corrispondente al secondo parametro.

Quindi, il processo padre deve generare **Z** processi figli: ogni processo figlio **Pz** deve creare a sua volta un processo nipote **PPz**; i processi figli e nipoti eseguono concorrentemente.

Il compito di ogni processo nipote **PPz** è quello di ricavare l’elenco semplice dei processi associati alla sessione interattiva corrente, usando in modo opportuno il comando `ps` di UNIX/Linux.

Ogni processo figlio **Pz**, a sua volta, deve - filtrando quanto ricevuto dal proprio nipote **PPz** - selezionare la linea del comando `ps` che corrisponde al PID del processo figlio, usando in modo opportuno il comando-filtro `grep` di UNIX/Linux. Tale linea selezionata deve contestualmente essere inviata al padre.

Il padre deve ricevere, rispettando l'ordine dei figli, ogni singola linea inviata dai figli; al termine della lettura di ogni singola linea, il padre deve scrivere tale linea (insieme con il terminatore di linea) sul file precedentemente creato.

Al termine, ogni processo figlio Pn torna al padre il risultato del comando-filtro grep eseguito e il padre deve stampare su standard output il PID di ogni figlio e il valore ritornato. Si inserisca un opportuno controllo nel caso questo valore non rappresenti il valore di successo del comando-filtro `grep`!

Ogni coppia costituita da un nipote e da un figlio realizza un piping di comandi, che sono quelli indicati nel testo. Rispetto ad un semplice piping di due comandi, in più ogni figlio si deve comportare in modo analogo al proprio nipote per quanto riguarda lo standard output, ma per mandare le informazioni al padre che agisce a livello di programmazione recuperando in modo opportuno quanto scritto da ogni figlio!

>[!NOTE]
> * La variabile buffer, array di char usato per leggere le singole linee, si può considerare di dimensione 250
> * Poiché la specifica indica “i processi figli e nipoti eseguono concorrentemente”, risulta opportuno che i figli NON ASPETTINO i nipoti

>[!WARNING]
>In alcuni testi di esame la grep doveva essere effettauta sul pid dei nipoti.

### NOTA BENE NEL FILE C main.c SI USI OBBLIGATORIAMENTE:
- una variabile di nome **Z** per il numero corrispondente al primo parametro;
- una variabile di nome **CreatedFile** per il file descriptor usato dal padre per la creazione del file;
- una variabile di nome **z** per l’indice dei processi figli;
- una variabile di nome **Buff** per leggere, da parte del padre, via via le singole linee inviate dai figli.

Execute using:
```console
$ make
$ ./main 3 file.log
```