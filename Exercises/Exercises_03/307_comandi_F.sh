#!/bin/sh

#13
#Controllo sul numero di parametri
if test $# -ne 2
then
	echo Numero di parametri errato: inseriti $# richiesti 2!
	exit 1
fi

#Controllo se i parametri sono nomi relativi semplici
for var in $*
do
	case $var in
	/*) echo $var : questo è un percorso ASSOLUTO.
		exit 2
	;;
	*/*) echo $var : questo è un percorso RELATIVO.
		exit 3
	;;
	*) echo $var OK questo è un percorso RELATIVO SEMPLICE. ;;
	esac
done

#Controlli sul primo parametro
if test ! -f $1 -o ! -r $1
then
	echo Il file $1 non esiste oppure non è leggibile
	exit 4;
fi
echo OK! il file $1 esiste ed è leggibile

#Controlli sul primo parametro
if test ! -f $2
then
	echo Il file $2 NON esiste
	if test -d `pwd` -a -w `pwd`
	then
		echo OK si possiede il diritto di scrtittura sulla directory corrente
	else
		echo MALE si possiede il diritto di scrtittura sulla directory corrente
	fi
	exit 5
fi

if test ! -w $2
then
	echo Il file $2 esiste MA non è scrivibile
	exit 6
fi

echo OK! il file $2 esiste ed è scrivibile

#Debug di fine script
echo DEBUG: fine dello script