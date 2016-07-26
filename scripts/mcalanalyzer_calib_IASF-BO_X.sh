for runid in 61 124 335 
do
        filename=$(find ./ -iname *${runid}_?_burst2_000.lv1)
        echo ...working on file $filename
	mcalanalyzer $filename -u >> $1
done

