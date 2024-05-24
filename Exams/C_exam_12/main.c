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

    // Variabili per gestione file e dati
    int fd, indice;
    char car, chMin, chControllo;
    line_t linea;

    // Controllo sul numero di parametri
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped_fp = calloc(N, sizeof(pipe_t));
    pipe_t *piped_pf = calloc(N, sizeof(pipe_t));
    if (piped_fp == NULL || piped_pf == NULL)
    {
        printf("Errore in calloc.\n");
        exit(2);
    }

    // Creazione dei canali di pipe fp e pf e controllo esito
    for (int n = 0; n < N; n++)
    {
        if ((pipe(piped_fp[n])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(3);
        }

        if ((pipe(piped_pf[n])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(3);
        }
    }

    // Generazione dei processi figli
    for (int n = 0; n < N; n++)
    {
        // Esecuzione della fork e controllo esito
        if ((pid = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(4);
        }

        if (pid == 0)
        {
            /* Codice del processo figlio */
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {
                close(piped_fp[k][0]);
                close(piped_pf[k][1]);
                if (k != n)
                {
                    close(piped_pf[k][0]);
                    close(piped_fp[k][1]);
                }
            }

            // Apertura del file e controllo esito
            if ((fd = open(argv[n + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[n + 1]);
                exit(-1);
            }

            // Lettura di ciascun carattere del file e inserimento nella linea corrente
            int j = 0, lastj = 0;
            while (read(fd, &(linea[j]), 1) != 0)
            {
                // Trovato il termine di una linea
                if (linea[j] == '\n')
                {
                    // Salvo la linea corrente
                    lastj = j;
                    j = 0;
                }
                else
                {
                    // Aggiornamento indice
                    j++;
                }
            }

            // Ricavo il primo carattere della linea
            car = linea[0];

            // Comunicazione al padre del primo carattere della linea appena letta
            write(piped_fp[n][1], &car, 1);

            // Attesa delle indicazioni di stampa dal padre
            read(piped_pf[n][0], &chControllo, 1);

            ritorno = 0;
            if (chControllo == 'S')
            {

                // strasformazione della linea in stringa
                linea[lastj + 1] = '\0';

                // Stampa su stdout
                printf("Figlio di indice n = %d con PID = %d, identificato carattere '%c' nell'ultima linea di seguito:\n%s", n, getpid(), car, linea);

                // Impostazione valore di ritorno
                ritorno = 1;
            }

            exit(ritorno);
        }
    }

    /* Codice del processo padre*/

    // Chiusura lati di scrittura inutilizzati
    for (int k = 0; k < N; k++)
    {
        close(piped_pf[k][0]);
        close(piped_fp[k][1]);
    }

    chMin = 127;
    for (int n = 0; n < N; n++)
    {
        // Lettura dei caratteri inviati dai figli
        read(piped_fp[n][0], &car, 1);

        // Ricavo il carattere con ASCII minimo
        if (car < chMin)
        {
            chMin = car;
            indice = n;
        }
    }

    // Comunicazione di stampa ai processi figli
    for (int n = 0; n < N; n++)
    {
        if (n == indice)
        {
            chControllo = 'S';
        }
        else
        {
            chControllo = 'N';
        }

        // Invio dei dati su pipe
        write(piped_pf[n][1], &chControllo, 1);
    }

    // Attesa dei processi figli
    for (int n = 0; n < N; n++)
    {
        // Esecuzione della wait e controllo esito
        if ((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(5);
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