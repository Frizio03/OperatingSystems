#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>

// Definizione del TIPO pipe_t come array di 2 interi
typedef int pipe_t[2];

// Definizione funzione handler
void catch (int sig)
{
    printf("DEBUG - processo con PID = %d, ha ricevuto l'indicazione a procedere.\n", getpid());
}

int main(int argc, char **argv)
{
    // Variabili per processi
    int pidFiglio, status, ritorno;

    // Variabili per gestione file
    int fd, fdAF, nr;
    char ch, chAF;

    // Controllo sul numero di parametri
    if (argc < 4)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 4\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 2;

    // Apertura del file e controllo esito
    if ((fdAF = open(argv[argc - 1], O_RDONLY)) < 0)
    {
        printf("Errore nell'apertura del file '%s'.\n", argv[argc - 1]);
        exit(2);
    }

    // Allocazione array di pid
    int *pid = calloc(N, sizeof(int));
    int *terminato = calloc(N, sizeof(int));
    if (pid == NULL || terminato == NULL)
    {
        printf("Errore in allocazione array di pid e controllo\n");
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
        if ((pid[i] = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(6);
        }

        if (pid[i] == 0)
        {
            /* Codice del processo figlio */
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d associato al file %s\n", getpid(), argv[i + 1]);

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {
                close(piped[k][0]);
                if (k != i)
                {
                    close(piped[k][1]);
                }
            }

            // Apertura del file e controllo esito
            if ((fd = open(argv[i + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[i + 1]);
                exit(-1);
            }

            while (1)
            {
                // Aggancio della funzione handler a SIGUSR1
                signal(SIGUSR1, catch);
                // Attesa del segnale di autorizzazione
                pause();

                // Lettura del carattere
                nr = read(fd, &ch, 1);

                // Uscita corretta dal ciclo per fine lettura
                if (nr != 1)
                    break;

                // Comunicazione del carattere al padre
                write(piped[i][1], &ch, 1);
            }

            exit(0);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati di scrittura inutilizzati
    for (int k = 0; k < N; k++)
    {
        close(piped[k][1]);
    }

    // Lettura di ciascun carattere del file associato al padre
    while (read(fdAF, &chAF, 1) != 0)
    {

        // Invio autorizzazioni di lettura a tutti i figli non terminati
        for (int i = 0; i < N; i++)
        {
            if (terminato[i] == 0)
            {
                // Attendo che il figlio sia pronto a ricevere il segnale
                sleep(1);
                if (kill(pid[i], SIGUSR1) < 0)
                {
                    printf("Errore in kill di autorizzazione\n");
                }
            }
        }

        // Lettura dei dati inviati da ciascun figlio
        for (int i = 0; i < N; i++)
        {

            // Lettura da pipe di figli non terminati
            if (terminato[i] == 0)
            {
                read(piped[i][0], &ch, 1);

                // In caso di carattere diverso
                if (ch != chAF)
                {
                    // Invio del segnale SIGKILL al figlio da terminare
                    printf("DEBUG - Invio del segnale SIGKILL: ch != chAF\n");
                    if (kill(pid[i], SIGKILL) < 0)
                    {
                        printf("Errore in kill di %d!\n", pid[i]);
                    }
                    terminato[i] = 1;
                }
            }
        }
    }

    // Attendo che i figli attivi siano pronti a ricevere i segnali
    sleep(1);

    // Spedizione di un unltimo segnale per sbloccare i figli attivi per farli leggere EOF (ultimo ciclo)
    for (int i = 0; i < N; i++)
    {
        if (terminato[i] == 0)
        {
            if (kill(pid[i], SIGUSR1) < 0)
            {
                printf("Errore in kill di autorizzazione\n");
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
            exit(7);
        }

        // Controllo su terminazioni anomale del figlio e valore di ritorno
        if ((status & 0xFF) != 0)
        {
            printf("Figlio con PID = %d terminato in modo anomalo (segnale) quindi il file '%s' risulta DIVERSO da '%s'\n", pidFiglio, argv[argc - 1], argv[i + 1]);
        }
        else
        {
            ritorno = (status >> 8) & 0xFF;
            printf("Figlio con PID = %d ha ritornato %d quindi il file '%s' risulta UGUALE a '%s'\n", pidFiglio, ritorno, argv[argc - 1], argv[i + 1]);
        }
    }

    // Uscita corretta dal programma
    exit(0);
}