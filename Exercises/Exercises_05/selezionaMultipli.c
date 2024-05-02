#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#define PERM 0644       // Permessi di default in ottale

int main(int argc, char** argv){

    //Dichiarazione variabili
    int fd, pos = 1;
    char curc;

    //Controllo sul numero di parametri
    if(argc != 3){
        printf("Inserire 2 parametri. Parametri inseriti %d\n", argc-1);
        exit(1);
    }

    //Controllo sul primo parametro
    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore: %s non corrisponde al nome di un file\n", argv[1]);
        exit(2);
    }

    //Controllo sul secondo parametro
    int N = atoi(argv[2]);
    if(N <= 0){
        printf("Errore: %s non numerico o nullo\n", argv[2]);
        exit(3);
    }

    while(read(fd, &curc, 1) != 0){
        
        //Se il carattere è in una posizione multipla di N, lo stampo
        if(pos % N == 0 && curc != '\n'){
            printf("Multiplo %d-esimo: carattere '%c' che si trova in posizione %d\n", pos/N, curc, pos);
        }
        pos++;
    }

    //Termine del programma senza errori
    exit(0);
}