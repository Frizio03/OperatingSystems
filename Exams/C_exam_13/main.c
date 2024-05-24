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

typedef struct {
    char c1[12];
    int c2;
} Strut;

int main(int argc, char** argv)
{
    // Variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    // Variabili per gestione file e dati
    int fd;
    Strut* cur;

    // Variabili di controllo read e write
    int nr, nw;

    //Controllo sul numero di parametri
    if (argc < 4){
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 4\n", argc);
        exit(1);
    }
    
    //Salvataggio del numero parametri passati da linea di comando
    int N = argc - 2;
    
    // Apertura del file e controllo esito
    if((fd = open(argv[1], O_RDONLY)) < 0)
    {
        printf("Errore nell'apertura del file '%s'.\n", argv[1]);
        exit(2);
    }
    close(fd);

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t* piped = calloc(N, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(3);
    }
    
    // Creazione dei canali di pipe pipeline e controllo esito
    for(int n = 0; n < N; n++)
    {
        if ((pipe(piped[n])) < 0){
            printf("Errore nella creazione della pipe.\n");
            exit(4);
        }
    }
    
    // Generazione dei processi figli
    for(int n = 0; n < N; n++)
    {
        // Esecuzione della fork e controllo esito
        if((pid = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(5);
        }
    
        if(pid == 0){
            /* Codice del processo figlio */

            // Chiusura lati di pipe inutilizzati
            for(int k = 0; k < N; k++)
            {
                if(n == 0 || k != n-1)
                    close(piped[k][0]);
                if(k != n)
                    close(piped[k][1]);
            }
            
            // Generazione di un processo nipote e controllo esito
            if ((pid = fork()) < 0)
            {
                printf("Errore in fork.\n");
                exit(-1);
            }
            
            if(pid == 0)
            {
                /*Codice del processo nipote*/

                // Implementazione della ridirezione di stdout su pipe
                close(1);
                open("/dev/null", O_WRONLY);

                execlp("grep", "grep", argv[n+2], argv[1], NULL);

                // Codice che viene eseguito solo in caso di fallimento della exec
                printf("Errore in exec!\n");
                exit(-1);
            }
            
            /* Proseguimento codice del processo figlio */

            //Creazione dell'array di strutture dati
            cur = calloc(n+1, sizeof(Strut));
            if(cur == NULL){
                printf("Errore in malloc eseguita dal processo figlio con n = %d\n", n);
                exit(-1);
            }

            // Attesa del processo nipote
            if((pidFiglio = wait(&status)) < 0)
            {
                printf("Errore in wait eseguita dal figlio.\n");
                exit(-1);
            }
            
            //Controllo su terminazioni anomale del nipote e valore di ritorno
            if((status & 0xFF) != 0)
            {
                printf("DEBUG-Nipote con PID = %d terminato in modo anomalo\n", pidFiglio);
            }
            else{
                ritorno = (status >> 8) & 0xFF;
                printf("DEBUG-Nipote con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);
            }
            
            //Lettura struttura dati proveniente da pipe e controllo esito (solo per figli successivi)
            if(n != 0){
                nr = read(piped[n-1][0], cur, n*sizeof(Strut));
                if(nr != n*sizeof(Strut))
                {
                    printf("Errore nella read da pipe con indice %d\n", n);
                    exit(-1);
                }
            }

            //Se il comando grep trova la stringa ritorna 0: impostazione valori struttura dati
            if(ritorno == 0){
                strcpy(cur[n].c1, "TROVATA\0");
            }
            else{
                strcpy(cur[n].c1, "NON TROVATA\0");
            }

            //Inserimento del pid del processo nipote
            cur[n].c2 = pid;

            //Invio in avanti della struttura dati e controllo esito
            nw = write(piped[n][1], cur, (n+1)*sizeof(Strut));
            if(nw != (n+1)*sizeof(Strut))
            {
                printf("Errore nella scrittura su pipe con indice %d\n", n);
                exit(-1);
            }
            
            // Viene ritornato al padre il valore ritornato dal nipote
            exit(ritorno);
        }
    }
    
    /* Codice del processo padre */
    
    // Chiusura lati di pipe inutilizzati
    for(int k = 0; k < N; k++)
    {
        if(k != N-1)
            close(piped[k][0]);
    
        close(piped[k][1]);
    }
    
    // Allcazione memoria per ricezione array
    cur = calloc(N, sizeof(Strut));
    if(cur == NULL){
        printf("Errore in allocazione array di struct del PADRE\n");
        exit(6);
    }

    //Lettura struttura dati proveniente da pipe e controllo esito
    nr = read(piped[N-1][0], cur, N*sizeof(Strut));
    if(nr != N*sizeof(Strut))
    {
        printf("Errore nella read da pipe con indice %d\n", N-1);
    }
    
    for (int n = 0; n < N; n++)
    {
        printf("Figlio di indice n = %d e PID = %d ha cercato la stringa '%s' nel file '%s'. La stringa risulta %s\n",
        n, cur[n].c2, argv[n+2], argv[1], cur[n].c1);
    }
    
    //Attesa dei processi figli
    for(int n = 0; n < N; n++)
    {
        //Esecuzione della wait e controllo esito
        if((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(7);
        }
    
        //Controllo su terminazioni anomale del figlio e valore di ritorno
        if((status & 0xFF) != 0)
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