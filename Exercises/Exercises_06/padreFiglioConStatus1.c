#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <time.h>       // Per utilizzo random

int main(){

    //Inizializzazione del seme random
    srand(time(NULL));

    //Dichiarazione variabili
    int pid, rwait, status;

    //Stampa del PID del processo corrente (padre)
    printf("Stampo il PID del processo corrente (padre): %d\n", getpid());

    //Tento di creare un processo figlio con la ork e controllo il risultato
    if((pid = fork()) < 0){
        printf("Errore in fork\n");
        exit(1);
    }

    //Se fd == 0 allora eseguo il codice del figlio
    if(pid == 0){

        printf("Esecuzione del processo figlio:\n");
        printf("Stampo il PID del processo padre: %d\n", getppid());
        printf("Stampo il PID del processo corrente (figlio): %d\n", getpid());

        int exitvalue = rand() % 99;

        exit(exitvalue);
    }

    //Altrimenti eseguo il codice del padre

    //Attesa dell'esecuzione del figlio
    if((rwait = wait(&status)) < 0){
        printf("Errore nella PRIMA wait\n");
        exit(2);
    }

    if(pid == rwait){
        printf("Terminato figlio con PID = %d\n", pid);
    }
    else{
        printf("Il pid della wait non corrisponde al pid della fork\n");
        exit(3);
    }

    //Ricavo il valore di ritorno del figlio
    if(WIFEXITED(status) == 0){
        printf("Figlio terminato in modo anomalo\n");
        exit(3);
    }
    printf("Valore di ritorno del figlio: %d\n", WEXITSTATUS(status));

    //Wait eseguita su nessun figlio
    if((rwait = wait(&status)) < 0){
        printf("Errore nella SECONDA wait\n");
        exit(4);
    }

    //Uscita dal programma senza errori
    exit(0);
}