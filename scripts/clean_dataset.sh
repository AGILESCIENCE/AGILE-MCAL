#!/bin/sh
export i=5000
while [ $i -le 12585 ]
do
	#mcalanalyzer /AGILE_PROC3_GTB/DATA/COR/0$i/PKP0$i\_1_3908_000.cor.gz -grb tmp_grb.dat -noproc
	#process3908 /AGILE_PROC3_GTB/DATA/COR/0$i/PKP0$i\_1_3908_000.cor.gz
        #root -b -q 'burst_search_2.C("/MCAL_PROC/ARCHIVE/0'$i'/RT0'$i'_3908.root")'
	root -b -q 'clean_dataset.C('$i')' 
	i=`expr $i + 1`
done
