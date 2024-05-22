#include <stdio.h>    //Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>   //Per poter usare la primitiva exit
#include <unistd.h>   //Per poter usare le primitive
#include <sys/wait.h> //Per poter usare la primitiva wait
#include <fcntl.h>    //Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>   //Utilizzo delle stringhe
#include <ctype.h>    //Utilizzo di islower etc.

// Definizione tipo pipe
typedef int pipe_t[2];

// Definizione della struttura dati
typedef struct
{
    int pidNipote;         // Campo c1 del testo
    char ultimaLinea[250]; // Campo c2 del testo
    int len;               // Campo c3 del testo
} Strut;

int main(int argc, char **argv)
{

    // Dichiarazione variabili
    int pid, pidFiglio;
    int status, ritorno;
    char buffer[250];
    Strut S;
    int j;

    // Controllo del numero di parametri
    if (argc < 3)
    {
        printf("Parametri richiesti: >=2, passati: %d\n", argc - 1);
        exit(1);
    }

    // Ricavo il valore di N
    int N = argc - 1;

    // Allocazione di memoria
    pipe_t *piped = malloc(N * sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in malloc!\n");
        exit(2);
    }

    // Generazione delle pipe per comunicare con i figli
    for (int i = 0; i < N; i++)
    {
        if (pipe(piped[i]) < 0)
        {
            printf("Errore nella creazione della pipe!\n");
            exit(3);
        }
    }

    // Generazione dei processi figli
    for (int i = 0; i < N; i++)
    {

        if ((pid = fork()) < 0)
        {
            printf("Errore in fork!\n");
            exit(4);
        }

        if (pid == 0)
        {
            /*Codice del processo figlio*/

            // Chiusura di pipe inutilizzate
            for (int k = 0; k < N; k++)
            {
                close(piped[k][0]);
                if (k != i)
                    close(piped[k][1]);
            }

            // Array per la pipe figlio-nipote
            int p[2];

            // Creazione pipe figlio nipote
            if (pipe(p) < 0)
            {
                printf("Errore nella creazione della pipe figlio-nipote!\n");
                exit(-1);
            }

            // Generazione processo nipote
            if ((pid = fork()) < 0)
            {
                printf("Errore in fork per creazione nipote!\n");
                exit(-1);
            }

            if (pid == 0)
            {
                /*Codice del processo nipote*/

                // Chiusura pipe inutilizzate
                close(piped[i][1]);
                close(p[0]);

                // Implementazione della ridirezione (stdout)
                close(1);
                dup(p[1]);
                close(p[1]);

                // Esecuzione del comando reverse e scrittura su pipe (stdout)
                execlp("rev", "rev", argv[i + 1], NULL);

                // Codice eseguito in caso di errori nella exec
                printf("Erorre in exec del comando rev!\n");
                exit(-1);
            }

            /*Proseguimento codice del processo figlio*/

            // Chiusura lato pipe di scrittura
            close(p[1]);

            // Lettura dei dati ricevuti dal figlio
            j = 0;

            // Lettura delle linee dalla pipe
            while (read(p[0], &(buffer[j]), 1))
            {
                if (buffer[j] == '\n')
                {
                    buffer[j + 1] = '\0';
                    S.len = j + 1;
                    j = 0;
                }
                else
                {
                    j++;
                }
            }

            // Ora i dati salvati in buffer e j corrispondono all'ultima linea letta
            // printf("----> BUFFER = %s\n", buffer);
            // printf("----> LEN = %d\n", S.len);

            // Inserimento dati nella struct
            S.pidNipote = pid;
            memcpy(S.ultimaLinea, buffer, S.len);

            // Invio della struct al padre
            write(piped[i][1], &S, sizeof(S));

            // Attesa del processo nipote
            if ((pidFiglio = wait(&status)) < 0)
            {
                printf("Errore in wait eseguita dal figlio!\n");
                exit(-1);
            }

            // Controllo valore di ritorno
            if (WIFEXITED(status) == 0)
            {
                printf("Nipote con PID = %d terminato in modo anomalo\n", pidFiglio);
                exit(-1);
            }

            if (pid != pidFiglio)
            {
                printf("Il pid ritornato dalla wait non coincide con quello della fork!\n");
                exit(-1);
            }

            ritorno = WEXITSTATUS(status);
            printf("Nipote con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);

            // Viene ritornato al padre la len della ultima linea escluso il terminatore
            exit(j);
        }
    }

    /*Codice del processo padre*/

    // Chiusura delle pipe di scrittura inutilizzate
    for (int k = 0; k < N; k++)
    {
        close(piped[k][1]);
    }

    // Lettura ordinata delle struct e stampa dei dati
    for (int i = 0; i < N; i++)
    {
        read(piped[i][0], &S, sizeof(S));

        printf("\nDati della struct n. %d\n", i);
        printf("> PID nipote: %d\n", S.pidNipote);
        printf("> Ultima linea: %s\n", S.ultimaLinea);
        printf("> Lunghezza linea: %d\n", S.len);
    }

    // Attesa dei figli
    for (int i = 0; i < N; i++)
    {

        if ((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre!\n");
            exit(5);
        }

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