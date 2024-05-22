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

// Funzione per capire s ei figli sono terminati
int termine(int *arr, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        if (arr[i] == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv)
{

    // Controllo sul numero di parametri
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }

    // Dichiarazione variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    // Dichiarazione variabili per gestione caratteri e file e contatori
    char ch, chControllo, chMax;
    int fd, cur, stampe, indiceFiglio;

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;

    // Allocazione array di stato processi
    int *finito = calloc(N, sizeof(int));
    if (finito == NULL)
    {
        printf("Errore  in calloc dell'array di stato processi\n");
        exit(2);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped_fp = calloc(N, sizeof(pipe_t));
    pipe_t *piped_pf = calloc(N, sizeof(pipe_t));
    if (piped_fp == NULL || piped_pf == NULL)
    {
        printf("Errore in calloc dell pipe.\n");
        exit(3);
    }

    // Creazione dei canali di pipe e controllo esito
    for (int i = 0; i < N; i++)
    {
        if ((pipe(piped_fp[i])) < 0)
        {
            printf("Errore nella creazione della pipe figli-padre.\n");
            exit(4);
        }

        if ((pipe(piped_pf[i])) < 0)
        {
            printf("Errore nella creazione della pipe padre-figli.\n");
            exit(5);
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
            /*Codice del processo figlio*/
            // printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {

                close(piped_fp[k][0]);
                close(piped_pf[k][1]);

                if (k != i)
                {
                    close(piped_fp[k][1]);
                    close(piped_pf[k][0]);
                }
            }

            // Apertura del file associato e controllo esito
            if ((fd = open(argv[i + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[i + 1]);
                exit(-1);
            }

            // Lettura di ciascun carattere del file associato
            cur = 0;
            stampe = 0;
            while (read(fd, &ch, 1) != 0)
            {

                if (islower(ch))
                {
                    // Comunicazione del caratetre minuscolo al padre
                    if (write(piped_fp[i][1], &ch, 1) != 1)
                    {
                        printf("Errore in scrittura su pipe FP\n");
                        exit(-1);
                    }

                    // Attesa dell'indicazione di stampa
                    if (read(piped_pf[i][0], &chControllo, 1) != 1)
                    {
                        printf("Errore in lettura da pipe PF\n");
                        exit(-1);
                    }

                    if (chControllo == 'S')
                    {
                        printf("Figlio con PID = %d, trovato carattere '%c' in posizione %d nel file '%s'\n",
                               getpid(), ch, cur, argv[i + 1]);
                        stampe++;
                    }
                }
                cur++;
            }

            // Il figlio ritorna al padre il numero di stampe effettuate
            exit(stampe);
        }
    }

    /*Codice del processo padre*/

    // Chiusura pipe non utilizzate
    for (int i = 0; i < N; i++)
    {
        close(piped_fp[i][1]);
        close(piped_pf[i][0]);
    }

    while (!termine(finito, N))
    {
        chMax = -1;

        // Lettura dei dati dei figli
        for (int i = 0; i < N; i++)
        {

            // Tentativo di lettura e aggiornamento stato del figlio
            finito[i] = (read(piped_fp[i][0], &ch, 1) != 1);

            if (!finito[i])
            {
                if (ch > chMax)
                {
                    chMax = ch;
                    indiceFiglio = i;
                }
            }
        }

        // Invio delle informazioni di stampa a ciascun figlio
        for (int i = 0; i < N; i++)
        {
            if (i == indiceFiglio)
            {
                chControllo = 'S';
            }
            else
            {
                chControllo = 'N';
            }

            // Se il figlio non è terminato scrivo sulla pipe
            if (!finito[i])
            {
                if (write(piped_pf[i][1], &chControllo, 1) != 1)
                {
                    printf("Errore nella scrittura su pipe del carattere di controllo\n");
                }
            }
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