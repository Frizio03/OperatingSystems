#!/bin/sh

echo ---------------------------------------------------------------------------
echo Controllo di ciascun parametro passato al file comandi
echo ---------------------------------------------------------------------------

#Ciclo for per iterare sui parametri passati
count=0
for par in $*
do
	count=`expr $count + 1`
	
	#Controllo sul tipo di percorso
	case $par in
	/*)
		#Se il parametro inizia con / allora ho un nome assoluto
		echo Il parametro $count ha nome ASSOLUTO: $par
	;;
	*/*)
		#Se il parametro contiene / al suo interno allora ho nome relativo
		echo Il parametro $count ha nome RELATIVO: $par
	;;
	*)
		#Giunti a questo caso allore il parameto non contiene / quindi è semplice
		echo Il parametro $count ha nome RELATIVO SEMPLICE: $par
	;;
	esac
done

echo ----------------------------------------------------------------------------
echo Controllo se il parametro passato è numero o stringa alfanumerica
echo ----------------------------------------------------------------------------

for par in $*
do
	case $par in
	*[!0-9]*)
		#Se il nome fa match conqualsiasi valore che non sia 0-9...
		echo MALE! Questo non è un numero: $par ;;
	*)
		echo OK! Questo è un numero: $par ;;
	esac
done

echo ----------------------------------------------------------------------------
echo Controllo se il parametro passato è una stringa di sole lettere minuscole
echo ----------------------------------------------------------------------------

for par in $*
do
	case $par in
	*[!a-z]*)
		#Se il nome fa match conqualsiasi valore che non sia in a-z...
		echo MALE! Questo nome contiene vari caratteri speciali o maiuscole: $par ;;
	*)
		echo OK! Questo nome contiene solo lettere minuscole: $par ;;
	esac
done

echo ----------------------------------------------------------------------------
echo Controllo se il parametro passato è un singolo carattere
echo ----------------------------------------------------------------------------

for par in $*
do
	case $par in
	?)
		echo OK! Questo parametro passato è un carattere singolo: $par ;;
	*)
		echo MALE! Questo parametro non è un singolo carattere: $par ;;
	esac
done

