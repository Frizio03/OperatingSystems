#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#define PERM 0644       // Permessi di default in ottale

int main(int argc, char** argv){

    //Dichiarazione variabili
    int fd;

    //Controllo sul numero di parametri
    if(argc != 4){
        printf("Inserire 3 parametri. Parametri inseriti %d\n", argc-1);
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

    //Controllo sul terzo parametro
    if(argv[3][1] != 0){
        printf("Errore: %s non singolo carattere\n", argv[3]);
        exit(4);
    }

    //Stampa delle informazioni iniziali
    printf("Il programma eseguito si chiama %s\n", argv[0]);
    printf("Il numero di parametri inseriti: %d\n", argc-1);

    //Stampa di ciascun parametro
    printf("Stampo il primo parametro, nome di un file: %s\n", argv[1]);
    printf("Stampo il secondo parametro, numero intero: %s\n", argv[2]);
    printf("Stampo il terzo parametro: singolo carettere: %s\n", argv[3]);

    //Termine del programma senza errori
    exit(0);
}