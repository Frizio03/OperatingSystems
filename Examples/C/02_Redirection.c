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
    
    //Inizializzazione stringhe per stampa di debug
    char s1[100] = "Valore di fdin: ";
    char s2[5];
    char s3[] = " valore di fdout: ";
    char s4[] = "\n";

    //Controllo del numero di parametri
    if(argc != 3){
        printf("Errore nel numero di parametri: argc = %d\n", argc);
		exit(1); 	
    }

    //Tentativo di apertura del dispositivo terminale per stamparvi eventuali errori
    if((fdev = open("/dev/tty", O_WRONLY)) < 0){
        printf("Errore in apertura di %s: fdev = %d\n", "/dev/tty", fdev);
        exit(2);
    }

    //Chiusura del dispositivo (file) di stdin
    close(0);

    //Tentativo di apertura di un nuovo stdin con fdin = 0;
    if((fdin = open(argv[1], O_RDONLY)) < 0){
        printf("Errore in apertura di %s: fdin = %d\n", argv[1], fdin);
        exit(3);
    }

    //Chiusura del dispositivo (file) di stdout
    close(1);

    //Tentativo di apertura di un nuovo stdin con fdout = 1;
    if((fdout = open(argv[2], O_CREAT | O_WRONLY, PERM)) < 0){
        printf("Errore in apertura/creazione di %s: fdout = %d\n", argv[2], fdout);
        exit(4);
    }

    //STAMPE DI DEBUG su fdev

    //Creazione della stringa da stampare
    sprintf(s2, "%d", fdin);
    strcat(s1, s2);
    strcat(s1, s3);
    sprintf(s2, "%d", fdout);
    strcat(s1, s2);
    strcat(s1, s4);

    //Scrittura del messaggio di debug
    write(fdev, s1, strlen(s1));

    //NB: non posso utilizzare la printf perchè stampa su stdout!

	//Fino a quando riesco a leggere caratteri da stdin = 0...
	while((nread = read(0, buffer, BUFSIZ)) > 0){

		//Scrittura caratteri su stdout = 1
		nwrite = write(1, buffer, nread);
		
		//Controllo se la scrittura è andata a buon fine
		if (nwrite < nread){
			printf("Errore nella scrittura dei dati su %s dato che nwrite = %d mentre nread = %d\n", argv[2], nwrite, nread);
			exit(5);
		}
	}

	//Valore di ritorno del programma eseguito correttamente
    exit(0);
}