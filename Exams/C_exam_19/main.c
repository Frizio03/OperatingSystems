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

typedef struct
{
    int pid;      // Campo c1 della specifica
    long int occ; // Campo c2 della specifica
} dati;

int main(int argc, char **argv)
{
    // Variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    // Variabili per gestione file e dati
    int fd;
    char ch;
    dati *cur;

    // Variabili per controllo lettura/scrittura da pipe
    int nr, nw;

    // Controllo sul numero di parametri
    if (argc < 4)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 4\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 2;

    if (strlen(argv[argc - 1]) != 1)
    {
        printf("Errore: l'ultimo parametro deve essere un singolo carattere minuscolo\n");
        exit(2);
    }
    char C = argv[argc - 1][0];

    if (!islower(C))
    {
        printf("Errore: l'ultimo parametro deve essere un singolo carattere MINUSCOLO\n");
        exit(3);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(4);
    }

    // Creazione dei canali di pipe e controllo esito
    for (int i = 0; i < N; i++)
    {
        if ((pipe(piped[i])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
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
            /* Codice del processo figlio */
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d e indice %d associato al file %s\n", getpid(), i, argv[i + 1]);

            // Chiusura lati di pipe inutilizzati
            for (int k = 0; k < N; k++)
            {
                if (i == 0 || k != i - 1)
                    close(piped[k][0]);
                if (k != i)
                    close(piped[k][1]);
            }

            // Apertura del file e controllo esito
            if ((fd = open(argv[i + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[i + 1]);
                exit(-1);
            }

            // Allocazione di memoria necessaria per l'array di strutture
            cur = calloc(i + 1, sizeof(dati));
            if (cur == NULL)
            {
                printf("Errore in callo del figlio %d\n", i);
                exit(-1);
            }

            // Se non si è al primo figlio: lettura dati da pipeline
            if (i != 0)
            {
                // Lettura struttura dati proveniente da pipe e controllo esito
                nr = read(piped[i - 1][0], cur, i * sizeof(dati));
                if (nr != i * sizeof(dati))
                {
                    printf("Errore nella read da pipe con indice %d\n", i);
                }
            }

            // Ricerca delle occorrenze di C nel file associato
            cur[i].occ = 0L;
            while (read(fd, &ch, 1) != 0)
            {

                // Trovata occorrenza di C
                if (ch == C)
                {
                    cur[i].occ += 1;
                }
            }

            // Settaggio del pid dell'elemento associato in cur
            cur[i].pid = getpid();

            // Invio della struttura dati in avanti e controllo esito di scrittura
            nw = write(piped[i][1], cur, (i + 1) * sizeof(dati));
            if (nw != (i + 1) * sizeof(dati))
            {
                printf("Errore nella scrittura su pipe con indice %d\n", i);
            }

            // Il figlio ritorna al padre il proprio indice di ordine
            exit(i);
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

    // Allocazione array di strutture
    cur = calloc(N, sizeof(dati));
    if (cur == NULL)
    {
        printf("Errore in calloc per array del padre\n");
        exit(7);
    }

    // Lettura struttura dati proveniente da pipe e controllo esito
    nr = read(piped[N - 1][0], cur, N * sizeof(dati));
    if (nr != N * sizeof(dati))
    {
        printf("Errore nella read effettuata dal padre da pipe con indice %d\n", N - 1);
        exit(8);
    }

    for (int i = 0; i < N; i++)
    {
        printf("Processo figlio di ordine i = %d con PID = %d ha trovato %ld occorrenze di %c nel file %s\n",
               i, cur[i].pid, cur[i].occ, C, argv[i + 1]);
    }

    // Attesa dei processi figli
    for (int i = 0; i < N; i++)
    {
        // Esecuzione della wait e controllo esito
        if ((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(9);
        }

        // Controllo su terminazioni anomale del figlio e valore di ritorno
        if ((status & 0xFF) != 0)
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