#-------------------------------- Esempi di ridirezione ed utilizzo del sort con ridirezione --------------------------------

#01
touch fileEsempio						#Creazione del file
cp -p fileEsempio fileEsempioBackup		#Copia del file conservando timestamp
> fileEsempioBackup						#Azzeramento contenuto del file

#02
> nuovoFile								#Creazione nuovo file vuoto con ridirezione

#INIZIALIZZAZIONE SORT
LC_ALL=C
export LC_ALL

#03
sort -C < proveSort						#Verifico se il file è ordinato
echo $?									#controllando il valore di ritorno del comando

sort -c < proveSort						#Verifica dell'ordinamento e lettura errore
sort -c < proveSort 2> /dev/null		#Rideirezione del risultato su std out
echo $?

#04
sort < proveSort						#Stampo il sort prendendo in input nuovoFile

#05
sort < proveSort						#Stampo il sort inverso prendendo in input nuovoFile

#06
sort -f < proveSort						#Stampo il sort considerando maiuscole

#07
sort -f < proveSort	> fileOrdinato		#Stampo il sort considerando maiuscole nel file ordinato

#08
sort -C < fileOrdinato					#Verifico se il file è ordinato
echo $?

#09
sort -u < proveSort	> ordNoDoppi		#Stampo il sort senza linee doppie

#10
sort -uf < proveSort > ordNoDoppi		#Stampo il sort senza linee doppie e ordinando maiuscole