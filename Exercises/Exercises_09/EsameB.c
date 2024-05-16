#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
typedef int pipe_t[2];   // Si definisce il tipo pipe_y come array di due interi

//Definizione della struct
struct ritStruct {
    int c1;
    int c2;
    char c3[250];
};

int main(int argc, char** argv){

    //Dichiarazione variabili
    int pid, pidFiglio;
    int status, ritorno;
    int fd;
    struct ritStruct s;

    //Controllo del numero di parametri
    if(argc < 4){
        printf("Parametri richiesti: >= 3, passati: %d\n", argc-1);
        exit(1);
    }

    //Ricavo il valore di N
    int N = argc-1;

    //Allocazione di memoria necessaria per contenere tutti i fd delle pipe
    pipe_t* piped = malloc(N*sizeof(pipe_t));
    if(piped == NULL){
        printf("Errore in malloc!\n");
        exit(2);
    }

    //Creazione delle N pipe
    for(int i = 0; i < N; i++){
        if(pipe(piped[i]) < 0){
            printf("Errore in pipe n. %d\n", i);
            exit(3);
        }
    }

    printf("DEBUG-Processo padre con PID = %d, inizio generazione figli\n", getpid());

    //Generazione degli N processi figli
    for(int i = 0; i < N; i++){
        
        if((pid = fork()) < 0){
            printf("Errore in fork n. %d\n", i);
            exit(4);
        }

        if(pid == 0){
            /*Codice del processo figlio*/

            printf("DEBUG-Processo figlio con PID = %d\n", getpid());

            //Chiusura dei lati di pipe di lettura e scrittura non utilizzati
            for(int k = 0; k < N; k++){
                
                //Chiusura pipe di lettura (non utilizzate)
                close(piped[k][0]);

                //Chiusura pipe di scrittura riferite ad altri processi
                if(k != i){
                    close(piped[k][1]);
                }
            }

            //Generazione della pipe per comunicare col nipote
            int p[2];
            if(pipe(p) < 0){
                printf("Errore in pipe figlio\n");
                exit(-1);
            }

            //Generazione del processo nipote per eseguire la wc
            if((pid = fork()) < 0){
                printf("Errore in fork del figlio %d", i);
                exit(-1);
            }

            if(pid == 0){
                /*Codice del processo nipote*/
                
                printf("DEBUG-Processo nipote con PID = %d\n", getpid());

                //Chiusura della pipe padre-figlio ancora aperta
                close(piped[i][1]);

                //Chiusura pipe di lettura inutilizzata
                close(p[0]);

                //Impostazione di pipe come stdout
                close(1);
                dup(p[1]);
                close(p[1]);

                //Controllo esistenza del file passato
                if((fd = open(argv[i+1], O_RDONLY)) < 0){
                    printf("Errore nell'apertura di '%s'\n", argv[i+1]);
                    exit(-1);
                }
                close(fd);

                //Esecuzione del comando filtro wc
                execlp("sort", "sort", "-f", argv[i+1], NULL);

                //Ritorno in caso di exec fallita
                printf("Exec fallita per nipote %d", getpid());
                exit(-1);
            }

            /*Proseguimento codice del processo figlio*/

            //Chiusura pipe di scrittura inutilizzata
            close(p[1]);

            //Salvo il pid del nipote nella struct
            s.c1 = pid;

            int count = 0;

            while(read(p[0], &(s.c3[count]), 1) != 0){
                if(s.c3[count] == '\n'){
                    s.c3[count+1] = '\0';
                    count++;
                    break;
                }
                else{
                    count++;
                }
            }

            //Salvataggio lunghezza linea
            s.c2 = count;

            //Comunicazione del valore al padre
            write(piped[i][1], &s, sizeof(s));

            //Attesa del processo nipote
            if((pidFiglio = wait(&status)) < 0){
                printf("Errore in wait del figlio\n");
                exit(-1);
            }

            //Controllo sui valori di ritorno della wait
            if(pid != pidFiglio){
                printf("Errore: pid ritornato dalla wait diverso da quello della fork\n");
                exit(-1);
            }

            //Controllo esito del processo nipote
            if(WIFEXITED(status) == 0){
                printf("Nipote con PID = %d terminato in modo anomalo\n", pid);
                exit(-1);
            }
            else{
                ritorno = WEXITSTATUS(status);
                //printf("Nipote con PID = %d ha ritornato %d (255 = errore)\n", pidFiglio, ritorno);
                exit(ritorno);
            }
        }
    }

    /*Codice del processo padre*/

    //Chiusura di tutte le pipe di scrittura
    for(int k = 0; k < N; k++){
        close(piped[k][1]);
    }

    //Recupero informazioni dalle pipe
    for(int i = 0; i < N; i++){
        read(piped[i][0], &s, sizeof(s));
        printf("Dati del file '%s':\n", argv[i+1]);
        printf("> Pid nipote: %d\n", s.c1);
        printf("> Lunghezza linea: %d\n", s.c2);
        printf("> Prima linea: %s\n", s.c3);
    }

    //Attesa degli N figli
    for(int i = 0; i < N; i++){
        
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait n. %d", i);
            exit(5);
        }

        //Controllo esito del processo nipote
        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else{
            ritorno = WEXITSTATUS(status);
            printf("Figlio con PID = %d ha ritornato %d (255 = errore)\n", pidFiglio, ritorno);
        }
    }

    //Uscita corretta dal processo
    exit(0);
}