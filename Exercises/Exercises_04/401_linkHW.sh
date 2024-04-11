#!/bin/sh

#01
mkdir provaDirBis
ls -al provaDirBis

#creazione di un file
> provaDirBis/file

#Listo le informazioni sul contenuto dell dir
ls -l provaDirBis
ls -al provaDirBis

#Rimozione del file e della directory
rm -i provaDirBis/file
rmdir provaDirBis
