#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>

/*
*
* ATTENZIONE:
* questo codice genera errori: non funziona
*
*/

// Definizione del TIPO pipe_t come array di 2 interi
typedef int pipe_t[2];

int istruzione = 0;

// Funzioni handler
void nothing(int sig) {
    printf("DEBUG - Sostituzione negata al figlio con PID = %d\n", getpid());
}

void replace(int sig)
{
    printf("DEBUG - Figlio con PID = %d ha ricevuto segnale di sostituzione\n", getpid());
    istruzione = 1;
}

int main(int argc, char **argv)
{
    // Variabili per gestione processi
    int pidFiglio, status, ritorno;

    // Variabili per gestione file e dati
    int fd, sost, finito, nr;
    char Cx, ch, letto, scarto;
    long int pos;

    // Controllo sul numero di parametri
    if (argc < 3)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 2;

    // Controllo sull'ultimo parametro
    if (strlen(argv[argc - 1]) != 1)
    {
        printf("Errore: l'ultimo parametro passato deve essere un singiolo carattere!\n");
        exit(2);
    }
    Cx = argv[argc - 1][0];

    // Allcoazione array di pid processi per kill
    int *pid = calloc(N, sizeof(int));
    if (pid == NULL)
    {
        printf("Errore in allocazione array di pid\n");
        exit(3);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped_fp = calloc(N, sizeof(pipe_t));
    pipe_t *piped_pf = calloc(N, sizeof(pipe_t));
    if (piped_fp == NULL || piped_pf == NULL)
    {
        printf("Errore in calloc array di pipe.\n");
        exit(4);
    }

    // Creazione dei canali di pipe fp-pf e controllo esito
    for (int i = 0; i < N; i++)
    {
        if ((pipe(piped_fp[i])) < 0)
        {
            printf("Errore nella creazione della pipe figlio-padre.\n");
            exit(5);
        }

        if ((pipe(piped_fp[i])) < 0)
        {
            printf("Errore nella creazione della pipe padre-figlio.\n");
            exit(6);
        }
    }

    // Generazione dei processi figli
    for (int i = 0; i < N; i++)
    {
        // Esecuzione della fork e controllo esito
        if ((pid[i] = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(7);
        }

        if (pid[i] == 0)
        {
            /* Codice del processo figlio */
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {
                close(piped_fp[k][0]);
                close(piped_pf[k][1]);
                if (k != i)
                {
                    close(piped_pf[k][0]);
                    close(piped_fp[k][1]);
                }
            }

            // Apertura del file e controllo esito
            if ((fd = open(argv[i + 1], O_RDWR)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[i + 1]);
                exit(-1);
            }

            // Azzeramento conttaore: il primo carattere del file ha posizione 1
            pos = 1L;
            sost = 0;

            // Lettura di ciascun carattere del file...
            while (read(fd, &ch, 1) != 0)
            {

                // Trovata occorrenza di Cx
                if (ch == Cx)
                {
                    // Invio della posizione del carattere al padre
                    pos = lseek(fd, 0L, SEEK_CUR);
                    write(piped_fp[i][1], &pos, sizeof(pos));

                    // Aggancio istruzioni ai segnali
                    signal(SIGUSR1, nothing);
                    signal(SIGUSR2, replace);

                    // Attesa di sitruzioni da parte del padre
                    pause();

                    if (istruzione)
                    {
                        read(piped_pf[i][0], &ch, 1);
                        printf("ch vale '%c'\n", ch);
                        lseek(fd, -1L, SEEK_CUR);
                        write(fd, &ch, 1);
                        sost++;
                        istruzione = 0;
                    }
                }
            }

            // Il figlio ritorna al padre il numero di posizioni lette
            exit(sost);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati di scrittura inutilizzati
    for (int k = 0; k < N; k++)
    {
        close(piped_pf[k][0]);
        close(piped_fp[k][1]);
    }

    // Lettura delle posizioni
    finito = 0;
    while (!finito)
    {
        // I figli potrebbero essere tutti finiti
        finito = 1;

        // Per ciascun processo...
        for (int i = 0; i < N; i++)
        {
            // Lettura da pipe di posizione inviata
            nr = read(piped_fp[i][0], &pos, sizeof(pos));

            if (nr != 0)
            {
                finito = 0;

                // Stampa informazioni su stdout
                printf("Figlio di indice i = %d ha trovato carattere '%c' in poszione %ld nel file '%s', sostituirlo?\n", i, Cx, pos, argv[i + 1]);

                // Attesa azione dell'utente
                read(1, &letto, 1);
                //printf("DEBUG - processo %d letto carattere '%c' da INPUT con nr = %d\n", getpid(), letto, nr);

                // Si attende che il processo figlio possa ricevere il segnale
                sleep(1);

                // Invio del segnale al figlio
                if (letto != '\n')
                {
                    // Lettura di eliminazione dello \n in eccesso
                    read(1, &scarto, 1);

                    write(piped_pf[i][1], &letto, 1);
                    kill(pid[i], SIGUSR2);
                }
                else
                {
                    kill(pid[i], SIGUSR1);
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
            exit(8);
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
