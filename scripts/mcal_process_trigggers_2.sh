#!/bin/sh
# good for orbits < 10000
export i=$1
while [ $i -le $2 ]
do
	#mcalanalyzer /AGILE_PROC3_GTB/DATA/COR/0$i/PKP0$i\_1_3908_000.cor.gz -grb tmp_grb.dat -noproc
	process3908 /AGILE_PROC3_GTB/DATA/COR/00$i/PKP00$i\_1_3908_000.cor.gz
	i=`expr $i + 1`
done
