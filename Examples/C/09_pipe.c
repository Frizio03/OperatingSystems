#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait

int main(){

    //Dichiarazione variabili
    int pid, rwait, status;
    char buffer[BUFSIZ];
    int piped[2];

    //Creazione della pipe e controllo esito
    if(pipe(piped) < 0){
        printf("Piping error!\n");
        exit(1);
    }

    //Creazione del processo figlio e controllo esito
    if((pid = fork()) < 0){
        printf("Fork error!\n");
        exit(2);
    }

    if(pid == 0){
        /*Codice del processo figlio*/

        printf("DEBUG-Iniziato figlio con PID = %d\n", getpid());

        //Chiusura del lato di lettura della pipe
        close(piped[0]);

        write(piped[1], "Testo figlio", 12);

        exit(0);
    }
    
    //Chiusura del lato di scrittura della pipe
    close(piped[1]);

    //Lettura di dati dalla pipe
    read(piped[0], buffer, BUFSIZ);

    printf("DEBUG-Terminato figlio con PID = %d\n", pid);
    printf("Buffer: %s\n", buffer);

    //Eventuale wait del figlio con controlli su status di ritorno

    //Uscita regolare dal processo
    exit(0);
}