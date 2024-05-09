#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe

int main(int argc, char** argv){

    //dichiarazioni iniziali
    int pid, status;

    //Controllo delnumero di parametri
    if(argc != 3){
        printf("Parametri richiesti: 2, inseriti: %d\n", argc-1);
        exit(1);
    }

    //Creazione del processo figlio
    if((pid = fork()) < 0){
        printf("Errore in fork\n");
        exit(2);
    }

    if(pid == 0){
        /*Codice del processo figlio*/

        //Implementazione ridirezione stdout e stderr
        close(1);
        open("/dev/null", O_WRONLY);
        close(2);
        open("/dev/null", O_WRONLY);

        execlp("grep", "grep", argv[1], argv[2], NULL);

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
        printf("Figlio con PID = %d terminato in modo anomalo\n", pid);
    }
    else{
        printf("Figlio con PID = %d terminato correttamente con valore: %d (da exec-grep)\n", pid, WEXITSTATUS(status));
    }

    //Terminazione corretta del processo
    exit(0);
}