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

int main(int argc, char** argv)
{
    // Variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    // Variabili per gestione file e dati
    int fd;
    char ch;
    int num; long int somma;

    // Variabili per controllo lettura/scrittura su pipe
    int nr, nw;

    //Controllo sul numero di parametri
    if (argc < 3){
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }
    
    //Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;
    
    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t* piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(2);
    }
    
    // Creazione dei canali di pipe figlio-padre e controllo esito
    for(int n = 0; n < N; n++)
    {
        if ((pipe(piped[n])) < 0){
            printf("Errore nella creazione della pipe.\n");
            exit(3);
        }
    }
    
    // Generazione dei processi figli
    for(int n = 0; n < N; n++)
    {
        // Esecuzione della fork e controllo esito
        if((pid = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(4);
        }
    
        if(pid == 0){
            /* Codice del processo figlio */
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d associato al file %s\n", getpid(), argv[n+1]);
            
            // Chiusura lati di lettura/scrittura inutilizzati
            for(int k = 0; k < N; k++)
            {
                close(piped[k][0]);
                if(k != n){
                    close(piped[k][1]);
                }
            }
            
            // Apertura del file e controllo esito
            if((fd = open(argv[n+1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[n+1]);
                exit(-1);
            }
            
            // Inizializzazione variabile somma
            somma = 0L; num = 0;

            //Lettura di ciascun carattere del file del file
            int j = 0;
            while(read(fd, &ch, 1) != 0){
                
                //Trovato carattere numerico
                if(j % 2 == 0)
                {
                    somma += atoi(&ch);
                    num++;
                }

                // Aggiornamento indice di posizione
                j++;
            }

            // Comunicazione al padre del valore di somma tramite pipe
            nw = write(piped[n][1], &somma, sizeof(somma));
            if(nw != sizeof(somma))
            {
                printf("Errore nella scrittura su pipe con indice %d\n", n);
                exit(-1);
            }

            // Il figlio ritorna al padre il numero di caratteri numerici letti
            exit(num); 
        }
    }
    
    /* Codice del processo padre */
    
    // Chiusura lati di scrittura inutilizzati
    for(int k = 0; k < N; k++)
    {
        close(piped[k][1]);
    }
    
    // Ricezione dati (in ordine) dai figli e stampa su stdout
    for (int n = 0; n < N; n++)
    {
        //Lettura struttura dati proveniente da pipe e controllo esito
        nr = read(piped[n][0], &somma, sizeof(somma));
        if(nr != sizeof(somma))
        {
            printf("Errore nella read da pipe con indice %d\n", n);
        }
        
        // Stampa dati su stdout
        printf("Processo figlio di indice n = %d ha comunicato somma = %ld per il file %s\n", n, somma, argv[n+1]);
    }

    //Attesa dei processi figli
    for(int n = 0; n < N; n++)
    {
        //Esecuzione della wait e controllo esito
        if((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(5);
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