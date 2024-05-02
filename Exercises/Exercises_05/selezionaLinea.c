#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#define PERM 0644       // Permessi di default in ottale

int main(int argc, char** argv){

    //Dichiarazione variabili
    int fd, nread;
    int curLine = 1;
    char buffer[256];
    int maxLen = 256;
    int len = 0, trovata = 0;

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

    int j = 0;
    while(read(fd, &(buffer[j]), 1) != 0){

        //Se il carattere corrente è \n allora sono in fondo alla linea
        if(buffer[j] == '\n'){
            
            if(curLine == N){
                //Inserisco il carattere terminatore e stampo la linea
                buffer[j+1] = 0;
                printf("Stampo la linea %d del file '%s':\n", N, argv[1]);
                printf("%s", buffer);
                trovata = 1;
                break;
            }
            else{
                //Aggiornamento indice buffer e numero linea
                j = 0;
                curLine++;
            }
        }
        else{
            //Incremento dell'indice del buffer
            j++;
        }
    }

    //Messaggio di linea non trovata
    if(!trovata){
        printf("Non è stato possibile trovare la linea %d del file '%s'\n", N, argv[1]);
    }

    //Termine del programma senza errori
    exit(0);
}