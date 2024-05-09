#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe

int main(){

    //Dichiarazione variabili
    int pid, val;

    //Richiest all'utente
    printf("Inserire un valore per l'invocazione ricorsiva (0 per terminare): ");
    scanf("%d", &val);

    if(val != 0){
        printf("Esecuzione dell'invocazione ricorsiva...\n");
        execl("./a.out", "", NULL);

        //Solo in caso di fallimento della exec
        printf("Errore in exec!\n");
        exit(1);
    }

    //Uscita corretta dal processo
    exit(0);
}