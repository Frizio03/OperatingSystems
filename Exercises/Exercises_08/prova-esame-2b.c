#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#include <ctype.h>      //Utilizzo di isalpha() e isdigit()

//Tipo che definisce un canale di pipe
typedef struct{
    char c;
    long int n;
} tipoS;

int main(int argc, char** argv){

    //Dichiarazione variabili
    int pid, pidFiglio, fd;
    int status, ritorno;
    int piped[2];
    tipoS msg;

    //Controllo sul numero di parametri
    if(argc < 4){
        printf("Parametri richiesti >= 3, passati %d\n", argc-1);
        exit(1);
    }

    //Controllo sul primo parametro
    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore nell'apertura del file '%s'\n", argv[1]);
        exit(2);
    }
    close(fd);

    //Controllo dei successivi parametri
    for(int i = 2; i < argc; i++){
        if(strlen(argv[i]) != 1){
            printf("Errore: '%s' non singolo carattere\n", argv[i]);
            exit(3);
        }
    }

    //Ricavo il valore di N
    int N = argc -2;

    //Generazione della pipe
    if(pipe(piped) < 0){
        printf("Errore in pipe\n");
        exit(5);
    }

    //Generazione dei processi figli
    for(int i = 0; i < N; i++){

        if((pid = fork()) < 0){
            printf("errore in fork n. %d\n", i);
            exit(6);
        }

        if(pid == 0){
            /*Codice del processo figlio*/

            //Chiusura pipe di lettura inutilizzata
            close(piped[0]);

            //Apertura del file (nei figli: no confivisione file pointer)
            if((fd = open(argv[1], O_RDONLY)) < 0){
                printf("Errore nell'apertura del file '%s'\n", argv[1]);
                exit(-1);
            }

            //Lettura di ciascun carattere dal file
            char ch;
            while(read(fd, &ch, 1) != 0){
                
                //Aggiornamento conteggio
                if(ch == argv[i+2][0]){
                    msg.n++;
                }
            }

            msg.c = argv[i+2][0];
            
            //comunicazione con il padre usando la pipe associata
            write(piped[1], &msg, sizeof(msg));

            //Il figlio comunica come valore di irotrno il carattere cercato
            exit(argv[i+2][0]);
        }

    }

    /*Codice del processo padre*/

    //Chiusura pipe di scrittura inutilizzata
    close(piped[1]);

    //Lettura dei dalla pipe
    while(read(piped[0], &msg, sizeof(msg)) > 0){
        printf("Rilevate %ld occorrenze per il carattere '%c'\n", msg.n, msg.c);
    }

    //Attesa dei figli e controllo valori di ritorno
    for(int i = 0; i < N; i++){

        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait n. %d\n", i);
            exit(5);
        }

        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo.\n", pidFiglio);
        }
        else{
            ritorno = WEXITSTATUS(status);
            printf("Figlio con PID = %d ha ritornato '%c' ossia %d (se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }

    //Uscita corretta dal processo
    exit(0);
}