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
        
        sleep(2);
        printf("\nEsecuzione del figlio...\n");

        printf("PPID = %d\n", getppid());
        printf("Corrisponde al PID di INIT\n");
        
        printf("Anche eseguendo il comando ps esso viene adottato da INIT\n");
        execl("/usr/bin/ps", "ps", "-l", (char*) 0);

        //Uscita con alore di ritorno
        exit(1);
    }

    //In altri casi eseguo il codie del padre
    printf("Generato figlio con PID = %d\n", pidFiglio);
    printf("Fine processo padre\n");

	//Valore di ritorno del programma eseguito correttamente
    exit(0);
}