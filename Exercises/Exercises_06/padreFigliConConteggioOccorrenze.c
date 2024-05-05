#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#define PERM 0644       // Permessi di default in ottale

int main(int argc, char** argv){

    //Dichiarazione variabili
    int pid, pidFiglio, status, fd;

    //Controllo sul numero di parametri
    if(argc < 4){
        printf("Errore nel numero di parametri: richiesti >= 4, passati %d\n", argc-1);
        printf("Invocazione corretta: f1 f2 f3 ... Cx\n");
        exit(1);
    }

    //Controllo sull'ultimo parametro
    if(strlen(argv[argc-1]) != 1){
        printf("Errore: ultimo parametro non singolo carattere\n");
        exit(2);
    }

    //Salvataggio informazioni
    char Cx = argv[argc-1][0];
    int N = argc-2;

    printf("DEBUG-Esecuzione del processo padre con PID = %d\n", getpid());
    printf("DEBUG-Inizio generazione figli che cerheranno le occorrenze di Cx nei file\n");

    //Creazione dei processi figli
    for(int i = 0; i < N; i++){

        if((pid = fork()) < 0){
            printf("Errore in fork!\n");
            exit(3);
        }

        if(pid == 0){
            
            //Esecuzione codice del figlio --> ricerca occorrenze Cx
            int occorrenze = 0;
            char c;

            printf("DEBUG-Processo figlio %d con PID = %d\n", i, getpid());
            
            //Tentativo di apertura del file
            if((fd = open(argv[i+1], O_RDONLY)) < 0){
                printf("Errore: file %s inesistente o non leggibile!\n", argv[i+1]);
                exit(-1);
            }

            //Leggo ciascun carattere del file
            while(read(fd, &c, 1) != 0){
                //Se trovo un'occorrenza incremento il contatore
                if(c == Cx){
                    occorrenze++;
                }
            }

            //Fornisco come uscita il numero delle occorrenze
            exit(occorrenze);
        }
    }

    //Attesa dell'esecuzione di tutti gli N figli
    for(int j = 0; j < N; j++){
        
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
            printf("Figlio con PID = %d terminato con valore di uscita %d. (255 = errori)\n", pidFiglio, WEXITSTATUS(status));
        }
    }

    //Uscita senza errori
    exit(0);
}