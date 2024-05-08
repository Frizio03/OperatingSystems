#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe

typedef int pipe_t[2];   // Si definisce il tipo pipe_y come array di due interi

int main(){

    //Dichiarazione variabili
    int pid, N = 5;
    int status;
    char buffer[BUFSIZ];
    pipe_t *piped;

    //Allocazione di memoria necessaria per contenere tutti i fd delle pipe
    piped = malloc(N*sizeof(pipe_t));
    if(piped == NULL){
        printf("Errore in malloc!\n");
        exit(1);
    }

    //Creazione delle N pipe
    for(int i = 0; i < N; i++){
        if(pipe(piped[i]) < 0){
            printf("Errore in pipe n. %d\n", i);
            exit(2);
        }
    }

    printf("DEBUG-Inizio generazione figli\n");

    //Creazione degli N figli
    for(int i = 0; i < N; i++){
        
        if((pid = fork()) < 0){
            printf("Errore in fork n. %d", i);
            exit(3);
        }

        if(pid == 0){
            /*Codice del processo figlio*/

            printf("DEBUG-Esecuzione figlio con PID = %d\n", getpid());
            //Chiusura delle pipe inutilizzate
            for(int k = 0; k < N; k++){
                
                //Chiusura pipe di lettura (non utilizzate)
                close(piped[k][0]);

                //Chiusura pipe di scrittura riferite ad altri processi
                if(k != i){
                    close(piped[k][1]);
                }

                //Scrittura dati nel canale di pipe
                write(piped[i][1], "Messaggio", 10);

                //Terminazione corretta del figlio
                exit(0);
            }
        }
    }

    /*Codice del processo padre*/

    //Chiusura pipe di scrittura (non utilizzate)
    for(int k = 0; k < N; k++){
        close(piped[k][1]);
    }

    //Recupero delle informazioni dalla pipe
    for(int i = 0; i < N; i++){
        read(piped[i][0], buffer, BUFSIZ);
        printf("Il figlio con indice %d ha scritto: %s\n", i, buffer);
    }

    //Attesa dei figli
    for(int i = 0; i < N; i++){

        if((pid = wait(&status)) < 0){
            printf("Errore in wait n. %d\n", i);
            exit(4);
        }

        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d ha subito terminazione anomala\n", pid);
        }
        else{
            printf("Figlio con PID = %d ha ritornato %d\n", pid, WEXITSTATUS(status));
        }

    }

    printf("DEBUG-Fine processo padre\n");
    //Terminazione corretta del processo
    exit(0);
}