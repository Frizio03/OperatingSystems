#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe

int main(int argc, char** argv){

    //dichiarazioni iniziali
    int pid, status, fd;

    //Controllo delnumero di parametri
    if(argc != 2){
        printf("Parametri richiesti: 1, inseriti: %d\n", argc-1);
        exit(1);
    }

    //Creazione del processo figlio
    if((pid = fork()) < 0){
        printf("Errore in fork\n");
        exit(2);
    }

    if(pid == 0){
        /*Codice del prtocesso figlio*/

        //Implementazione ridirezione stdin
        close(0);
        if((fd = open(argv[1], O_RDONLY)) < 0){
            printf("Errore nell'apertura del file %s\n", argv[2]);
            exit(-1);
        }

        execlp("cat", "cat", argv[1], NULL);

        //In caso di fallimento della exec
        printf("Errore in exec\n");
        exit(-1);
    }

    //Attesa del figlio
    if((pid = wait(&status)) < 0){
        printf("Errore in wait\n");
        exit(3);
    }

    //Controllo valore di ritorno del figlio
    if(WIFEXITED(status) == 0){
        printf("\nFiglio con PID = %d terminato in modo anomalo\n", pid);
    }
    else{
        printf("\nFiglio con PID = %d terminato correttamente con valore: %d (se 255 problemi)\n", pid, WEXITSTATUS(status));
    }

    //Terminazione corretta del processo
    exit(0);
}