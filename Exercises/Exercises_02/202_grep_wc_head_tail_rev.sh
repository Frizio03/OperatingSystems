#-------------------------------- Esempi di utilizzo del comando GREP --------------------------------

#11
grep mare < ordNoDoppi				#Cerco le linee che contengono la stringa "mare"

#12
grep -n mare < ordNoDoppi			#Cerco le linee e stampo il loro numero

#13
grep -i MARE < ordNoDoppi			#Cerco ignorando maiuscole/minuscole

#14
grep -v mare < ordNoDoppi			#Cerco le linee che non contengono "mare"

#15
grep '^ci' < ordNoDoppi				#Cerco le linee che iniziano per "ci"

#16
grep -n 'e$' < ordNoDoppi			#Cerco le linee che finiscono per e

#17
grep -n '\.$' < ordNoDoppi			#Cerco le linee che finiscono con .

#18
grep '\.$' < ordNoDoppi > exportGrep	#Cerco le linee che finiscono con . e le salvo in exportGrep

#19
grep 'ciao' ordNoDoppi > exportGrep	#Utilizzo del grep senza ridirezione in input

#-------------------------------- Esempi di utilizzo del comando WC --------------------------------

#20
wc -l < proveSort					#Conto il numero di linee del file

#21
wc -c < proveSort					#Conto il numero di caratteri del file

#22
wc -w < proveSort					#Conto il numero di parole del file

#23									#Stessi comandi ma senza ridirezione --> viene stampato il nomefile
wc -l proveSort
wc -c proveSort
wc -w proveSort

#24
wc < fileInesistente 2> /dev/null	#Eseguo wc su file inesistente...
echo $?								#...e controllo il calore di ritorno

#-------------------------------- Comando PS e TEE - PIPING --------------------------------

#25-a
ps -lf | tee fileTemp | wc -l		#Estraggo i processi e li salvo in fileTemp
									#Estraggo poi il numero di linee (processi) in filetemp

#25-b
cat fileTemp						#Visualizzo le informazioni sui processi

#-------------------------------- Comandi HEAD, TAIL, REV --------------------------------

#26-27-28
head < nuovoFile					#Ricavo le prime 10 righe di nuovoFile
head -1 < nuovoFile					#Ricavo la prima linea di nuovoFile
head -3 < nuovoFile					#Ricavo le prime 3 linee di nuovoFile

#29-30-31
tail < nuovoFile					#Ricavo le iltime 10 righe di nuovoFile
tail -1 < nuovoFile					#Ricavo l'ultima linea di nuovoFile
tail -3 < nuovoFile					#Ricavo le ultime 3 linee di nuovoFile

#32
tail -3 < nuovoFile > ultimeTre		#Ridirigo il risultato nel file ultimeTre

#33
tail -3 nuovoFile					#Senza ridirezione

#34
head -3 nuovoFile | tail -1 > terzaRiga		#Isolo la terza riga del file

#35
tail -3 nuovoFile | head -1 > terzaRiga		#Isolo la terzultima riga del file

#36
rev < nuovoFile > rovesciato				#Rovescio ogni liena del file