#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

//Definizione del TIPO pipe_t come array di 2 interi
typedef int pipe_t[2];

//Funzione per controllare se i figli sono terminati
int termine(int* arr, int dim){
    for (int i = 0; i < dim; i++){
        if(!arr[i]){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char** argv){

    //Controllo sul numero di parametri
    if (argc < 3){
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }
    
    //Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;
    
    //Dichiarazione variabili per gestione processi
    int pid, pidFiglio;
    int status, ritorno;

    //Dichiarazione variabili per utilizzo file
    int fd;
    char car, chControllo, chMax;
    int indiceStampa, stampe;
    
    //Si suppone che una linea abbia lunghezza massima 250 caratteri incluso il terminatore
    char linea[250];

    //Allocazione array dinamico per valori scritti da figli
    int* finito = calloc(N, sizeof(int));
    if(finito == NULL){
        printf("Errore in malloc.\n");
        exit(2);
    }

    //Allocazione della memoria per array di pipe e controllo esito
    pipe_t* piped_fp = malloc(N*sizeof(pipe_t));
    pipe_t* piped_pf = malloc(N*sizeof(pipe_t));
    if (piped_fp == NULL || piped_pf == NULL){
        printf("Errore in malloc.\n");
        exit(3);
    }
    
    for(int n = 0; n < N; n++){
        //Creazione dei canali di pipe figli-padre (caratteri) e controllo esito
        if ((pipe(piped_fp[n])) < 0){
            printf("Errore nella creazione della pipe.\n");
            exit(4);
        }

        //Creazione dei canali di pipe padre-figli (per stampa) e controllo esito
        if ((pipe(piped_pf[n])) < 0){
            printf("Errore nella creazione della pipe.\n");
            exit(5);
        }
    }
    
    //Generazione dei processi figli
    for(int n = 0; n < N; n++){
        //Esecuzione della fork e controllo esito
        if((pid = fork()) < 0){
            printf("Errore in fork di indice %d\n", n);
            exit(6);
        }
    
        if(pid == 0){
            /*Codice del processo figlio*/
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());
            
            //Chiusura lati di lettura/scrittura inutilizzati
            for(int k = 0; k < N; k++){
                close(piped_fp[k][0]);
                close(piped_pf[k][1]);
                if(k != n){
                    close(piped_fp[k][1]);
                    close(piped_pf[k][0]);
                }
            }

            if((fd = open(argv[n+1], O_RDONLY)) < 0){
                printf("Errore nell'apertura del file '%s'\n", argv[n+1]);
                printf("File '%s' inesistente o permessi negati\n", argv[n+1]);
                exit(-1);
            }

            stampe = 0;
            int j = 0;
            while(read(fd, &linea[j], 1)){
                if(linea[j] == '\n'){
                    
                    //Comunicazione dell'ultimo carattere della linea corrente al padre
                    if(write(piped_fp[n][1], &linea[j-1], 1) != 1){
                        printf("Errore in scrittura in pipe figlio-padre\n");
                        exit(-1);
                    }

                    //Attesa istruzioni di stampa dal padre
                    if(read(piped_pf[n][0], &chControllo, 1) != 1){
                        printf("Errore in lettura da pipe padre-figlio\n");
                        exit(-1);
                    }
                    
                    if(chControllo == 'S'){
                        //Formattazione e stampa della linea
                        linea[j+1] = '\0';
                        printf("Processo figlio con n = %d e PID = %d: trovata linea terminante per %c con ASCII maggiore degli altri\n Stampa linea: '%s'\n", n, getpid(), linea[j-1], linea);
                        stampe++;
                    }

                    //Azzeramento dell'indice
                    j = 0;
                }
                else{
                    //Aggiornamento indice di posizione
                    j++;
                }
            }
            //Il figlio ritorna al padre il numero di stampe eseguite
            exit(stampe);
        }
    }
    
    /*Codice del processo padre*/
    printf("Processo padre inizia ad utilizzare pipe\n");
    
    //Chiusura lati di lettura/scrittura inutilizzati
    for(int n = 0; n < N; n++){
        close(piped_fp[n][1]);
        close(piped_pf[n][0]);
    }
    
    //Lettura dei dati dalle pipe dei figli e invio informazioni di stampa (fino a quando ci sono figli attivi)
    while(!termine(finito, N)){
        
        chMax = -1;
        for (int n = 0; n < N; n++){
            //Tentativo di lettura dal figlio i-esimo e aggiornamento stato figli
            finito[n] = (read(piped_fp[n][0], &car, 1) != 1);

            //Controllo se l'ascii è massimo e aggiorno l'indice del figlio che deve stampare
            if(!finito[n]){
                if(car > chMax){
                    chMax = car;
                    indiceStampa = n;
                }
            }
        }

        //Invio delle informazioni di stampa a ciascun figlio
        for(int n = 0; n < N; n++){
            if(n == indiceStampa){
                chControllo = 'S';
            }
            else{
                chControllo = 'N';
            }
            
            //Se il figlio non è terminato...
            if(!finito[n]){
                //Invio dei caratteri e controllo esito
                if(write(piped_pf[n][1], &chControllo, 1) != 1){
                    printf("Errore nell'invio del carattere di controllo al figlio di indice %d\n", n);
                }
            }
        }
    }

    //Attesa dei processi figli
    for(int n = 0; n < N; n++){
        //Esecuzione della wait e controllo esito
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait eseguita dal padre\n");
            exit(7);
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