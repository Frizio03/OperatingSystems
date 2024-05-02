#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{   char buffer[BUFSIZ];
    int i = 0, N;
    int fd = 0;
    int finito = 0;
    int nread;
    
    //Non è necessario alcun controllo sui parametri

    //Ricavo il numero di parametri e lo metto in
	if (argc >= 2)	
		N = argc-1;

	while (!finito)
	{ 
		if (argc >= 2){
        	if ((fd = open(argv[i+1], O_RDONLY)) < 0){
                printf("Errore in apertura file %s\n", argv[i+1]);
               	exit(1); 
			}
        }
		i++;
		// Se non abbiamo un parametro, allora fd rimane uguale a 0
		while ((nread = read(fd, buffer, BUFSIZ)) > 0 ){
            //Scrittura su standard output
            write(1, buffer, nread);
        }
		if ((argc == 1) || (i == N)){  
			finito=1;
        }
	}

	exit(0);
}