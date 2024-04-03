#!/bin/sh

#Mi sposto all'interno della gerarchia o sotto-gerarchia
cd $1

#Variabile di controllo su dir corrente
condizione=0

#Controllo se la directory corrente ha nome idoneo
case `pwd` in
*/$2)
	echo "DEBUG-la directoy corrente HA nome idoneo"
	#Se la directory corrente ha nome idoneo controllo ogni file
	for e in *
	do
		#Se l'elemento è un file leggibile...
		if test -f $e -a -r $e
		then
			#Controllo il contenuro del file
			R=`grep '[0-9]' $e | wc -c`
			echo "DEBUG-Stampo valore di R: $R"

			if test $R -gt 0
			then
				condizione=1
			fi
		fi
	done
;;
*)
	echo "DEBUG-la directoy corrente NON HA nome idoneo"
;;
esac

#Se sono stati superati i controlli aggiungo la directory corrente all'elenco
if test $condizione -eq 1
then
	echo `pwd` >> $3
fi

#Esplorazione ricorsiva
for i in *
do
	if test -d $i -a -x $i
	then
		FCR.sh `pwd`/$i $2 $3
	fi
done
