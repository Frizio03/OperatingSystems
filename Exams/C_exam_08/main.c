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

// Definizione del tipo linea come array di 250 char
typedef char line_t[250];

int main(int argc, char **argv)
{

    // Variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    // Variabili per la gestione di file
    int fd, fdout;

    // Variabili di controllo per read e write
    int nr, nw;

    // Variabile per linea corrente
    line_t linea;

    // Controllo sul numero di parametri
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;

    // Creazione del file nomecognome
    if ((fdout = creat("nomecognome", 0644)) < 0)
    {
        printf("Errore nella creazione del file\n");
        exit(2);
    }

    // Allocazione array per contenere le linee
    line_t *linesArray = malloc(N * sizeof(line_t));
    if (linesArray == NULL)
    {
        printf("Errore in allocazione array di linee\n");
        exit(3);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(4);
    }

    // Creazione dei canali di pipe padre-figlio e controllo esito
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
            /*Codice del processo figlio*/
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());

            // Chiusura pipe inutilizzate
            for (int k = 0; k < N; k++)
            {
                // Chiusura lati di lettura eccetto che corrente
                if (k != n)
                {
                    close(piped[k][0]);
                }
                // Chiusura lati di scrittura eccetto successivo
                if (k != (n + 1) % N)
                {
                    close(piped[k][1]);
                }
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

                    // Lettura dell'array proveniente dal figlio precedente o dal padre e controllo esito
                    nr = read(piped[n][0], linesArray, N * sizeof(line_t));
                    if (nr != N * sizeof(line_t))
                    {
                        printf("Errore nella read da pipe di figlio n = %d\n", n);
                        exit(-1);
                    }

                    // Copia dell'array
                    for (int k = 0; k < j + 1; k++)
                    {
                        linesArray[n][k] = linea[k];
                    }

                    // Invio dell'array in avanti
                    nw = write(piped[(n + 1) % N][1], linesArray, N * sizeof(line_t));
                    if (nw != N * sizeof(line_t))
                    {
                        printf("Errore nella write su pipe di figlio n = %d\n", n);
                        exit(-1);
                    }

                    // L'ultimo figlio scrive le linee sul file creato dal padre
                    if (n == N - 1)
                    {
                        for (int i = 0; i < N; i++)
                        {
                            for (int k = 0; k < 250; k++)
                            {
                                // scrittura su file fino al carattere \n
                                write(fdout, &(linesArray[i][k]), 1);
                                if (linesArray[i][k] == '\n')
                                {
                                    break;
                                }
                            }
                        }
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

            // Si fornisce la lunghezza dell'ultima linea letta come valore di ritorno
            exit(ritorno);
        }
    }

    /*Codice del processo padre*/

    // Chiusura pipe inutilizzate (partendo da indice 1)
    for (int n = 1; n < N; n++)
    {
        close(piped[n][0]);
        close(piped[n][1]);
    }

    // Invio del primo array a P0 e controllo esito
    nw = write(piped[0][1], linesArray, N * sizeof(line_t));
    if (nw != N * sizeof(line_t))
    {
        printf("Errore in write del padre su pipe\n");
        exit(7);
    }

    // Chiusura del lato di scrittura non più utilizzato
    close(piped[0][1]);

    // Attesa dei processi figli
    for (int n = 0; n < N; n++)
    {
        // Esecuzione della wait e controllo esito
        if ((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(8);
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