#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>

//Definizione del TIPO pipe_t come array di 2 interi
typedef int pipe_t[2];

//Definizione del tipo struttura dati
typedef struct {
    int maxLinee;   //Valore c1 del testo
    int indice;     //Valore c2 del testo
} tipo_s;

//Definizione funzione
int mia_random(int n){
    int rnd = rand() % n;
    rnd++;
    return rnd;
}

int main(int argc, char** argv){
    
    //Inizializzazione seme random
    srand(time(NULL));

    //Dichiarazione variabili per gestione processi e file
    int pidFiglio, status, ritorno, fd;

    //Dichiarazione variabili da testo
    int N, L, r;
    int nr, nw, nlines;

    //Dichiarazione array per contenere una linea (max 250 caratteri da specifica)
    char linea[250];

    //Dichiarazione struct da mandare in avanti
    tipo_s S;
    S.maxLinee = 0;
    S.indice = 0;

    //Controllo sul numero di parametri
    if (argc < 4){
        printf("Parametri richiesti: >= 3; passati: %d\n", argc-1);
        printf("Formato parametri: int file file [file] ...\n");
        exit(1);
    }
    
    if((L = atoi(argv[1])) <= 0){
        printf("Errore: il primo parametro deve essere un intero!\n");
        printf("Formato parametri: int file file [file] ...\n");
        exit(2);
    }

    //Salvataggio del numero parametri passati da linea di comando
    N = argc - 2;
    
    //Allocazione memoria per array di pid
    int* pid = calloc(N, sizeof(int));
    if(pid == NULL){
        printf("Errore in malloc array di pid.\n");
        exit(3);
    }

    //Allocazione memoria per pipeline
    pipe_t* pipes = calloc(N, sizeof(pipe_t));
    
    //Allocazione di memoria per le pipe padre-figli
    pipe_t* piped_pf = malloc(N*sizeof(pipe_t));
    
    //Controllo allocazione di memoria
    if(pipes == NULL || piped_pf == NULL){
        printf("Errore in malloc array per pipeline.\n");
        exit(4);
    }

    //Generazione delle pipe
    for(int n = 0; n < N; n++){
        
        if(pipe(pipes[n]) < 0){
            printf("Errore in pipe su vettore per pipeline!\n");
            exit(5);
        }

        if(pipe(piped_pf[n]) < 0){
            printf("Errore in pipe su vettore per pipe pf!\n");
            exit(6);
        }
    }

    //Generazione delle pipe padre-figli
    for(int n = 0; n < N; n++){

    }
    
    //Generazione dei processi figli
    for(int n = 0; n < N; n++){
        //Esecuzione della fork e controllo esito
        if((pid[n] = fork()) < 0){
            printf("Errore in fork.\n");
            exit(7);
        }
    
        if(pid[n] == 0){
            /*Codice del processo figlio*/
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());
            
            //Chiusura pipe inutilizzate
            for(int k = 0; k < N; k++){
                if(k != n){
                    //Chiusura pipe scrittura eccetto che per figlio corrente
                    close(pipes[k][1]);

                    //Chiusura lati di lettura inutilizzati con padre
                    close(piped_pf[k][0]);
                }
                if(n == 0 || k != n-1){
                    //Chiusura pipe di lettura eccetto che per figlio precedente
                    close(pipes[k][0]);
                }
                //Chiusura lati di scrittura inutilizzati con padre
                close(piped_pf[k][1]);
            }
            
            //Apertura del file
            if((fd = open(argv[n+2], O_RDONLY)) < 0){
                printf("Errore nell'apertura del file '%s'\n", argv[n+2]);
                exit(-1);
            }

            //Calcolo della lunghezza in linee del file associato
            int j = 0; nlines = 0;
            while(read(fd, &linea[j], 1) != 0){
                if(linea[j] == '\n'){
                    nlines++;
                    j = 0;
                }
                else{
                    j++;
                }
            }

            //Lettura da pipe se non si è il primo figlio
            if(n != 0){
                nr = read(pipes[n-1][0], &S, sizeof(S));
                if(nr != sizeof(S)){
                    printf("Errore nella lettura da pipe[%d]\n", n);
                    exit(-1);
                }
            }
            
            //Aggiornamento della struct S
            if(nlines > S.maxLinee){
                S.maxLinee = nlines;
                S.indice = n;
            }

            //Invio delle informazioni in avanti
            nw = write(pipes[n][1], &S, sizeof(S));
            if(nw != sizeof(S)){
                printf("Errore nella scrittura su pipes[%d]\n", n);
                exit(-1);
            }

            //Attesa e ricavo delle informazioni dal padre
            nr = read(piped_pf[n][0], &r, sizeof(r));
            if(nr != sizeof(r)){
                printf("Errore in lettura da pipe con padre\n");
                exit(-1);
            }

            if(r > nlines){
                printf("Figlio di indice n = %d con PID = %d, ricevuto r = %d inammissibile\n", n, getpid(), r);
                exit(1);
            }
            else{
                
                //Riposizionamento file pointer a inizio file
                lseek(fd, 0L, SEEK_SET);
                
                int j = 0; nlines = 0;
                while(read(fd, &linea[j], 1) != 0){
                    if(linea[j] == '\n'){
                        nlines++;
                        if(nlines == r){
                            linea[j] = '\0';
                            break;
                        }
                        j = 0;
                    }
                    else{
                        j++;
                    }
                }

                printf("Figlio di indice n = %d con PID = %d, ricevuto r = %d\nFile associato: '%s', linea: '%s'\n",
                n, getpid(), r, argv[n+2], linea);

                exit(0);
            }
        }
    }
    
    /*Codice del processo padre*/

    //Chiusura delle pipe eccetto l'ultima in lettura
    for(int n = 0; n < N; n++){
        close(pipes[n][1]);
        if(n != N-1){
            close(pipes[n][0]);
        }

        //Chiusura lati di lettura inutilizzati
        close(piped_pf[n][0]);
    }

    //Lettura dati da pipeline
    nr = read(pipes[N-1][0], &S, sizeof(S));
    if(nr != sizeof(S)){
        printf("Errore di lettura in pipeline del padre\n");
    }

    //Stampa dati sella struct
    printf("La struct contiene maxLinee = %d ed indice = %d e si riferisce al file '%s'\n",
    S.maxLinee, S.indice, argv[S.indice+2]);

    //Controllo valore maxLinee
    if(S.maxLinee > L){
        printf("Errore: il valore ritornato dai figli maxLinee = %d ma L = %d\n", S.maxLinee, L);
    }

    r = mia_random(S.maxLinee);
    
    //Comunicazione del valore r ai figli
    for(int n = 0; n < N; n++){
        nw = write(piped_pf[n][1], &r, sizeof(r));
        if(nw != sizeof(r)){
            printf("Errore in comunicazione di r al figlio\n");
        }
    }

    //Attesa dei processi figli
    for(int n = 0; n < N; n++){
        //Esecuzione della wait e controllo esito
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait eseguita dal padre\n");
            exit(8);
        }
    
        //Controllo su terminazioni anomale del figlio e valore di ritorno
        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else{
            ritorno = WEXITSTATUS(status);
            printf("Figlio con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);
        }
    }

    //Uscita corretta dal programma
    exit(0);
}