#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe

#define PERM 0644

int main(int argc, char** argv){

    //dichiarazioni iniziali
    int pid, status, fd, pidFiglio;
    char* fout;

    //Controllo delnumero di parametri
    if(argc < 4){
        printf("Parametri richiesti: >= 3, inseriti: %d\n", argc-1);
        exit(1);
    }

    //Creazione dei processi figli
    int N = argc - 1; 
    for(int i = 0; i < N; i++){
        
        if((pid = fork()) < 0){
            printf("Errore in fork n. %d\n", i);
            exit(2);
        }

        if(pid == 0){
            /*Codice del processo figlio*/
            printf("DEBUG-Esecuzione figlio con PID = %d\n", getpid());

            //Allocazione memoria stringa file finale
            fout = calloc(strlen(argv[i+1]) + 6, sizeof(char));
            if(fout == NULL){
                printf("Errore in malloc\n");
                exit(-1);
            }

            //Creazione del nome del file finale
            strcpy(fout, argv[i+1]);
            strcat(fout, ".sort");

            if((fd = creat(fout, PERM)) < 0){
                printf("Impossibile creare il file %s per il figlio %d\n", fout, i);
                exit(-1);
            }
            //Chiusura del file creato inutilizzato dal figlio
            close(fd);

            //Creazione processo nipote
            if((pid = fork()) < 0){
                printf("Errore in fork n. %d\n", i);
                exit(-1);
            }

            if(pid == 0){
                /*Codice processo nipote*/
                printf("DEBUG-Esecuzione nipote con PID = %d\n", getpid());

                //Ridirezione standard input
                close(0);
                if(open(argv[i+1], O_RDONLY) < 0){
                    printf("Errore in apertura di %s\n", argv[i+1]);
                    exit(-1);
                }

                //Ridirezione standard output
                close(1);
                if(open(fout, O_WRONLY) < 0){
                    printf("Errore nell'arpertura di %s\n", argv[i+1]);
                    exit(-1);
                }

                execlp("sort", "sort", NULL);

                //Eventuali errori in exec;
                printf("Errore in exec\n");
                exit(-1);
            }

            //Attesa del nipote e controllo valori di ritorno
            if((pidFiglio = wait(&status)) < 0){
                printf("Errore in wait del nipote\n");
                exit(-1);
            }

            if(WIFEXITED(status) == 0){
                printf("Nipote con PID = %d terminato in modo anomalo\n", pid);
            }
            else{
                printf("Nipote con PID = %d terminato correttamente con valore: %d (se 255 problemi)\n", pid, WEXITSTATUS(status));
            }

            //Terminazione corretta del figlio
            exit(WEXITSTATUS(status));
        }
    }

    //Attesa dei figli e controllo valori di ritorno
    for(int i = 0; i < N; i++){
        
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait\n");
            exit(3);
        }

        if(WIFEXITED(status) == 0){
            printf("\nFiglio con PID = %d terminato in modo anomalo\n", pid);
        }
        else{
            printf("\nFiglio con PID = %d terminato correttamente con valore: %d (se 255 problemi)\n", pid, WEXITSTATUS(status));
        }
    }

    //Terminazione corretta del processo
    exit(0);
}