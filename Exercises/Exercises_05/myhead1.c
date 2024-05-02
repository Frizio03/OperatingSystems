#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#define PERM 0644       // Permessi di default in ottale

int main  (int argc, char **argv){
    
    //Dichiarazione delle variabili
    int lines = 1, n;
    char c;

	//Controllo sul numero di parametri
	if (argc != 2){ 	
		printf("Errore: Necessario 1 argomento (nel formato -numero) per %s\n", argv[0]); 
		exit (1); 
	}

    //Controllo sul simbolo
	if (argv[1][0] != '-')
	{ 	
		printf ("Errore: Necessario il simbolo di opzione '-'\n");
		exit (2); 
	}

    //Estrazione del numero N di linee
	n = atoi(&(argv[1][1]));
	if (n <= 0){
        printf("Errore: l'opzione non risulta numerica\n");
        exit(3);
	}

    //Lettura di un carattere alla volta da stdin
	while (read (0, &c, 1)){	 	
		if (c == '\n'){
            //Incremento il numero di linee
            lines++;
        }
		write(1, &c, 1);
		if (lines > n){
            //Interruzione del ciclo
            break;
        }
    }

	exit(0);
}