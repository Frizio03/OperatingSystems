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

    // Variabili per gestione file e dati
    int fd, fcreato;
    line_t linea;
    int indiceLinea, len, nro;

    // Variabili di controllo lettura da pipe
    int nrdispari, nrpari;

    // Controllo sul numero di parametri
    if (argc < 2)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 2\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;

    // Creazione del file fcreato
    if ((fcreato = creat("fcreato.log", 0644)) < 0)
    {
        printf("Errore nella creazione del file di log\n");
        exit(2);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *piped_pari = calloc(N, sizeof(pipe_t));
    pipe_t *piped_dispari = calloc(N, sizeof(pipe_t));
    if (piped_pari == NULL || piped_dispari == NULL)
    {
        printf("Errore in calloc.\n");
        exit(3);
    }

    // Creazione dei canali di pipe pari e dispari e controllo esito
    for (int n = 0; n < N; n++)
    {
        if ((pipe(piped_pari[n])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(4);
        }

        if ((pipe(piped_dispari[n])) < 0)
        {
            printf("Errore nella creazione della pipe.\n");
            exit(5);
        }
    }

    // Generazione dei processi figli
    for (int n = 0; n < 2 * N; n++)
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
            //printf("DEBUG-Esecuzione processo FIGLIO con PID = %d e indice n = %d associato al file %s\n", getpid(), n, argv[n / 2 + 1]);

            // Chiusura lati di lettura/scrittura inutilizzati
            for (int k = 0; k < N; k++)
            {
                close(piped_pari[k][0]);
                close(piped_dispari[k][0]);
                if (k != (n / 2))
                {
                    close(piped_pari[k][1]);
                    close(piped_dispari[k][1]);
                }
            }

            // Chiusura della pipe in eccesso rimasta aperta da ciclo precedente
            if (n % 2 == 0)
            {
                close(piped_dispari[n][1]);
            }
            else
            {
                close(piped_pari[n][1]);
            }

            // Apertura del file e controllo esito
            if ((fd = open(argv[n / 2 + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[n / 2 + 1]);
                exit(-1);
            }

            // Lettura di ciascun carattere del file e inserimento nella linea corrente
            int j = 0;
            indiceLinea = 0;
            nro = 0;
            while (read(fd, &(linea[j]), 1) != 0)
            {
                // Trovato il termine di una linea
                if (linea[j] == '\n')
                {
                    indiceLinea++;
                    j++;

                    // Invio della linea al padre tramite pipe
                    if ((n % 2 == 0) && (indiceLinea % 2 == 0))
                    {
                        // printf("DEBUG - Processo %d scrive su pipe %d\n", n, j);
                        write(piped_pari[n/2][1], &j, sizeof(j));

                        // Salvataggio della lunghezza massima
                        if (j > nro)
                        {
                            nro = j;
                        }
                    }

                    if ((n % 2 != 0) && (indiceLinea % 2 != 0))
                    {
                        // printf("DEBUG - Processo %d scrive su pipe %d\n", n, j);
                        write(piped_dispari[n/2][1], &j, sizeof(j));

                        // Salvataggio della lunghezza massima
                        if (j > nro)
                        {
                            nro = j;
                        }
                    }

                    // Azzeramento indice di posizione
                    j = 0;
                }
                else
                {
                    // Aggiornamento indice
                    j++;
                }
            }

            // Si ritorna al padre il massimo delle lunghezze calcolate
            exit(nro);
        }
    }

    /* Codice del processo padre */

    // Chiusura lati di scrittura inutilizzati
    for (int k = 0; k < N; k++)
    {
        close(piped_pari[k][1]);
        close(piped_dispari[k][1]);
    }

    // Per ciascun file
    for (int n = 0; n < N; n++)
    {
        printf("Dati realtivi al file %s\n", argv[n + 1]);

        // Lettura della lunghezza di ciascuna linea
        indiceLinea = 0;
        while (1)
        {
            // Lettura struttura dati proveniente da pipe e controllo esito
            nrdispari = read(piped_dispari[n][0], &len, sizeof(len));
            // printf("DEBUG - letti dati da pipe dispari nr = %d\n", nrdispari);
            if (nrdispari != 0)
            {
                indiceLinea++;
                printf("Linea di indice %d risulta lunga %d\n", indiceLinea, len);
            }

            // Lettura struttura dati proveniente da pipe e controllo esito
            nrpari = read(piped_pari[n][0], &len, sizeof(len));
            // printf("DEBUG - letti dati da pipe pari nr = %d\n", nrpari);
            if (nrpari != 0)
            {
                indiceLinea++;
                printf("Linea di indice %d risulta lunga %d\n", indiceLinea, len);
            }

            if (nrdispari == 0 && nrpari == 0)
            {
                break;
            }
        }
    }

    // Attesa dei processi figli
    for (int n = 0; n < 2 * N; n++)
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
            sprintf(linea, "Figlio con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);
            write(fcreato, linea, strlen(linea));
        }
    }

    // Uscita corretta dal programma
    exit(0);
}