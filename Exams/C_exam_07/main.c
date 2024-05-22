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

    // Variabili per gestione file
    int fd, fdout;
    int nlines, len;
    char ch;
    char linea[200];

    // Controllo sul numero di parametri
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;

    // Creazione del file temporaneo
    if ((fdout = creat("/tmp/nomecognome", 0644)) < 0)
    {
        printf("Errore nella creazione del file /tmp/nomecognome\n");
        exit(2);
    }

    // Generazione pipe e processo figlio speciale
    pipe_t p;
    if (pipe(p) < 0)
    {
        printf("Errore in pipe di figlio speciale\n");
        exit(3);
    }

    // Generazione del figlio che alcola la lunghezza del primo file
    if ((pid = fork()) < 0)
    {
        printf("Errore in fork\n");
        exit(4);
    }

    if (pid == 0)
    {
        /*Codice del processo figlio che utilizza wc*/
        printf("DEBUG-Esecuzione processo FIGLIO per WC con PID = %d\n", getpid());

        // Chiusura lato pipe di lettura non utilizzato
        close(p[0]);

        // Ridirezione stdin
        close(0);
        if ((fd = open(argv[1], O_RDONLY)) < 0)
        {
            printf("Errore nell'apertura del file '%s'.\n", argv[1]);
            exit(-1);
        }

        // Ridirezione stdout
        close(1);
        dup(p[1]);
        close(p[1]);

        // Esecuzione del comando wx per ricavare le linee del file
        execlp("wc", "wc", "-l", NULL);

        // Codice che viene eseguito solo in caso di fallimento della exec
        printf("Errore in exec!\n");
        exit(-1);
    }

    /*Codice del processo padre*/

    // Chiusura lato di scrittura pipe non utilizato
    close(p[1]);

    // Lettura dei dati da pipe e trasformazione in intero
    nlines = 0;
    while (read(p[0], &ch, 1) != 0)
    {
        if (ch != '\n')
        {
            nlines = nlines * 10 + atoi(&ch);
        }
    }

    printf("DEBUG-nlines = %d\n", nlines);

    // Controllo dell'esito prodotto dal processo speciale
    if (nlines == 0)
    {
        printf("File '%s' vuoto o nessun valore inviato dal figlio\n", argv[1]);
        exit(5);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(6);
    }

    // Creazione dei canali di pipe padre-figlio e controllo esito
    for (int n = 0; n < N; n++)
    {
        if ((pipe(piped[n])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(7);
        }
    }

    // Generazione dei processi figli
    for (int n = 0; n < N; n++)
    {
        // Esecuzione della fork e controllo esito
        if ((pid = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(8);
        }

        if (pid == 0)
        {
            /*Codice del processo figlio*/
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

            // Lettura di ciascun carattere dal file associato
            int j = 0;
            while (read(fd, &(linea[j]), 1) != 0)
            {
                if (linea[j] == '\n')
                {
                    // Invio al padre della lunghezza della linea
                    j++;
                    write(piped[n][1], &j, sizeof(j));

                    // Invio della linea al padre
                    write(piped[n][1], linea, j);

                    // Azzeramento indice e salvataggio valore da ritornare
                    ritorno = j;
                    j = 0;
                }
                else
                {
                    // Aggiornamento indice
                    j++;
                }
            }

            // Si fornisce come valore di ritorno la lunghezza dell'ultima linea
            exit(ritorno);
        }
    }

    /*Codice del processo padre*/

    // Chiusura lati di scrittura inutilizzati
    for (int n = 0; n < N; n++)
    {
        close(piped[n][1]);
    }

    // Lettura di sciascuna linea...
    for (int j = 0; j < nlines; j++)
    {

        //...inviata da ciascun figlio
        for (int n = 0; n < N; n++)
        {

            // lettura lunghezza e linea da pipe
            read(piped[n][0], &len, sizeof(len));
            read(piped[n][0], linea, len);

            printf("DEBUG-len = %d per processo di indice n = %d\n", len, n);

            // DEBUG
            // linea[len] = '\0';
            // printf("DEBUG-Scritta linea: %s\n", linea);

            // Scrittura su file
            write(fdout, linea, len);
        }
    }
    // Attesa dei processi figli (anche speciale)
    for (int n = 0; n < N + 1; n++)
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