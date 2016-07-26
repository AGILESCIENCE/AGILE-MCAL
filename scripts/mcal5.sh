#!/bin/sh
# apply ruby script grb3.rb to a selected range of orbits
export i=$1
while [ $i -le $2 ]
do
	if test -f /AGILE_DATA/COR/00$i/PKP00$i\_1_3908_000.cor.gz
	then
	#exec ruby grb3.rb "'00$i' '/home/mcal/data_analysis/00$i'"
	ruby grb3.rb 00$i /home/mcal/data_analysis/00$i
	fi
	i=`expr $i + 1`
done

