#!/bin/sh

# ------------------------- ESEMPI DI UTILIZZO COMANDO LINK (ln) -------------------------

#27

#Creazione di un file per test su ln
echo Sono il file C per prova dei link > dataFiles/fileC

#Verifico il numero di link del fileC
echo Il fileC possiede i segueti link:
ls -li dataFiles/fileC
echo

#Creo un nuovo linke per il file dentro a provaDir
ln dataFiles/fileC dataFiles/provaDir/linkFileC

#Verifico il numero di link del fileC (aumentato)
echo Il fileC possiede i segueti link:
ls -li dataFiles/fileC
echo

#Verifico il contenuto del file al nuovo link
cat dataFiles/provaDir/linkFileC
echo

#28

#Verifico il numero di link del file linkfileC
echo Il linkFileC possiede i segueti link:
ls -li dataFiles/provaDir/linkFileC
echo

#29

#Modifico il file tramite linkFileC
echo Aggiungo una riga tramite nuovo link >> dataFiles/provaDir/linkFileC
echo Mostro il contenuto di dataFiles/fileC:
cat dataFiles/fileC
echo

#30

#Elimino il fileC
rm dataFiles/fileC

#Verifico il numero di link del linkFileC
echo Il linkFileC possiede i seguenti link:
ls -li dataFiles/provaDir/linkFileC
echo
echo Il linkFileC contiene:
cat dataFiles/provaDir/linkFileC
echo