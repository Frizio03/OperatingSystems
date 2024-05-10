#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe
#include <ctype.h>      //Utilizzo di isalpha() e isdigit()

int main(int argc, char** argv){

    //Dichiarazioni iniziali
    int pid, pidFiglio, fd;
    int status, ritorno;
    char ch;

    //Definizione array per canali di pipe --> 0 dispari, 1 pari
    int p[2][2];

    //Controllo del numero di parametri
    if(argc < 3){
        printf("Parametri richiesti: >= 2, passati: %d\n", argc-1);
        exit(1);
    }

    //Generazione canali di pipe
    if(pipe(p[0]) < 0){
        printf("Errore in prima pipe\n");
        exit(2);
    }
    if(pipe(p[1]) < 0){
        printf("Errore in seconda pipe\n");
        exit(3);
    }

    //Generazione dei processi figli
    int N = argc-1;
    printf("DEBUG-Procesos padre con PID = %d, inizio generazione figli\n", getpid());
    for(int i = 0; i < N; i++){

        if((pid = fork()) < 0){
            printf("errore in fork n. %d\n", i);
            exit(4);
        }

        if(pid == 0){
            /*Codice proceso figlio*/

            //Chiusura pipe di lettura inurilizzate
            close(p[0][0]);
            close(p[1][0]);

            //Chiusura di pipe di scrittura pari o dispari
            close(p[i%2][1]);

            //Apertura del file associato al processo figlio
            if((fd = open(argv[i+1], O_RDONLY)) < 0 ){
                printf("Errore nell'apertura del file '%s'\n", argv[i+1]);
                exit(-1);
            }

            //Lettura di ciascun carattede del file associato
            while(read(fd, &ch, 1) != 0){

                //Se ch viene letto da un proceso di indice pari --> file dispari
                if(i%2 == 0 && isalpha(ch)){
                    //Scrittura su pipe di processi pari
                    write(p[(i+1)%2][1], &ch, 1);
                }

                //Se ch viene letto da un processo di indice dispari --> file pari
                if(i%2 == 1 && isdigit(ch)){
                    //Scrittura su pipe di processi dispari
                    write(p[(i+1)%2][1], &ch, 1);
                }
            }

            //Si ritorna l'ultimo carattere letto dal figlio
            exit(ch);
        }

    }

    /*Codice processo padre*/

    //Chiusura dei lati di scrittura inutilizati
    close(p[0][1]);
    close(p[1][1]);

    printf("Stampa dei caratteri inviati dai figli:\n");

    int nr0, nr1, tot = 0;
    char ch0, ch1;

    do{

        //Lettura dai caratteri di pipe
        nr0 = read(p[0][0], &ch0, 1);
        nr1 = read(p[1][0], &ch1, 1);

        //Stampa di un alfabetico e un numerico
        write(1, &ch0, 1);
        write(1, &ch1, 1);

        //Aggiornamento del totale di caratteri letti
        tot = nr0 + nr1;

    }while(nr0 != 0 || nr1 != 0);

    printf("\nNumero totale di caratteri letti dalle pipe: %d\n", tot);

    //Attesa dei figli e controllo valori di ritorno
    for(int i = 0; i < N; i++){

        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait n. %d\n", i);
            exit(5);
        }

        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo.\n", pidFiglio);
        }
        else{
            ritorno = WEXITSTATUS(status);
            printf("Figlio con PID = %d ha ritornato '%c' ossia %d (se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }

    //Uscita corretta dal processo
    exit(0);
}