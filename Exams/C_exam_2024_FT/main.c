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
    int CreatedFile;
    char Buff[250];

    // Controllo sul numero di parametri
    if (argc != 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere 3.\n", argc);
        exit(1);
    }

    // Ricavo e controllo il numero Z
    int Z = atoi(argv[1]);
    if (Z <= 0)
    {
        printf("Errore: il primo parametro deve esssere un numero strettamente positivo.\n");
        exit(2);
    }

    // Creazione del file il cui nome viene passato come parametro
    if ((CreatedFile = creat(argv[2], 0644)) < 0)
    {
        printf("Errore nella creazione del file %s\n", argv[2]);
        exit(3);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *pipe_fp = malloc(Z * sizeof(pipe_t));
    if (pipe_fp == NULL)
    {
        printf("Errore in malloc dell'array di pipe figli-padre.\n");
        exit(4);
    }

    // Creazione dei canali di pipe figli-padre e controllo esito
    for (int z = 0; z < Z; z++)
    {
        if ((pipe(pipe_fp[z])) < 0)
        {
            printf("Errore nella creazione della pipe figlio-padre.\n");
            exit(5);
        }
    }

    // Generazione dei processi figli
    for (int z = 0; z < Z; z++)
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
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d e indice %d\n", getpid(), z);

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < Z; k++)
            {
                close(pipe_fp[k][0]);
                if (k != z)
                {
                    close(pipe_fp[k][1]);
                }
            }

            // Pipe per comunicazione nipote-figlio
            pipe_t pipe_nf;
            if (pipe(pipe_nf) < 0)
            {
                printf("Errore nella creazione della pipe nipote-figlio di indice %d\n", z);
                exit(-1);
            }

            // Generazione di un processo nipote e controllo esito
            if ((pid = fork()) < 0)
            {
                printf("Errore in fork di generazione del nipote con indice %d.\n", z);
                exit(-1);
            }

            if (pid == 0)
            {
                /*Codice del processo nipote*/
                printf("DEBUG-Esecuzione processo NIPOTE con PID = %d e indice %d\n", getpid(), z);

                // Chiusura pipe con il padre inutilizzata
                close(pipe_fp[z][1]);

                // Chiusura lato di lettura piepe inutilizzata
                close(pipe_nf[0]);

                // Ridirezione dello stdout su lato scrittura della pipe
                close(1);
                dup(pipe_nf[1]);
                close(pipe_nf[1]);

                // Esecuzione del comando ps per ricavare l'elenco dei processi
                execlp("ps", "ps", NULL);

                // Codice che viene eseguito solo in caso di fallimento della exec
                printf("Errore in exec del nipote con indice %d!\n", z);
                exit(-1);
            }

            /* Proseguimento codice del processo figlio */

            // Chiusura lato di scrittura nella pipe con nipote
            close(pipe_nf[1]);

            // Ridirezione di stdin da pipe con nipote
            close(0);
            dup(pipe_nf[0]);
            close(pipe_nf[0]);

            // Ridirezione di stdout su pipe associata con padre
            close(1);
            dup(pipe_fp[z][1]);
            close(pipe_fp[z][1]);

            // Stringa per contenere il pid del processo corrente
            char pids[10];

            // Ricavo il pid del processo figlio corrente e lo inserisco in una stringa
            sprintf(pids, "%d", getpid());

            // Ricavo la linea con il pid del figlio corrente tramite il comando grep
            execlp("grep", "grep", pids, NULL);

            // Codice che viene eseguito solo in caso di fallimento della exec
            printf("Errore in exec del figlio con indice %d!\n", z);
            exit(-1);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati di scrittura inutilizzati delle pipe con i figli
    for (int k = 0; k < Z; k++)
    {
        close(pipe_fp[k][1]);
    }

    for (int z = 0; z < Z; z++)
    {
        // Lettura di ciascun carattere da pipe con il figlio e inserimento nel buffer
        int j = 0;
        while (read(pipe_fp[z][0], &(Buff[j]), 1) != 0)
        {
            // Trovato il termine della linea
            if (Buff[j] == '\n')
            {
                // Aggiornamento indice con terminatore di linea
                j++;

                // Scrittura della linea all'interno del file
                write(CreatedFile, Buff, j);

                j = 0;
            }
            else
            {
                // Aggiornamento indice
                j++;
            }
        }
    }

    // Attesa dei processi figli
    for (int z = 0; z < Z; z++)
    {
        // Esecuzione della wait e controllo esito
        if ((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait.\n");
            exit(7);
        }

        // Controllo su terminazioni anomale del figlio e valore di ritorno
        if ((status & 0xFF) != 0)
        {
            printf("Figlio con PID = %d terminato in modo anomalo.\n", pidFiglio);
        }
        else
        {
            ritorno = (status >> 8) & 0xFF;
            
            // Secondo la semantica UNIX il valore di ritorno 0 indica esito di successo
            if (ritorno == 0)
            {
                printf("Figlio con PID = %d ha ritornato %d ossia la grep del figlio ha avuto successo.\n", pidFiglio, ritorno);
            }
            else
            {
                printf("Figlio con PID = %d ha ritornato %d ossia la grep del figlio NON ha avuto successo!\n", pidFiglio, ritorno);
            }
        }
    }

    // Uscita corretta dal programma
    exit(0);
}