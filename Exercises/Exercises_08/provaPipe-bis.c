#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe

int main(int argc, char** argv){

    //Dichiarazione variabili
    int fd1, fd2;
    int piped[2];

    //Controllo sul numero di parametri
    if(argc != 3){
        printf("Parametri richiesti: 2, passati: %d\n", argc-1);
        exit(1);
    }

    if((fd1 = open(argv[1], O_RDONLY)) < 0){
        printf("Errore nell'apertura di %s\n", argv[1]);
        exit(2);
    }

    if((fd2 = open(argv[2], O_RDONLY)) < 0){
        printf("Errore nell'apertura di %s\n", argv[1]);
        exit(3);
    }

    close(fd1);

    if(pipe(piped) < 0){
        printf("Errore in pipe\n");
        exit(4);
    }

    printf("Valori piped[0] = %d, piped[1] = %d\n", piped[0], piped[1]);
    printf("Valore di fd2 = %d\n", fd2);

    //Terminazione corretta del processo
    exit(0);
}