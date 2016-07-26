#!/bin/sh
# MM 15/10/12, based on mcal_burst_search_2.sh. Find high energy triggers ONLY
export i=27010
while [ $i -le 27010 ]
do
	#mcalanalyzer /AGILE_PROC3_GTB/DATA/COR/0$i/PKP0$i\_1_3908_000.cor.gz -grb tmp_grb.dat -noproc
	#process3908 /AGILE_PROC3_GTB/DATA/COR/0$i/PKP0$i\_1_3908_000.cor.gz
	root -b -q 'burst_search_3.C("/MCAL_PROC/ARCHIVE/0'$i'/RT0'$i'_3908.root")' 
	i=`expr $i + 1`
done
