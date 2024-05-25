#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

// Definizione del TIPO pipe_t come array di 2 interi
typedef int pipe_t[2];

// Definizione del tipo linea di 255 char (da specifica)
typedef char line_t[255];

// Funzione per intercettare SIGPIPE
void catch(int sig){
    printf("Intercettato segnale SIGPIPE - tentativo di lettura da pipe processo figlio inesistente! ERRORE.\n");
    exit(10);
}

// Definizione della funzione mia_random
int mia_random(int n)
{
    int casuale;
    casuale = rand() % n;
    return casuale;
}

int main(int argc, char **argv)
{
    // Installazione funzione per segnale SIGPIPE
    signal(SIGPIPE, catch);

    // Dichiarazione variabili per processi
    int pid, pidFiglio, status, ritorno;

    // Dichiarazione variabili per file
    int fd, fdout, len, rnd, index;
    line_t linea;

    // Controllo sul numero di parametri
    if (argc < 6)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 6\n", argc);
        exit(1);
    }

    int H = atoi(argv[argc - 1]);
    if (H <= 0 || H >= 255)
    {
        printf("Errore: l'ultimo parametro deve essere un numero compreso fra 0 e 255 esclusi\n");
        exit(2);
    }
    printf("DEBUG: valore H = %d\n", H);

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 2;

    // Inizializzazione seme random
    srand(time(NULL));

    // Creazione del file fcreato
    if ((fdout = creat("files/fcreato", 0644)) < 0)
    {
        printf("Errore nella creazione del file\n");
        exit(3);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped_pf = calloc(N, sizeof(pipe_t));
    pipe_t *piped_fp = calloc(N, sizeof(pipe_t));
    if (piped_pf == NULL || piped_fp == NULL)
    {
        printf("Errore in calloc.\n");
        exit(4);
    }

    // Creazione dei canali di pipe padre-figlio e controllo esito
    for (int i = 0; i < N; i++)
    {
        if ((pipe(piped_pf[i])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(5);
        }

        if ((pipe(piped_fp[i])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(6);
        }
    }

    // Generazione dei processi figli
    for (int i = 0; i < N; i++)
    {
        // Esecuzione della fork e controllo esito
        if ((pid = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(6);
        }

        if (pid == 0)
        {
            /* Codice del processo figlio */
            //printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());
            
            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {
                close(piped_fp[k][0]);
                close(piped_pf[k][1]);
                if (k != i)
                {
                    close(piped_pf[k][0]);
                    close(piped_fp[k][1]);
                }
            }

            // Apertura del file e controllo esito
            if((fd = open(argv[i+1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[i+1]);
                exit(-1);
            }

            // Lettura di ciascun carattere del file linea del file
            int j = 0;
            int stampe = 0;
            while (read(fd, &(linea[j]), 1) != 0)
            {
                // Trovato il termine di una linea
                if (linea[j] == '\n')
                {

                    // Comunicazione al padre della lunghezza di linea corrente (con terminatore)
                    j++;
                    write(piped_fp[i][1], &j, sizeof(j));

                    // Lettura indice fornito dal padre
                    read(piped_pf[i][0], &index, sizeof(index));

                    // Controllo se l'indice ricevuto è ammissibile per la linea corrente
                    if (index < j)
                    {
                        // Scrivo il carattere index della line acorrente su fcreato
                        printf("DEBUG-Figlio di indice %d stampa '%c' trovato al posto %d\n", i, linea[index], index);
                        
                        write(fdout, &linea[index], 1);
                        stampe++;
                    }
                    else{
                        printf("DEBUG-Figlio di indice %d non ha caratteri al posto %d\n", i, index);
                    }

                    // Azzeramento contatore
                    j = 0;
                }
                else
                {
                    // Aggiornamento indice
                    j++;
                }
            }

            //Comunicazione del numero di stampe al padre
            exit(stampe);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati di scrittura inutilizzati
    for (int k = 0; k < N; k++)
    {
        close(piped_pf[k][0]);
        close(piped_fp[k][1]);
    }

    // Per ciascuna delle linee dei file
    for (int h = 0; h < H; h++)
    {

        rnd = mia_random(N - 1);

        // Lettura dei valori dei figli per la len di linea corrente
        for (int i = 0; i < N; i++)
        {

            // Lettura lunghezza linea da pipe
            read(piped_fp[i][0], &len, sizeof(len));
            printf("DEBUG-padre legge len = %d per la linea %d del processo i = %d\n", len, h, i);

            // In caso di figlio presecelto considero la lunghezza
            if (rnd == i)
            {
                index = mia_random(len - 1);
            }
        }

        // Invio dei dati a ciascun figlio
        for (int i = 0; i < N; i++)
        {

            // Comunicazione dell'indice del carattere da stampare
            printf("DEBUG-padre comunica index = %d per la linea %d\n", index, h);
            write(piped_pf[i][1], &index, sizeof(index));
        }
    }

    // Attesa dei processi figli
    for (int i = 0; i < N; i++)
    {
        // Esecuzione della wait e controllo esito
        if ((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(7);
        }

        // Controllo su terminazioni anomale del figlio e valore di ritorno
        if (WIFEXITED(status) == 0)
        {
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else
        {
            ritorno = WEXITSTATUS(status);
            printf("Figlio con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);
        }
    }

    // Uscita corretta dal programma
    exit(0);
}