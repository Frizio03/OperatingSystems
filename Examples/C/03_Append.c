#include <stdio.h> 	// Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	// Per poter usare la primitiva exit
#include <unistd.h>	// Per poter usare la primitiva close
#include <fcntl.h>	// Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	// Utilizzo delle stringhe
#define PERM 0644   // Permessi di default in ottale

int main(int argc, char** argv){

    int fdin, fdout, fdev;  //Definizione variabili per i file descriptor
    char buffer[BUFSIZ];    //Definizione buffer per contenere i vari caratteri da gestire
    int nread, nwrite;      //Variabili per valori di ritorno di funzioni read e write

    //Controllo del numero di parametri
    if(argc != 2){
        printf("Errore nel numero di parametri: argc = %d\n", argc);
		exit(1); 	
    }

    //Tentativo di apertura/creazione di file di output
    if((fdout = open(argv[1], O_CREAT | O_WRONLY, PERM)) < 0){
        printf("Errore in apertura/creazione di %s: fdout = %d\n", argv[1], fdout);
        exit(2);
    }

    //Sposto il file pointer a 0 posti dal termine del file
    lseek(fdout, 0L, SEEK_END);

	//Fino a quando riesco a leggere caratteri da fdin
    printf("Inserire i caratteri da aggiungere in coda al file\n");
    printf("Premere CTRL+D per terminare\n");
	while((nread = read(0, buffer, BUFSIZ)) > 0){

		//Scrittura caratteri su fdout
		nwrite = write(fdout, buffer, nread);
		
		//Controllo se la scrittura è andata a buon fine
		if (nwrite < nread){
			printf("Errore nella scrittura dei dati su %s dato che nwrite = %d mentre nread = %d\n", argv[2], nwrite, nread);
			exit(3);
		}
	}

    exit(0);
}