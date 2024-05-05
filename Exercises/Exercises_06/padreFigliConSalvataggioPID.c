#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <time.h>       // Per utilizzo del random

int main(int argc, char** argv){

    //Inizializzazione random
    srand(time(NULL));

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

    //Allocazione array per salvare i PID;
    int* arrayPid = malloc(N*sizeof(int));

    //Controllo sull'esito della malloc
    if(arrayPid == NULL){
        printf("Errore di allocazione nella malloc\n");
        exit(3);
    }

    //Creazione dei processi figli
    for(int i = 0; i < N; i++){

        if((arrayPid[i] = fork()) < 0){
            printf("Errore in fork!\n");
            exit(3);
        }

        if(arrayPid[i] == 0){
            //esecuzione codice del figlio
            printf("Processo figlio %d con PID = %d\n", i, getpid());

            //Valore random da ritornare nella exit
            int rndval = rand() % (100 + i);

            exit(rndval);
        }
    }

    //Attesa dell'esecuzione di tutti gli N figli
    for(int i = 0; i < N; i++){
        
        //Controllo sulla wait
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait: %d\n", pidFiglio);
            exit(4);
        }

        //Controllo sulla terminazione del figlio
        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else{
            
            for(int j = 0; j < N; j++){
                
                if(arrayPid[j] == pidFiglio){
                    printf("Figlio con indice i = %d e PID = %d terminato con valore di uscita %d\n", j, pidFiglio, WEXITSTATUS(status));
                    break;
                }
            }
        }
    }

    //Uscita senza errori
    exit(0);
}