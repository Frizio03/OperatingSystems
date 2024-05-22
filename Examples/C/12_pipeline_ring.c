#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

//Definizione del TIPO pipe_t come array di 2 interi
typedef int pipe_t[2];

int main(){
    
    //Dichiarazione variabili
    int pid, pidFiglio, status, ritorno;
    int num, nr, nw;

    //Salvataggio del numero N
    int N = 4;
    
    //Allocazione della memoria per array di pipe e controllo esito
    pipe_t* piped = malloc(N*sizeof(pipe_t));
    if (piped == NULL){
        printf("Errore in malloc.\n");
        exit(2);
    }

    /*
    Ogni processo di indice i legge dalla pipe di indice i e scrive all'interno
    della pipe di indice (i+1)%N in modo che l'ultimo figlio scriva su piped[0][1]
    */
    
    //Creazione dei canali di pipe e controllo esito
    for(int i = 0; i < N; i++){
        if ((pipe(piped[i])) < 0){
            printf("Errore nella creazione della pipe.\n");
            exit(3);
        }
    }
    
    printf("DEBUG-Esecuzione processo PADRE con PID = %d\n", getpid());

    //Generazione dei processi figli
    for(int i = 0; i < N; i++){
        //Esecuzione della fork e controllo esito
        if((pid = fork()) < 0){
            printf("Errore in fork.\n");
            exit(4);
        }
    
        if(pid == 0){
            /*Codice del processo figlio*/
            printf("DEBUG-Esecuzione processo FIGLIO con PID = %d\n", getpid());

            //Chiusura lati di pipe inutilizzati
            for(int k = 0; k < N; k++){
                if(k != i){
                    //Chiusura pipe lettura eccetto che per l'indice corrente
                    close(piped[k][0]);
                }
                if(k != (i+1)%N){
                    //Chiusura pipe di scrittura eccetto che quella di indice successivo
                    close(piped[k][1]);
                }
            }

            //Lettura struttura dati proveniente da pipe e controllo esito
            nr = read(piped[i][0], &num, sizeof(num));
            if(nr != sizeof(num)){
                printf("Errore nella read da pipe con indice %d\n", i);
                exit(-1);
            }

            printf("- Processo figlio di indice i = %d, letto valore di num = %d\n", i, num);
            num += 10;
            printf("- Processo figlio di indice i = %d, scrittura del valore di num = %d\n", i, num);

            //Scrittura struttura dati su pipe e controllo esito
            nw = write(piped[(i+1)%N][1], &num, sizeof(num));
            if(nw != sizeof(num)){
                printf("Errore nella scrittura su pipe con indice %d\n", i);
                exit(-1);
            }
            
            exit(i);
        }
    }
    
    //Chiusura delle pipe in scrittura e lettura (eccetto quella di indice 0: k = 1)
    for(int k = 1; k < N; k++){
        close(piped[k][0]);
        close(piped[k][1]);
    }

    sleep(2);

    num = 1;
    printf("- Processo padre sta per scrivere num = %d. INIZIO PIPELINE A RING\n", num);

    //Scrittura del dato iniziale su piped[0][1]
    nw = write(piped[0][1], &num, sizeof(num));
    if(nw != sizeof(num)){
        printf("Errore nella scrittura su pipe con indice %d\n", 0);
        exit(5);
    }

    //IMPORTANTE: chiusura lato di scrittura non più utilizzato
    close(piped[0][1]);

    sleep(2);

    //Attesa dei processi figli
    for(int i = 0; i < N; i++){
        //Esecuzione della wait e controllo esito
        if((pidFiglio = wait(&status)) < 0){
            printf("Errore in wait eseguita dal padre\n");
            exit(7);
        }
    
        //Controllo su terminazioni anomale del figlio e valore di ritorno
        if(WIFEXITED(status) == 0){
            printf("Figlio con PID = %d terminato in modo anomalo\n", pidFiglio);
        }
        else{
            ritorno = WEXITSTATUS(status);
            printf("Figlio con PID = %d ha ritornato %d (255 = errori)\n", pidFiglio, ritorno);
        }
    }
    
    //Uscita corretta dal programma
    exit(0);
}