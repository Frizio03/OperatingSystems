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

// Definizione del tipo linea di 250 char (da specfica)
typedef char line_t[250];

int main(int argc, char** argv)
{
    //Variabili per gestione processi
    int pid, pidFiglio, status, ritorno;

    //Variabili per file
    int fd, fdout;
    line_t linea;

    //Controllo sul numero di parametri
    if (argc < 3){
        printf("Numero di parametri errato: argc = %d, ma dovrebbe essere >= 3\n", argc);
        exit(1);
    }
    
    //Salvataggio del numero parametri passati da linea di comando
    int Q = argc - 1;
    
    // Creazione del file di output
    if((fdout = creat("out.txt", 0644)) < 0)
    {
        printf("Errore nella creazione del file\n");
        exit(2);
    }

    // Allocazione della memoria per array di pipe e controllo esito
    pipe_t* piped = calloc(Q, sizeof(pipe_t));
    if (piped == NULL)
    {
        printf("Errore in calloc.\n");
        exit(3);
    }
    
    // Creazione dei canali di pipe figlio-padre e controllo esito
    for(int q = 0; q < Q; q++)
    {
        if ((pipe(piped[q])) < 0){
            printf("Errore nella creazione della pipe.\n");
            exit(4);
        }
    }
    
    // Generazione dei processi figli
    for(int q = 0; q < Q; q++)
    {
        // Esecuzione della fork e controllo esito
        if((pid = fork()) < 0)
        {
            printf("Errore in fork.\n");
            exit(5);
        }
    
        if(pid == 0){
            /* Codice del processo figlio */
    
            // Chiusura lati di lettura/scrittura inutilizzati
            for(int k = 0; k < Q; k++)
            {
                close(piped[k][0]);
                if(k != q){
                    close(piped[k][1]);
                }
            }
            
            // Apertura del file e controllo esito
            if((fd = open(argv[q+1], O_RDONLY)) < 0)
            {
                printf("Errore nell'apertura del file '%s'.\n", argv[q+1]);
                exit(-1);
            }
            

            //Lettura di ciascun carattere del file linea del file
            int j = 0, inviate = 0;
            while(read(fd, &(linea[j]), 1) != 0)
            {
                //Trovato il termine di una linea
                if(linea[j] == '\n')
                {
                    j++;
                    //Se la linea inizia con un carattere numerico ed ha lunghezza < 10 la invio al padre
                    if(isdigit(linea[0]) && j < 10){
                        
                        // Scrittura dati su pipe: invio della linea al padre
                        write(piped[q][1], linea, j);
                        inviate++;
                    }

                    j = 0;
                }
                else
                {
                    //Aggiornamento indice
                    j++;
                }
            }
            
            exit(inviate);
        }
    }
    
    // Chiusura lati di scrittura inutilizzati
    for(int k = 0; k < Q; k++)
    {
        close(piped[k][1]);
    }
    
    // Per ciascun processo figlio
    for (int q = 0; q < Q; q++)
    {
        // Lettura ordinata delle linee inviate da pipe dei figli

        //Lettura di ciascun carattere del file linea del file
        int j = 0;
        while(read(piped[q][0], &(linea[j]), 1) != 0)
        {
            //Trovato il termine di una linea
            if(linea[j] == '\n')
            {
        
                // Inserimento linea su fdout
                j++;
                write(fdout, linea, j);

                // Trasformazione in stringa e stampa informazioni
                linea[j] = '\0';
                printf("Figlio di indice q = %d riferito al file '%s' ha inviato linea seguente:\n%s\n", q, argv[q+1], linea);
        
                j = 0;
            }
            else
            {
                //Aggiornamento indice
                j++;
            }
        }
    }
    
    //Attesa dei processi figli
    for(int q = 0; q < Q; q++)
    {
        //Esecuzione della wait e controllo esito
        if((pidFiglio = wait(&status)) < 0)
        {
            printf("Errore in wait eseguita dal padre\n");
            exit(6);
        }
    
        //Controllo su terminazioni anomale del figlio e valore di ritorno
        if(WIFEXITED(status) == 0)
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