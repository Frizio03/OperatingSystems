#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#define MSGSIZE 5

int main (int argc, char **argv)
{   
   int pid, j, piped[2];
   char mess[MSGSIZE];
   int pidFiglio, status, ritorno;

	if (argc != 2)
	{	printf("Parametri richiesti: 1, passati %d\n", argc-1);
    	exit(1);
	}

    //Creazione della pipe
	if (pipe(piped) < 0 ){
        printf("Errore creazione pipe\n");
    	exit(2); 
	}

    //Creazione del processo figlio
	if ((pid = fork()) < 0){
        printf("Errore creazione figlio\n");
    	exit(3); 
	}

	if (pid == 0){   
        /* Codice del processo figlio */
        
        int fd;
        
        //Non effettuo la chiusura di piped[0];

        if ((fd = open(argv[1], O_RDONLY)) < 0){
            printf("Errore in apertura file %s\n", argv[1]);
                        exit(-1); /* torniamo al padre un -1 che sara' interpretato come 255 e quindi identificato come errore */
        }

        printf("DEBUG-Figlio %d sta per iniziare a scrivere una serie di messaggi, ognuno di lunghezza %d, sulla pipe dopo averli letti dal file passato come parametro\n", getpid(), MSGSIZE);
            
        j=0;
        while (read(fd, mess, MSGSIZE)){   
            mess[MSGSIZE-1]='\0'; 
            write(piped[1], mess, MSGSIZE);
            j++;
        }
		
        printf("DEBUG-Figlio %d scritto %d messaggi sulla pipe\n", getpid(), j);
		exit(j);
    }

	/* Codice del processo padre */
	
    //Non effettuo la chiusura di piped[0];
    
    printf("DEBUG-Padre %d sta per iniziare a leggere i messaggi dalla pipe\n", getpid());
	
    j=0;
    while (read(piped[0], mess, MSGSIZE)){ 	
        printf("%d: %s\n", j, mess);
        j++;
	}

	printf("DEBUG-Padre %d letto %d messaggi dalla pipe\n", getpid(), j);
	
    //Attesa del processo figlio
	if((pidFiglio = wait(&status)) < 0){
      	printf("Errore wait\n");
      	exit(4);
	}

	if(WIFEXITED(status) == 0){
        printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
    }
	else
	{
       	ritorno = WEXITSTATUS(status);
       	printf("Il figlio con PID = %d ha ritornato %d (se 255 problemi!)\n", pidFiglio, ritorno);
	}

	exit(0);
}