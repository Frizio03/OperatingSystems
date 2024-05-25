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

int termine(int *finito, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        if (!finito[i])
        {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv)
{
    // Variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    // Variabili per gestione file e dati
    int fd, fdw, inviati;
    char chs[2];

    // Variabili per controllo read e write
    int nr, nw;

    // Controllo sul numero di parametri
    if (argc < 4)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 4\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 2;

    // Apertura del file e controllo esito
    if ((fdw = open(argv[argc - 1], O_WRONLY)) < 0)
    {
        printf("Errore nell'apertura del file '%s'.\n", argv[argc - 1]);
        exit(2);
    }

    // Allocazione memoria per array di controllo stato processi
    int *finito = calloc(N, sizeof(int));
    if (finito == NULL)
    {
        printf("Errore in calloc array di controllo stato processi\n");
        exit(3);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(4);
    }

    // Creazione dei canali di pipe figlio-padre e controllo esito
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
            
            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {
                close(piped[k][0]);
                if (k != n)
                {
                    close(piped[k][1]);
                }
            }

            // Apertura del file e controllo esito
            if ((fd = open(argv[n + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[n + 1]);
                exit(-1);
            }

            // Tentativo di lettura di due caratteri dal file....
            inviati = 0;
            while ((nr = read(fd, chs, 2)) != 0)
            {

                // Invio dei caratteri al padre in base a quanti ne sono stati effettivamente letti
                nw = write(piped[n][1], chs, nr);

                if (nw != nr)
                {
                    printf("Errore: figlio con pid = %d ha scritto un numero di byte sabgliati su pipe!\n", getpid());
                    exit(-1);
                }

                // Aggiornamento contatore
                inviati += nr;
            }

            // Il figlio ritorna al padre il numero di caratteri inviati
            exit(inviati);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati di scrittura inutilizzati
    for (int k = 0; k < N; k++)
    {
        close(piped[k][1]);
    }

    // Lettura dei caratteriinviati dalle pipe finchè ci sono processi attivi
    while (!termine(finito, N))
    {
        // Per ciascun processo
        for (int n = 0; n < N; n++)
        {
            // Se il processo non risulta termnato...
            if (!finito[n])
            {
                // Tentativo di lettura da pipe corrispondente
                nr = read(piped[n][0], chs, 2);

                //Scrittura dei caratteri sul file di output (rispetto ad ner effettivamente letti)
                write(fdw, chs, nr);

                // Aggiornamento dello stato del processo
                if(nr == 0){
                    finito[n] = 1;
                }
            }
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
            printf("Figlio con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);
        }
    }

    // Uscita corretta dal programma
    exit(0);
}