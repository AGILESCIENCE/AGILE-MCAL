for filename in *_3201_*
do
	hkmonitor $filename $1
done
hkviewer -f $1
