#!/bin/sh
rm /home/mcal/local/mcalsw/bin/mcalsw_output/misc/MCAL_global_data.root
rm hktmp.root
export i=$1
while [ $i -le $2 ]
do
	mcalanalyzer /AGILE_DATA/COR/00$i/PKP00$i\_1_3913_000.cor.gz -u
	hkmonitor /AGILE_DATA/COR/00$i/PKP00$i\_1_3201_000.cor.gz hk$1\_$2\.root
	i=`expr $i + 1`
done
mv $MCALSW/bin/mcalsw_output/misc/MCAL_global_data.root ./$1\_$2\_MCAL_global_data.root
#plotmcaldata -b -f $1\_$2\_MCAL_global_data.root
#mv mcal_sci-rm_1.png $1\_$2\_mcal_rm.png
plotmcaldata -f $1\_$2\_MCAL_global_data.root -r 1 &
hkviewer -f hk$1\_$2\.root &
