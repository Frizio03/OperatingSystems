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

// Definizione del tipo linea di 250 char
typedef char line_t[250];

int main(int argc, char **argv)
{
    // Variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    // Variabili per file e controlli pipeline
    int fd, fdout, nr, nw;
    line_t linea;

    // Controllo sul numero di parametri
    if (argc < 4)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 4\n", argc);
        exit(1);
    }

    // Controllo dell'ultimo parametro
    int nroLinee = atoi(argv[argc - 1]);
    if (nroLinee <= 0)
    {
        printf("Attenzione: l'ultimo parametro deve essere un numero positivo!\n");
        exit(2);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 2;

    // Creazione del file fcreato
    if ((fdout = creat("fcreato", 0644)) < 0)
    {
        printf("Errore nella creazione del file\n");
        exit(3);
    }

    // Allocazione array di linee
    line_t *tutteLinee = calloc(N, sizeof(line_t));
    if (tutteLinee == NULL)
    {
        printf("Errore in allocazione array di linee\n");
        exit(4);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(5);
    }

    // Creazione dei canali di pipe per pipeline e controllo esito
    for (int n = 0; n < N; n++)
    {
        if ((pipe(piped[n])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(6);
        }
    }

    // Generazione dei processi figli
    for (int n = 0; n < N; n++)
    {
        // Esecuzione della fork e controllo esito
        if ((pid = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(7);
        }

        if (pid == 0)
        {
            /* Codice del processo figlio */
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {
                if (k != n)
                    close(piped[k][1]);

                if (n == 0 || k != n - 1)
                    close(piped[k][0]);
            }

            // Apertura del file associato e controllo esito
            if ((fd = open(argv[n + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[n + 1]);
                exit(-1);
            }

            // Lettura di ciascun carattere del file linea del file
            int j = 0;
            while (read(fd, &(linea[j]), 1) != 0)
            {
                // Trovato il termine di una linea
                if (linea[j] == '\n')
                {

                    // Escludo il primo figlio dalla lettura da pipe
                    if (n != 0)
                    {
                        // Attesa dell'array dal figlio
                        nr = read(piped[n - 1][0], tutteLinee, N * sizeof(line_t));
                        if (nr != N * sizeof(line_t))
                        {
                            printf("Errore nella read da pipe con indice %d\n", n);
                            exit(-1);
                        }
                    }

                    // Inserimento della linea nella posizione dell'array
                    memcpy(&tutteLinee[n], &linea, 250);

                    // Invio dell'array al processo successivo
                    nw = write(piped[n][1], tutteLinee, N * sizeof(line_t));
                    if (nw != N * sizeof(line_t))
                    {
                        printf("Errore nella scrittura su pipe con indice %d\n", n);
                        exit(-1);
                    }

                    ritorno = j;
                    j = 0;
                }
                else
                {
                    // Aggiornamento indice
                    j++;
                }
            }

            // Il processo figlio ritorna al padre la lunghezza dell'ultima linea letta
            exit(ritorno);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati pipe inutilizzati
    for (int k = 0; k < N; k++)
    {
        close(piped[k][1]);
        if (k != N - 1)
            close(piped[k][0]);
    }

    for (int l = 0; l < nroLinee; l++)
    {

        // Lettura struttura dati proveniente da pipe e controllo esito
        nr = read(piped[N - 1][0], tutteLinee, N * sizeof(line_t));
        if (nr != N * sizeof(line_t))
        {
            printf("Errore nella read da pipe con indice N-1 = %d\n", N - 1);
            exit(8);
        }

        printf("DEBUG-Processo padre ha letto array allo step nr. %d. Scrittura linee su fcreato.\n", l + 1);

        // Scrittura linee su fcreato
        for (int curlinea = 0; curlinea < N; curlinea++)
        {
            int j = 0;
            while (1)
            {
                write(fdout, &(tutteLinee[curlinea][j]), 1);
                if (tutteLinee[curlinea][j] == '\n')
                {
                    break;
                }
                j++;
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
            exit(9);
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