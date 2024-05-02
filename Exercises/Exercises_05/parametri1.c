#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#define PERM 0644       // Permessi di default in ottale

int main(int argc, char** argv){

    //Controllo sul numero di parametri
    if(argc < 2){
        printf("Inserire almeno un parametro. Parametri inseriti %d\n", argc-1);
        exit(1);
    }

    //Stampa delle informazioni iniziali
    printf("Il programma eseguito si chiama %s\n", argv[0]);
    printf("Il numero di parametri inseriti: %d\n", argc-1);

    //Stampa di ciascun parametro
    for(int i = 0; i < argc-1; i++){
        printf("Parametro %d: %s\n", i+1, argv[i+1]);
    }

    //Termine del programma senza errori
    exit(0);
}