for filename in *_BURST.root
do
#        echo ...working on file $filename
	mcalselect $filename -m >> $1
done

