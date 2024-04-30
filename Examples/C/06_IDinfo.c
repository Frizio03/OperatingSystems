#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait

int main(){

    //Variabili per contenere PID
    int pidFiglio, rwait;

    //Variabile per contenere il valore ritornato dal figlio
    int status, returnValue;

    //Ricavo e stampo i dati del processo padre
    printf("* Father real-UID: %d\n", getuid());
    printf("* Father effective-UID: %d\n", geteuid());
    printf("* Father real-GID: %d\n", getgid());
    printf("* Father effective-GID: %d\n", getegid());
    printf("* Father PID: %d\n", getpid());
    printf("* Father PPID: %d\n", getppid());

    //Controllo su fork fallita
    if((pidFiglio = fork()) < 0){
        printf("Errore in fork\n");
        exit(1);
    }

    //Se pid vale 0 allora eseguo il codice del figlio
    if(pidFiglio == 0){

        //Ricavo e stampo i dati del processo figlio
        printf("> Child real-UID: %d\n", getuid());
        printf("> Child effective-UID: %d\n", geteuid());
        printf("> Child real-GID: %d\n", getgid());
        printf("> Child effective-GID: %d\n", getegid());
        printf("> Child PID: %d\n", getpid());
        printf("> Child PPID: %d\n", getppid());
        exit(-1);
    }

    //In altri casi eseguo il codie del padre
    printf("Generato figlio con PID = %d\n", pidFiglio);

    //Attesa dell'esecuzione del figlio
    if((rwait = wait(&status)) < 0){
        printf("Errore nella wait\n");
        exit(2);
    }

    if(pidFiglio == rwait){
        printf("Figlio con PID %d terminato correttamente\n", rwait);
    }
    else{
        printf("Errore: pid della wait non corrisponde al pid della fork\n");
        exit(3);
    }

    //Controllo sul byte basso: uscite anomale
    if((status & 0xFF) != 0){
        printf("Figlio terminato in modo involontario/anomalo\n");
        printf("Valore del byte basso: %d\n", status & 0XFF);
    }
    else{
        
        //Selezione manuale del byte alto ed estrazione valore di ritorno
        returnValue = status >> 8;
        returnValue &= 0xFF;

        //Stampa del valore di ritorno ottenuto manualmente
        printf("Figlio con PID %d valore di exit: %d\n", pidFiglio, returnValue);
    }

	//Valore di ritorno del programma eseguito correttamente
    exit(0);
}