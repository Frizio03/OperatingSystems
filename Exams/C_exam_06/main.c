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

    // Variabili per controllo file, read e write su pipe
    int fd, nr, nw;

    // variabili per operazioni dei processi figli
    int occ;
    char ch, okstampa = 'k';

    // Controllo sul numero di parametri
    if (argc < 5)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 5\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int Q = argc - 3;

    // Controllo sul secondo parametro
    int L = atoi(argv[2]);
    if (L < 0)
    {
        printf("Errore: il secondo parametro deve essere numerico\n");
        exit(2);
    }

    for (int i = 3; i < argc; i++)
    {
        if (strlen(argv[i]) != 1)
        {
            printf("Errore: parametro %s, non singolo carattere!\n", argv[1]);
            exit(3);
        }
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped = calloc(Q + 1, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(3);
    }

    // Creazione dei canali di pipe - e controllo esito
    for (int q = 0; q < Q + 1; q++)
    {
        if ((pipe(piped[q])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(4);
        }
    }

    // Generazione dei processi figli
    for (int q = 0; q < Q; q++)
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

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < Q + 1; k++)
            {
                if (k != q)
                    close(piped[k][0]);
                if (k != q + 1)
                    close(piped[k][1]);
            }

            // Apertura del file e controllo esito
            if ((fd = open(argv[1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[1]);
                exit(-1);
            }

            // Azzeramento occorrenze per la linea corrente
            occ = 0;

            // Lettura di ciascun carattere del file del file
            while (read(fd, &ch, 1) != 0)
            {

                // Termine della linea
                if (ch == '\n')
                {

                    // Attesa dell'indicazione di stampa
                    nr = read(piped[q][0], &okstampa, sizeof(char));
                    if (nr != sizeof(char))
                    {
                        printf("Errore nella read da pipe con indice %d\n", q);
                        exit(-1);
                    }

                    printf("- Figlio con indice q = %d e pid = %d ha trovato %d occorrenze di '%c' nella linea corrente\n", q, getpid(), occ, argv[q + 3][0]);

                    // Comunicazione di stampa in avanti e controllo esito
                    nw = write(piped[q + 1][1], &okstampa, sizeof(char));
                    if (nw != sizeof(char))
                    {
                        printf("Errore nella scrittura su pipe con indice %d\n", q);
                        exit(-1);
                    }

                    ritorno = occ;
                    occ = 0;
                }
                else if (ch == argv[q + 3][0])
                {
                    occ++;
                }
            }

            // Ciuascun figlio ritorna al padre il numero di occorrenze nell'ultima linea
            exit(ritorno);
        }
    }

    /*Codice del processo padre*/

    // Chiusura dei lati pipe non utilizzati
    for (int q = 0; q < Q + 1; q++)
    {
        if (q != 0)
            close(piped[q][1]);
        if (q != Q)
            close(piped[q][0]);
    }

    // Per ciascuna linea si stampa il messaggio iniziale
    for (int l = 0; l < L; l++)
    {
        printf("Linea %d del file %s\n", l + 1, argv[1]);

        // Invio dell'ok al primo figlio e controllo esito
        nw = write(piped[0][1], &okstampa, sizeof(char));
        if (nw != sizeof(char))
        {
            printf("Errore nella scrittura su pipe con indice %d\n", 0);
            exit(7);
        }

        // Attesa dell'ok delll'ultimo figlio e controllo esito
        nr = read(piped[Q][0], &okstampa, sizeof(char));
        if (nr != sizeof(char))
        {
            printf("Errore nella read da pipe con indice %d\n", Q);
            exit(8);
        }
    }

    // Attesa dei processi figli
    for (int q = 0; q < Q; q++)
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