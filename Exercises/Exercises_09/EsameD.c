#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#include <ctype.h>      //utilizzo di islower etc.

//definizione del tipo pipe_t
typedef int pipe_t[2];

int main(int argc, char** argv){

    //Dichiarazione variabili
    int pid, pidFiglio;
    int status, ritorno;
    int fd;
    long int trasformazioni;
    char ch, newch;

    //Controllo sul numero di parametri
    if(argc < 3){
        printf("Parametri richiesti: >= 2, passati: %d\n", argc-1);
        exit(1);
    }

    //Determinazione del valore N
    int N = argc-1;

    //Allocazione degli array di pipe padre-figli e padre-nipoti
    pipe_t* pf = malloc(N*sizeof(pipe_t));
    pipe_t* pn = malloc(N*sizeof(pipe_t));

    if(pf == NULL || pn == NULL){
        printf("Errore in malloc\n");
        exit(2);
    }

    //Generazione delle pipe
    for(int i = 0; i < N; i++){
        if(pipe(pf[i]) < 0){
            printf("Errore in pipe\n");
            exit(3);
        }
        
        if(pipe(pn[i]) < 0){
            printf("Errore in pipe\n");
            exit(4);
        }
    }

    printf("DEBUG-Processo padre con PID = %d, inizio generazione figli\n", getpid());

    //Generazione dei figli
    for(int i = 0; i < N; i++){

        //Fork e controllo eventuali errori
        if((pid = fork()) < 0){
            printf("Errore in fork\n");
            exit(5);
        }

        if(pid == 0){
            /*Codice del processo figlio*/
            printf("DEBUG-Figlio con PID = %d\n", getpid());

            //Chiusura delle pipe padre-figlio non necessarie
            for(int k = 0; k < N; k++){
                close(pf[k][0]);
                if(k != i)
                    close(pf[k][1]);
            }

            //Generazione del nipote
            if((pid = fork()) < 0){
                printf("Errore in fork figlio\n");
                exit(-1);
            }

            if(pid == 0){
                /*Codice processo nipote*/

                //Chiusura della pipe padre-figlio rimasta
                close(pf[i][1]);

                //Chiusura delle pipe inutilizzate
                for(int k = 0; k < N; k++){
                    close(pn[k][0]);
                    if(k != i)
                        close(pn[k][1]);
                }

                //Apertura del file (nipote)
                if((fd = open(argv[i+1], O_RDWR)) < 0){
                    printf("Errore nell'apertura di '%s'\n", argv[i+1]);
                    exit(-1);
                }

                //Lettura ed eventuale sostituzione
                trasformazioni = 0L;
                while(read(fd, &ch, 1) != 0){
                    if(islower(ch)){
                        lseek(fd, -1L, SEEK_CUR);
                        newch = toupper(ch);
                        write(fd, &newch, 1);
                        trasformazioni++;
                    }
                }
                
                //Comunicazione con il padre utilizzando la pipe
                write(pn[i][1], &trasformazioni, sizeof(trasformazioni));

                exit(trasformazioni/256);
            }

            /*Codice processo figlio*/

            //Chiusura delle pipe padre-nipote
            for(int i = 0; i < N; i++){
                close(pn[i][0]);
                close(pn[i][1]);
            }

            //Apertura del file (figlio)
            if((fd = open(argv[i+1], O_RDWR)) < 0){
                printf("Errore nell'apertura di '%s'\n", argv[i+1]);
                exit(-1);
            }

            //Lettura ed eventuale sostituzione
            trasformazioni = 0L;
            while(read(fd, &ch, 1) != 0){
                if(isdigit(ch)){
                    lseek(fd, -1L, SEEK_CUR);
                    newch = ' ';
                    write(fd, &newch, 1);
                    trasformazioni++;
                }
            }

            //Comunicazione con il padre utilizzando la pipe
            write(pf[i][1], &trasformazioni, sizeof(trasformazioni));

            //Attesa del processo nipote
            if((pidFiglio = wait(&status)) < 0){
                printf("Errore in wait eseguita dal figlio\n");
            }

            if(WIFEXITED(status) == 0){
                printf("Nipote con PID = %d terminato in modo anomalo\n", pidFiglio);
            }
            else{
                ritorno = WEXITSTATUS(status);
                printf("Nipote con PID = %d ha ritornato %d\n", pidFiglio, ritorno);
            }

            exit(trasformazioni/256);
        }
    }

    /*Codice del processo padre*/

    //Chiusura di tutte le pipe di scrittura
    for(int i = 0; i < N; i++){
        close(pf[i][1]);
        close(pn[i][1]);
    }

    //Lettura ordinata dei dati dalle pipe
    for(int i = 0; i < N; i++){
        read(pf[i][0], &trasformazioni, sizeof(trasformazioni));
        printf("Il figlio di indice i = %d ha operato %ld trasformazioni su '%s'\n", i, trasformazioni, argv[i+1]);
        read(pn[i][0], &trasformazioni, sizeof(trasformazioni));
        printf("Il nipote di indice i = %d ha operato %ld trasformazioni su '%s'\n", i, trasformazioni, argv[i+1]);
    }

    //Attesa degli N figli
    for(int i = 0; i < N; i++){
    
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait eseguita dal figlio\n");
            exit(6);
        }

        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else{
            ritorno = WEXITSTATUS(status);
            printf("Figlio con PID = %d ha ritornato %d\n", pidFiglio, ritorno);
        }
    }

    //Uscita corretta dal programma
    exit(0);
}