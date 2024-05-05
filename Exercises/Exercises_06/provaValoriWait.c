#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait

int main(){

    //Dichiarazione variabili
    int pid, pidFiglio;
    int status, ritorno;

    if((pid = fork()) < 0){
        printf("Errore in fork\n");
        exit(1);
    }

    if(pid == 0){
        //Esecuzione codice del figlio
        printf("Esecuzione del figlio\n");
        
        int val;
        printf("Inserire un valore numerico: ");
        scanf("%d", &val);
        
        if(val > 255 || val < 0){
            printf("Attenzione: il valore sarà troncato!\n");
        }
        
        exit(val);
    }

    //Esecuzione codice del padre
    printf("Generato figlio con PID = %d\n", pid);

    //Attesa dell'esecuzione del figlio
    if((pidFiglio = wait(&status)) < 0){
        printf("Errore in wait\n");
        exit(2);
    }

    //Controllo sul risultato della wait
    if(pid == pidFiglio){
        printf("Terminato figlio con PID = %d\n", pidFiglio);
    }
    else{
        printf("Valore di ritorno della wait non corrisponde al PID della fork\n");
        exit(3);
    }

    //Ricavo il valore di ritorno del figlio
    if(WIFEXITED(status) == 0){
        printf("Figlio terminato in modo anomalo\n");
    }
    else{
        printf("Valore di ritorno del figlio: %d\n", WEXITSTATUS(status));
    }

    //Uscita senza errori
    exit(0);
}