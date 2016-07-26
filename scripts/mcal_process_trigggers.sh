#!/bin/sh
export i=$1
while [ $i -le $2 ]
do
	mcalanalyzer /AGILE_PROC3/DATA/COR/0$i/PKP0$i\_1_3908_000.cor.gz -grb tmp_grb.dat -noproc
	#process3908 /AGILE_PROC3/DATA/COR/0$i/PKP0$i\_1_3908_000.cor.gz
	i=`expr $i + 1`
done
