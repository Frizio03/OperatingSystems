#!/bin/sh

#Mi sposto nella directory di interesse
cd $1

#Controllo il nome della gerarchia corrente
case $1 in
	#Se il nome è composto da almeno 3 caratteri con C come carattere dispari
	*/$2?$2)
		#Scrivo la posizione corrente sul file temporaneo
		pwd >> $3 ;;
	*) ;;
esac

#Scorro 
for i in *
do
    if test -d $i -a -x $i 
    then
        echo Directory: $i
        #Inovoczione ricorsiva
        FCR.sh `pwd`/$i $2 $3
    fi
done