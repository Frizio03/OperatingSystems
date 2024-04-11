#!/bin/sh

#07 - 08
ls -lR / > tempDir 2> tempError &
ps -lf

#09
echo
echo Stampo data e ora correnti
date

#10
diff file1 file2
echo $?

#11
diff file1 file2
echo ----------------------------------------
diff file2 file1