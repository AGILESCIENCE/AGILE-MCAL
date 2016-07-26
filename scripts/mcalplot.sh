# scientific ratemeters analysis
rm $MCALSW/bin/mcalsw_output/misc/MCAL_global_data.root
for filename in *_3913_*
do
        echo ...working on 3913 file $filename
	mcalanalyzer $filename -u >> tmp.log
done
plotmcaldata -b -p ./
rm tmp.log

# HK analysis
rm ./hktmp.root
for filename in *_3201_*
do
        echo ...working on 3201 file $filename
	hkmonitor $filename hktmp.root
done
hkviewer -b -p ./ -f hktmp.root
