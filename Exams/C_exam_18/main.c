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
    int fd, fcreato;
    int nro;
    long int lenfile;

    // Variabili di controllo lettura e scrittura su pipe
    int nr, nw;

    // Controllo sul numero di parametri
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 2;

    int C = atoi(argv[argc - 1]);
    if (C <= 0 || C % 2 == 0)
    {
        printf("Errore: l'ultimo parametro deve essere un intero strettamente positivo e dispari \n");
        exit(2);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(3);
    }

    // Creazione dei canali di pipe figlio-figlio e controllo esito
    for (int i = 0; i < N; i++)
    {
        if ((pipe(piped[i])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(4);
        }
    }

    // Allocazione memoria per bloccco b di char di diemnsione C
    char *b = calloc(C, sizeof(char));
    if (b == NULL)
    {
        printf("Errore in allocazione spazio per blocc b di lettura\n");
        exit(5);
    }

    // Generazione dei processi figli
    for (int i = 0; i < 2 * N; i++)
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
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d, indice %d associato al file %s\n", getpid(), i, argv[i / 2 + 1]);

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {
                if (k != (i / 2))
                {
                    close(piped[k][0]);
                    close(piped[k][1]);
                }
            }

            // I processi di indice pari chiudono il lato di lettura
            if (i % 2 == 0)
            {
                close(piped[i][0]);
            }
            // I processi di indice dispari chiudono il lato di scrittura
            else
            {
                close(piped[i][1]);
            }

            // Apertura del file associato e controllo esito
            if ((fd = open(argv[i / 2 + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[i / 2 + 1]);
                exit(-1);
            }

            // Inizializzazione del nro di blocchi letti
            nro = 0;

            // Calcolo della lunghezza del file in blocchi
            lenfile = lseek(fd, 0L, SEEK_END) / C;

            // Caso del secondo processo della coppia
            if (i % 2 != 0)
            {
                // Creazione nome del file
                char *nome = calloc(strlen(argv[i / 2 + 1]) + 11, sizeof(char));
                strcpy(nome, argv[i / 2 + 1]);
                strcat(nome, ".mescolato");

                // Creazione del file fcreato.mescolato
                if ((fcreato = creat(nome, 0644)) < 0)
                {
                    printf("Errore nella creazione del file\n");
                    exit(-1);
                }

                // Posizionamento del I/O pointer dopo la prima metà
                lseek(fd, lenfile * C / 2, SEEK_SET);

                // Lettura dati da file
                while (read(fd, b, C))
                {
                    // Scrittura del blocco sul file fcreato
                    write(fcreato, b, C);

                    // Lettura blocco proveniente dal primo processo
                    nr = read(piped[i / 2][0], b, C);
                    if (nr != C)
                    {
                        printf("Errore nella read da pipe con indice %d, letto numero sbagliato di byte!\n", i / 2);
                    }

                    // Scrittura del blocco sul file fcreato
                    write(fcreato, b, C);

                    // Aggiornamento del nro di blocchi letti
                    nro++;
                }
            }
            // Caso del primo processo della coppia
            else
            {
                // Riposizionamento del I/O pointer all'inizio del file
                lseek(fd, 0L, SEEK_SET);
                
                // Lettura dati da file
                while (read(fd, b, C))
                {
                    // Scrittura del blocco dati letto su pipe
                    nw = write(piped[i / 2][1], b, C);
                    if (nw != C)
                    {
                        printf("Errore nella scrittura su pipe con indice %d\n", i / 2);
                    }

                    // Aggiornamento del nro di blocchi letti
                    nro++;

                    // Controllo di non superare la prima metà del file
                    if (nro >= lenfile / 2)
                    {
                        break;
                    }
                }
            }

            // Ogni figlio ritorna al padre il nro di blocchi letti
            exit(nro);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati di pipe inutilizzati
    for (int k = 0; k < N; k++)
    {
        close(piped[k][0]);
        close(piped[k][1]);
    }

    // Attesa dei processi figli
    for (int i = 0; i < 2 * N; i++)
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