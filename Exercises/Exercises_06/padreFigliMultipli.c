#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait

int main(int argc, char** argv){

    //Dichiarazione variabili
    int pid, pidFiglio, status;

    //Controllo sul numero di parametri
    if(argc != 2){
        printf("Errore nel numero di parametri: richiesti 1, passati %d\n", argc-1);
        exit(1);
    }

    //Controllo sul parametro passato
    int N = atoi(argv[1]);
    if(N <= 0 || N >= 255){
        printf("Il valore N = %d passato non risulta compreso fra 0 e 255\n", N);
        exit(2);
    }

    printf("Esecuzione del processo padre con PID = %d ed inizio creazione figli\n", getpid());

    //Creazione dei processi figli
    for(int i = 0; i < N; i++){

        if((pid = fork()) < 0){
            printf("Errore in fork!\n");
            exit(3);
        }

        if(pid == 0){
            //esecuzione codice del figlio
            printf("Processo figlio %d con PID = %d\n", i, getpid());
            exit(i);
        }
    }

    //Attesa dell'esecuzione di tutti gli N figli
    for(int j = 0; j < N; j++){
        
        //Controllo sulla wait
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait\n");
            exit(4);
        }

        //Controllo sulla terminazione del figlio
        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else{
            printf("Figlio con PID = %d terminato con valore di uscita %d\n", pidFiglio, WEXITSTATUS(status));
        }
    }

    //Uscita senza errori
    exit(0);
}