for filename in *_3902_*
do
        echo ...working on file $filename
	mcalanalyzer $filename -u >> $1
done

