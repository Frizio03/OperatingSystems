#include <stdio.h> 	    // Per poter usare printf e altre funzioni di I/O
#include <stdlib.h>	    // Per poter usare la primitiva exit
#include <unistd.h>	    // Per poter usare le primitive
#include <sys/wait.h>	// Per poter usare la primitiva wait
#include <fcntl.h>	    // Per poter usare le costanti per la open (O_RDONLY, O_WRONLY e O_RDWR)
#include <string.h>	    // Utilizzo delle stringhe

// Si definisce il tipo pipe_y come array di due interi
typedef int pipe_t[2];

//Definizione della struttura dati
typedef struct {
    char v1;
    long int v2;
} dati;

//Definizione della funzione bubblesort
void bubbleSort(dati v[], int dim){
    int i;
    int ordinato = 0;

    while(dim > 1 && !ordinato){
        ordinato = 1;
        for(i = 0; i < dim-1; i++){
            if(v[i].v2 > v[i+1].v2){
                dati tmp = v[i];
                v[i] = v[i+1];
                v[i+1] = tmp;
                ordinato = 0;
            }
        }
        dim--;
    }
}

int main(int argc, char** argv){

    //Dichiarazione variabili
    int pidFiglio, status, ritorno;     //Variabili per pid e valori di ritorno figli
    int fd, N = 26, count;              //Variabili per salvataggio valori
    int nr, nw;                         //Variabili di controllo read e write
    char C = 'a', ch;                   //Variabili per salvataggio caratteri
    
    //Definizione degli array
    pipe_t pipes[N];
    dati d[N];
    int pid[N];

    //Controllo del numero di parametri
    if(argc != 2){
        printf("Parametri richiest: 1, passati: %d\n", argc-1);
        exit(1);
    }

    //Controllo sul nome del file
    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore nell'apertura del file '%s'\n", argv[1]);
        printf("File inesistente o permessi negati\n");
        exit(2);
    }

    //I procesi devono avere file pointer differenti!
    close(fd);

    printf("DEBUG-Processo PADRE con pid = %d\n", getpid());

    //Generazione delle pipe
    for(int i = 0; i < N; i++){
        if(pipe(pipes[i]) < 0){
            printf("Errore in pipe n. %d\n", i);
            exit(3);
        }
    }

    //Generazione degli N processi figli e della pipeline
    for(int i = 0; i < N; i++){

        //Controllo sulla fork
        if((pid[i] = fork()) < 0){
            printf("Errore in fork n. %d\n", i);
            exit(4);
        }

        if(pid[i] == 0){
            /*Codice del processo figlio*/
            printf("DEBUG-Processo figlio con pid = %d\n", getpid());

            //Inizializzazione variabili del figlio corrente
            C = C + i;
            count = 0;

            //Chiusura delle pipe inutilizzate
            for(int k = 0; k < N; k++){
                if(k != i){
                    //Chiusura pipe scrittura eccetto che per figlio corrente
                    close(pipes[k][1]);
                }
                if(i == 0 || k != i-1){
                    //Chiusura pipe di lettura eccetto che per figlio precedente
                    close(pipes[k][0]);
                }
            }

            //Apertura del file
            if((fd = open(argv[1], O_RDONLY)) < 0){
                printf("Errore nell'apertura del file '%s'\n", argv[1]);
                exit(-1);
            }

            //Lettura di ciascun carattere dal file
            while(read(fd, &ch, 1) != 0){
                if(ch == C){
                    count++;
                }
            }

            //Se non si è il primo figlio: lettura array dalla pipe
            if(i != 0){
                nr = read(pipes[i-1][0], d, sizeof(d));
                if(nr != sizeof(d)){
                    printf("Errore nella lettura da pipe[%d]\n", i);
                    exit(-1);
                }
            }

            //Inserimento dell informazioni ricavate nella posizione i
            d[i].v1 = C;
            d[i].v2 = count;

            //Invio delle informazioni al figlio (in avanti)
            nw = write(pipes[i][1], d, sizeof(d));
            if(nw != sizeof(d)){
                printf("Errore nella scrittura su pipe[%d]\n", i);
                exit(-1);
            }

            //Viene ritornato l'ultimo carattere letto
            exit(ch);
        }
    }

    /*Codice del padre*/

    //Chiusura delle pipe eccetto l'ultima in lettura
    for(int i = 0; i < N; i++){
        close(pipes[i][1]);
        if(i != N-1)
            close(pipes[i][0]);
    }

    nr = read(pipes[N-1][0], d, sizeof(d));
    
    if(nr != sizeof(d)){
        printf("Errore nella lettura da pipe da parte del padre\n");
    }
    else{
        //Ordinamento del vettore
        bubbleSort(d, N);

        //Stampa dei risultati
        for(int i = 0; i < N; i++){
            printf("Figlio di indice %d e PID = %d, ha trovato -- %ld -- occorrenze di -- %c --\n", d[i].v1-'a', pid[d[i].v1-'a'], d[i].v2, d[i].v1);
        }
    }

    //Attesa dei figli e dei valori di ritorno
    for(int i = 0; i < N; i++){
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait\n");
            exit(5);
        }

        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else{
            ritorno = WEXITSTATUS(status);
            printf("Figlio con PID = %d ha ritornato %d ossia '%c' (255 = errore)\n", pidFiglio, ritorno, ritorno);
        }
    }

    //Uscita corretta dal programma
    exit(0);
}