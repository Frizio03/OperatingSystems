#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define PERM 0644

int main (int argc, char** argv)
{
    int infile, outfile, nread;
    char buffer[BUFSIZ];
    printf("Stampo la dimensione INIZIALE di BUSFIZ: %d\n", BUFSIZ);

    //Controllo sul numero di parametri
	if (argc != 3){
        printf("Errore: numero di argomenti sbagliato dato che argc = %d\n", argc);
       	printf("Ci vogliono 2 argomenti: nome-file-sorgente e nome-file-destinazione\n");
       	exit(1); 
	}

    //Prova di apertura di un file
	if ((infile = open(argv[1], O_RDONLY)) < 0){
        printf("Errore in apertura file per %s dato che infile = %d\n", argv[1], infile);
		exit(2);
	}

    //Prova di creazione del file
	if ((outfile = creat(argv[2], PERM)) < 0 ){
        printf("Errore nella creazione file per %s dato che outfile = %d\n", argv[2], outfile);
		exit(3);
	}

	while ((nread = read(infile, buffer, BUFSIZ)) > 0 ){ 
		if (write(outfile, buffer, nread) < nread){
            printf("Errore nella scrittura del file per %s\n", argv[2]);
			exit(4);
		}
        printf("Stampo la dimensione di BUSFIZ: %d\n", BUFSIZ);
	}

	//Uscita corretta dal programma
	exit(0);
}