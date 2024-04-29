#include <stdio.h> 	// Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	// Per poter usare la primitiva exit
#include <unistd.h>	// Per poter usare la primitiva close
#include <fcntl.h>	// Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	// Utilizzo delle stringhe
#define PERM 0644   // Permessi di default in ottale

int main(int argc, char** argv){

	//Inizializzazioni
	int fdin, fdout;
	int nread, nwrite;
	char buffer[BUFSIZ];  

	//Controllo del numero di parametri
	if (argc != 3){
		printf("Errore nel numero di parametri: argc = %d\n", argc);
		exit(1); 	
	}

	//Controllo del valore di ritorno della open
	if ((fdin = open(argv[1], O_RDONLY)) < 0){
		printf("Errore in apertura file per %s dato che fd = %d\n", argv[1], fdin);
        exit(2); 
	}	
	printf("Valore di fd = %d per %s\n", fdin, argv[1]);
	
	//Creazione file di output e controllo di eventuali errori nella creazione
	if ((fdout = creat(argv[2], PERM)) < 0){
		printf("Errore nella creazione file per %s dato che fdout = %d\n", argv[2], fdout);
		exit(3);
	}
	printf("Valore di fd = %d per %s\n", fdout, argv[2]);

	//Fino a quando riesco a leggere caratteri da fdin... (stdin = 0)
	while((nread = read(fdin, buffer, BUFSIZ)) > 0){

		//Scrittura caratteri su fdout (stdout = 1) 
		nwrite = write(fdout, buffer, nread);
		
		//Controllo se la scrittura è andata a buon fine
		if (nwrite < nread){
			printf("Errore nella scrittura dei dati su %s dato che nwrite = %d mentre nread = %d\n", argv[2], nwrite, nread);
			exit(4);
		}
	}
	
	//Valore di ritorno del programma eseguito correttamente
	exit(0);

	/*	
		NB: per comprendere appieno la sequenza di esecuzione delle istruzioni
		sostituire ripettivamente 0 e 1 al posto di fdin e fdout nella read() e nella write()	
	*/
}