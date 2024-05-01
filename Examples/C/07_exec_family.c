#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait

int main(){

    //Variabili per contenere PID
    int pidFiglio, rwait;

    //Variabile per contenere il valore ritornato dal figlio
    int status, returnValue;

    printf("Esecuzione processo padre con PID = %d\n", getpid());

    //Controllo su fork fallita
    if((pidFiglio = fork()) < 0){
        printf("Errore in fork\n");
        exit(1);
    }

    //Se pid vale 0 allora eseguo il codice del figlio
    if(pidFiglio == 0){
        
        //Argomenti da passare alla exec
        char* argv[2] = {"ls", "-al"};

        //Esecuzione della exec passando ciascun parametro
        execl("/usr/bin/ls", argv[0], argv[1], (char*) 0);

        //Uscita con errore solamente se exec fallisce
        exit(1);
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