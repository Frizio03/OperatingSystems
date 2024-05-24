#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

// Definizione del TIPO pipe_t come array di 2 interi
typedef int pipe_t[2];

//Definizione del tipo linea come array di 250 char
typedef char line_t[250];

// Definizione del tipo struct
typedef struct {
    long int min;    //VAriabile c1 del testo
    int n;      //Variabile c2 del testo
} data;

int main(int argc, char** argv)
{
    //Variabili per gestione processi
    int pidFiglio, status, ritorno;

    //Variabili per gestione file
    int fd; line_t linea;

    //Variabili per dati
    long int cur_tot, min; data D;

    //Variabili di controllo read e write pipeline
    int nr, nw;

    //Controllo sul numero di parametri
    if (argc < 3){
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }
    
    //Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;
    
    // Allocazione array per salvataggio PID
    int* pid = calloc(N, sizeof(int));
    if(pid == NULL){
        printf("Errore in calloc di array di pid.\n");
        exit(2);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t* pipes = calloc(N, sizeof(pipe_t));
    pipe_t* piped_pf = calloc(N, sizeof(pipe_t));
    if (pipes == NULL || piped_pf == NULL)
    {
        printf("Errore in calloc.\n");
        exit(3);
    }
    
    // Creazione dei canali di pipe fp-pf e controllo esito
    for(int n = 0; n < N; n++)
    {
        if ((pipe(pipes[n])) < 0){
            printf("Errore nella creazione della pipe per pipeline.\n");
            exit(4);
        }

        if ((pipe(piped_pf[n])) < 0){
            printf("Errore nella creazione della pipe per comunicazione padre-figlio.\n");
            exit(5);
        }
    }
    
    // Generazione dei processi figli
    for(int n = 0; n < N; n++)
    {
        // Esecuzione della fork e controllo esito
        if((pid[n] = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(6);
        }
    
        if(pid[n] == 0){
            /* Codice del processo figlio */

            // Chiusura lati di pipe inutilizzati
            for(int k = 0; k < N; k++)
            {
                if(n == 0 || k != n-1)
                    close(pipes[k][0]);
                if(k != n){
                    close(pipes[k][1]);
                    close(piped_pf[k][0]);
                }
                close(piped_pf[k][1]);
            }

            // Apertura del file associato e controllo esito
            if((fd = open(argv[n+1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[n+1]);
                exit(-1);
            }
            
            //Lettura di ciascun carattere del file linea del file
            cur_tot = 0L;
            int j = 0;
            while(read(fd, &(linea[j]), 1) != 0)
            {
                //Trovato il termine di una linea
                if(linea[j] == '\n')
                {
                    cur_tot++;
                    j = 0;
                }
                else
                {
                    j++;
                }
            }
            if(n != 0){
                
                //Lettura struttura dati proveniente da pipe e controllo esito
                nr = read(pipes[n-1][0], &D, sizeof(D));
                if(nr != sizeof(D))
                {
                    printf("Errore nella read da pipe con indice %d\n", n);
                    exit(-1);
                }
                
                // Confronto per trovare il processo con meno linee contate
                if(cur_tot < D.min){
                   D.min = cur_tot;
                   D.n = n; 
                }
            }
            else{
                D.min = cur_tot;
                D.n = n;
            }

            //Scrittura struttura dati su pipe e controllo esito
            nw = write(pipes[n][1], &D, sizeof(D));
            if(nw != sizeof(D))
            {
                printf("Errore nella scrittura su pipe con indice %d\n", n);
                exit(-1);
            }
            
            // Attesa di informazioni dal padre
            read(piped_pf[n][0], &min, sizeof(min));

            // Riposto I/O poiner a inizio file
            lseek(fd, 0L, SEEK_SET);

            //Lettura di ciascun carattere del file linea del file
            cur_tot = 0L;
            j = 0;
            while(read(fd, &(linea[j]), 1) != 0)
            {
                //Trovato il termine di una linea
                if(linea[j] == '\n')
                {
                    cur_tot++;

                    if(cur_tot == min){

                        // Trasformazione della linea in stringa
                        linea[j] = '\0';

                        // Stampa su stdout
                        printf("Figlio di indice n = %d, con PID = %d ha ricevuto min = %ld, stampa della linea del file associato '%s':\n%s\n",
                        n, getpid(), min, argv[n+1], linea);

                        //Interruzione ciclo
                        break;
                    }
                    j = 0;
                }
                else
                {
                    //Aggiornamento indice
                    j++;
                }
            }
            
            //Il figlio ritorna al padre il proprio indice d'ordine
            exit(n);
        }
    }
    
    /* Codice del processo padre */

    // Chiusura lati di pipe inutilizzati
    for(int k = 0; k < N; k++)
    {
        if(k != N-1)
            close(pipes[k][0]);
    
        close(pipes[k][1]);
        close(piped_pf[k][0]);
    }
    
    //Lettura struttura dati proveniente da pipe e controllo esito
    nr = read(pipes[N-1][0], &D, sizeof(D));
    if(nr != sizeof(D))
    {
        printf("Errore nella read del padre da pipe con indice %d\n", N-1);
    }
    
    // Stampa dati su stdout
    printf("Trovato file '%s' associato al processo di indice n = %d con PID = %d con numero minimo di linee pari a %ld\n",
    argv[D.n+1], D.n, pid[D.n], D.min);

    // Comunicazione a tutti i figli del numero minimo di linee
    min = D.min;
    for (int n = 0; n < N; n++)
    {
        write(piped_pf[n][1], &min, sizeof(min));
    }
    
    //Attesa dei processi figli
    for(int n = 0; n < N; n++)
    {
        //Esecuzione della wait e controllo esito
        if((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(7);
        }
    
        //Controllo su terminazioni anomale del figlio e valore di ritorno
        if((status & 0xFF) != 0)
        {
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else
        {
            ritorno = (status >> 8) & 0xFF;
            printf("Figlio con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);
        }
    }

    // Uscita corretta dal programma
    exit(0);
}