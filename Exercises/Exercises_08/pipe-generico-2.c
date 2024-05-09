#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#define MAXLEN 512

int main(int argc, char** argv){

    //Dichiarazione variabili
    int fd, nread, len;
    int pid, pidFiglio;
    int status, ritorno;
    int piped[2];
    char buffer[MAXLEN];

    //Controllo sul numero di parametri
    if(argc != 2){
        printf("Parametri richiesti: 1, passati: %d\n", argc-1);
        exit(1);
    }

    if(pipe(piped) < 0){
        printf("Errore in pipe\n");
        exit(2);
    }

    if((pid = fork()) < 0){
        printf("Errore in fork\n");
        exit(3);
    }

    if(pid == 0){
        /*Codice del processo figlio*/

        //Chiusura del canale di pipe inutilizzato dal figlio
        close(piped[0]);

        //Apertura del file passato e controllo fd
        if((fd = open(argv[1], O_RDONLY)) < 0){
            printf("Errore nell'apertura del file '%s'\n", argv[1]);
            exit(-1);
        }

        len = 0;

        //Leggo ciascuna linea del file
        while(read(fd, &(buffer[len]), 1) != 0){
            len++;

            //Quando incontro il carattere \n...
            if(buffer[len-1] == '\n'){
                
                buffer[len] = '\0';
                len++;

                //Scrivo la linea sulla pipe
                write(piped[1], buffer, len);

                //Reset del contatore
                len = 0;
            }
        }

        exit(0);
    }

    /*Codice del processo padre*/

    //Chiusura del canale di pipe inutilizzato dal padre
    close(piped[1]);

    //Lettura della linea dalla pipe e stampa su stdout
    char c;
    while(read(piped[0], &c, 1) != 0){
        if(c != 0){
            printf("%c", c);
        }
    }

    //Attesa del processo figlio
    if((pidFiglio = wait(&status)) < 0){
        printf("Errore in wait\n");
        exit(4);
    }

    if(pid != pidFiglio){
        printf("Il pid ritornato dalla wait non coincide con quello della fork!\n");
        exit(5);
    }

    if(WIFEXITED(status) == 0){
        printf("Figlio con PID = %d terminato in modo anomalo.\n", pidFiglio);
    }
    else{
        ritorno = WEXITSTATUS(status);
        printf("Figlio con PID = %d ha ritornato %d (se 255 problemi!)\n", pidFiglio, ritorno);
    }

    //Terminazione corretta del processo
    exit(0);
}