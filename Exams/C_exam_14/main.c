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

int main(int argc, char **argv)
{
    // Variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    // Variabili per gestione file e dati
    int fd, fdout;
    char car, ultimoCar;

    // Variabili di controllo read e write su pipe
    int nr, nw;

    // Controllo sul numero di parametri
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;

    // Creazione del file fcreato
    if ((fdout = creat("fcreato", 0644)) < 0)
    {
        printf("Errore nella creazione del file\n");
        exit(2);
    }

    // Allocazione array da inviare su pipe
    char *cur = calloc(N, sizeof(char));

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(4);
    }

    // Creazione dei canali di pipe e controllo esito
    for (int n = 0; n < N; n++)
    {
        if ((pipe(piped[n])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(5);
        }
    }

    // Generazione dei processi figli
    for (int n = 0; n < N; n++)
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
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());

            // Chiusura lati di pipe inutilizzati
            for (int k = 0; k < N; k++)
            {
                if (n == 0 || k != n - 1)
                    close(piped[k][0]);
                if (k != n)
                    close(piped[k][1]);
            }

            // Apertura del file associato e controllo esito
            if ((fd = open(argv[n + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[n + 1]);
                exit(-1);
            }

            // Lettura di ciascun carattere del file del file
            int j = 1;
            while (read(fd, &car, 1) != 0)
            {

                // Trovato un carattere in posizione dispari
                if (j % 2 != 0)
                {

                    // Lettura dell'array da pipeline
                    if (n != 0)
                    {
                        // Lettura struttura dati proveniente da pipe e controllo esito
                        nr = read(piped[n - 1][0], cur, N * sizeof(char));
                        if (nr != N * sizeof(char))
                        {
                            printf("Errore nella read da pipe con indice %d\n", n);
                            exit(-1);
                        }
                    }

                    // Inserimento del carattere nell'array
                    cur[n] = car;

                    // Invio in avanti dell'array su pipeline
                    nw = write(piped[n][1], cur, N * sizeof(char));
                    if (nw != N * sizeof(char))
                    {
                        printf("Errore nella scrittura su pipe con indice %d\n", n);
                        exit(-1);
                    }

                    ultimoCar = car;
                }

                // Aggiornamento indice
                j++;
            }

            // Il figlio ritona l'ultimo carattere dispari letto
            exit(ultimoCar);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati di pipe inutilizzati
    for (int k = 0; k < N; k++)
    {
        if (k != N - 1)
            close(piped[k][0]);

        close(piped[k][1]);
    }

    // Lettura dati da pipe finechè disponibili
    while ((nr = read(piped[N - 1][0], cur, N * sizeof(char))) > 0)
    {

        // Inserimento di ciascun carattere dell'array nel file fcreato
        for (int i = 0; i < N; i++)
        {
            write(fdout, &(cur[i]), 1);
        }
    }

    // Attesa dei processi figli
    for (int n = 0; n < N; n++)
    {
        // Esecuzione della wait e controllo esito
        if ((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(7);
        }

        // Controllo su terminazioni anomale del figlio e valore di ritorno
        if ((status & 0xFF) != 0)
        {
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else
        {
            ritorno = (status >> 8) & 0xFF;
            printf("Figlio con PID = %d ha ritornato %d (255 = errori) ossia il carattere '%c'\n", pidFiglio, ritorno, ritorno);
        }
    }

    // Uscita corretta dal programma
    exit(0);
}