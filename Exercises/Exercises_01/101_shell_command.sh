#Visualizzare il contenuto della directory di sistema (root)
ls /

#Verificare il contenuto di /etc/passwd
cat /etc/passwd

#Visualizzare la directory corrente
pwd

#Visualizzare la directory di HOME indipendentemente dalla posizione
echo $HOME

#Visualizzare il contenuto di /etc/shadow
cat shadow #Permission denied!

#Visualizzare le informazioni relative all'id
id

#Visualizzare i processi della sessione corrente in forma estesa con relazioni di parentela
ps -fl

#Visualizzare i processi id sistema in forma estesa
ps -fle

#Visualizzare la guida del comando ps
man ps

#Verificare dove si trova il file eseguibile del comando ps
which ps

#Mostrare le informazioni estese di file1.txt
ls -l file1.txt

#Comando per mostrare i file nascosti
ls -a

#Comando per mostrare i file nascosti esclusi . e ..
ls -A

#Mostrare sotto gerarchia della directory corrente con informazioni dei file e loro tipo
ls -lRF

#Mostrare i nomi dei file per modifica più recente
ls -tl

#