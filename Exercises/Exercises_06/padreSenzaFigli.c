#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait

int main(){

    //Dichiarazione variabili
    int rwait, status;

    //Stampa del PID del processo corrente (padre)
    printf("Stampo il PID del processo corrente (padre): %d\n", getpid());

    if((rwait = wait(&status)) < 0){
        printf("Errore in wait con rwait = %d\n", rwait);
        exit(1);
    }

    //Uscita senza errori
    exit(0);
}