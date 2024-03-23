#-------------------------------- Esempi di utilizzo delle wildcard e metacaratteri --------------------------------

#37
mkdir provaDir
cd provaDir

#Creazione file per testare wildcard
> test1
> test2
> test3
> 1234
> test_only_char
> test_UPPERCASE

#Creazione directory per testare wildcard
mkdir testDir1
mkdir TESTDIR2
mkdir simple_dir

#Utilizzo delle wildcard e verifica della stampa

echo [0-9]*			#Stampo gli elementi con nome che INIZIA con un numero
echo *[0-9]			#Stampo gli elementi con nome che FINISCE con un numero
echo [A-Z]*			#Stampo gli elementi con nome che INIZIA con lettera MAIUSCOLA
echo *[A-Z]			#Stampo gli elementi con nome che FINISCE con lettera MAIUSCOLA
echo *[a-z]			#Stampo gli elementi con nome che finisce con lettera minuscola

#38
echo *[!a-z]		#Stampo gli elementi con nome che NON inizia con lettera minuscola

#39
ls -l [a-z]*		#Stampo diritti degli elementi che hanno nome  che inizia con lettera minuscola

#NB: in caso di matching con nome di directory che fanno match ls mostra il loro contenuto!!!