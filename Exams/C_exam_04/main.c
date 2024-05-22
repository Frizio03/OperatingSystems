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

// Definizione di MGSIZE
#define MGSIZE 6

int main(int argc, char **argv)
{

    // Variabili per gestione processi e file
    int pid, pidFiglio, status, ritorno;
    int fd, nr, nw;

    // Variabili per stringhe
    char linea[MGSIZE];
    char buffer[MGSIZE];

    // Controllo sul numero di parametri
    if (argc < 4)
    {
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 4\n", argc);
        exit(1);
    }

    // Salvataggio del numero parametri passati da linea di comando
    int N = argc - 1;

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t *pipes = calloc(N, sizeof(pipe_t));
    if (pipes == NULL)
    {
        printf("Errore in malloc.\n");
        exit(2);
    }

    // Creazione dei canali di pipe per pipeline e controllo esito
    for (int n = 0; n < N; n++)
    {
        if ((pipe(pipes[n])) < 0)
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
            /*Codice del processo figlio*/

            // Apertura del file associato e controllo esito
            if ((fd = open(argv[n + 1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[n + 1]);
                exit(-1);
            }

            // In caso il figlio sia il primo figlio
            if (n == 0)
            {

                // printf("DEBUG-Esecuzione processo FIGLIO P0 con PID = %d\n", getpid());

                // Chiusura pipe lettura inutilizzate
                for (int k = 0; k < N; k++)
                {
                    close(pipes[k][0]);
                }

                while ((nr = read(fd, linea, sizeof(linea))) != 0)
                {

                    if (nr != sizeof(linea))
                    {
                        printf("P0-Errore nella lettura della linea da file '%s'\n", argv[n + 1]);
                        exit(-1);
                    }

                    // Invio della linea ai processi fratelli
                    linea[MGSIZE - 1] = '\0';
                    for (int k = 1; k < N; k++)
                    {

                        // printf("DEBUG-P0: comunico %s al fratello %d\n", linea, k);

                        nw = write(pipes[k][1], linea, sizeof(linea));
                        if (nw != sizeof(linea))
                        {
                            printf("Errore nella scrittura su pipe dei fratelli\n");
                            exit(-1);
                        }
                    }
                }
            }

            // In caso il figlio sia uno dei figli successivi
            else
            {

                // printf("DEBUG-Esecuzione processo FIGLIO Pn con PID = %d\n", getpid());

                // Chiusura pipe scrittura inutilizzate e lettura di altri figli
                for (int k = 0; k < N; k++)
                {
                    close(pipes[k][1]);
                    if (k != n)
                    {
                        close(pipes[k][0]);
                    }
                }

                // Lettura dei dati inviati dal fratello P0
                while (read(pipes[n][0], buffer, sizeof(buffer)) != 0)
                {

                    // printf("DEBUG-P%d: letto buffer '%s'\n", n, buffer);

                    // Lettura della linea dal proprio file ripeartendo dall'inizio
                    lseek(fd, 0L, SEEK_SET);
                    while ((nr = read(fd, linea, sizeof(linea))) != 0)
                    {

                        // Trasformazione della linea in stringa e applicazione di diff
                        linea[MGSIZE - 1] = '\0';

                        // Generazione del processo nipote per ogni coppia buffer-linea
                        if ((pid = fork()) < 0)
                        {
                            printf("Errore in fork.\n");
                            exit(-1);
                        }

                        if (pid == 0)
                        {
                            /*Codice del processo nipote*/
                            // printf("DEBUG-Esecuzione processo NIPOTE con PID = %d\n", getpid());

                            // Chiusura del canale di pipe di lettura inutilizzato
                            close(pipes[n][0]);

                            // Ridirezione per valori stampadi da diff su stdout e stderr
                            close(1);
                            open("/dev/null", O_WRONLY);
                            close(2);
                            open("/dev/null", O_WRONLY);

                            execlp("diff", "diff", buffer, linea, NULL);

                            // Codice che viene eseguito solo in caso di fallimento della exec
                            printf("Errore in exec!\n");
                            exit(-1);
                        }

                        /*Proseguimento codice del processo figlio*/

                        // Attesa del processo nipote
                        if ((pidFiglio = wait(&status)) < 0)
                        {
                            printf("Errore in wait eseguita dal figlio.\n");
                            exit(-1);
                        }

                        // Controllo su terminazioni anomale del figlio e valore di ritorno
                        if (WIFEXITED(status) == 0)
                        {
                            printf("Nipote con PID = %d terminato in modo anomalo\n", pidFiglio);
                        }
                        else
                        {
                            ritorno = WEXITSTATUS(status);
                            // printf("DEBUG-Nipote con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);

                            // Il comando diff ritorna 0 se i file sono uguali
                            if (ritorno == 0)
                            {
                                printf("Figlio n = %d: il contenuto del file buffer = '%s' risulta UGUALE al file linea = '%s'\n",
                                       n, buffer, linea);
                            }
                            else
                            {
                                printf("Figlio n = %d: il contenuto del file buffer = '%s' risulta DIVERSO dal file linea = '%s'\n",
                                       n, buffer, linea);
                            }
                        }
                    }
                }
            }
            // Ciuascun figlio ritorna al padre il proprio numero d'ordine
            exit(n);
        }
    }

    /*Codice del processo padre*/

    // Chiusura di tutte le pipe non utilizzate dal padre
    for (int n = 0; n < N; n++)
    {
        close(pipes[n][0]);
        close(pipes[n][1]);
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
        if (WIFEXITED(status) == 0)
        {
            printf("Figlio di indice %d con PID = %d terminato in modo anomalo\n", n, pidFiglio);
        }
        else
        {
            ritorno = WEXITSTATUS(status);
            printf("Figlio di indice %d con PID = %d ha ritornato %d (255 = errori)\n", n, pidFiglio, ritorno);
        }
    }

    // Uscita corretta dal programma
    exit(0);
}