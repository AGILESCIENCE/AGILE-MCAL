#!/bin/bash
# User define Function (UDF)
processLine(){
  line="$@" # get all args
#  echo $line
  TYPE=$(echo $line | awk '{ print $8 }')
  echo $TYPE
  if [ $TYPE == "GRB" ]
  then
  	test -d $outpath || mkdir $outpath
  	mcalanalyzer /AGILE_DATA/COR/$i/PKP$i\_1_3908_000.cor.gz 
	T0=$(echo $line | awk '{ print $3 }')
	plotgrb -r $MCALSW/bin/mcalsw_output/RT/RT$i\_3908.root -t0 $T0 -100 30 -eband 0.35 1. -batch -o $outpath -save
		
	if [ $SEND_MAIL == "1" ]
	then
		ATT=""
		for filename in $outpath/MCAL_$i*
		do
			#echo "working on file " $filename
			ATT=$ATT" -a "$filename
		done
		nail -s 'MCAL 3908 GRB alert' -A mcal $ATT mcal@iasfbo.inaf.it, bulgarelli@iasfbo.inaf.it, agile_grid_burst@iasfbo.inaf.it  < $outpath/mcal_grb3908_msg.txt
		nail -s 'MCAL 3908 GRB alert' -A mcal mcal@iasfbo.inaf.it, bulgarelli@iasfbo.inaf.it, agile_grid_burst@iasfbo.inaf.it  < $outpath/mcal_grb3908_msg.txt
#		nail -s 'MCAL 3908 GRB alert' -A mcal $ATT marisaldi@iasfbo.inaf.it  < $outpath/mcal_grb3908_msg.txt
#		nail -s 'MCAL 3908 GRB alert' -A mcal marisaldi@iasfbo.inaf.it  < $outpath/mcal_grb3908_msg.txt
	fi
	
	FILE1=$outpath"/grb_info.dat"
	T90=$(awk  '{ print $7 }' $FILE1)
	echo $T90
	
	T90THR="1."
	if [ $T90 -lt $T90THR ] 
	then
		echo "this is a TGF"
	else 
		echo "this is a GRB" 
	fi
	
	
#	mv $outpath /AGILE_DATA/HLT/MCALBURST/GRB/
  fi
}

### Main script stars here ###

# usage: source grb3.sh <contact # (6 cifre)> <absolute output path>

# Make sure we get file name as command line argument 
# Else read it from standard input device
export i=$1
export outpath=$2
export SEND_MAIL=0
FILE="tmp_grb.dat"
if test -f $FILE
then
	rm $FILE
fi
#rm tmp_grb.dat
if test -f /AGILE_DATA/COR/$i/PKP$i\_1_3908_000.cor.gz
then
	mcalanalyzer /AGILE_DATA/COR/$i/PKP$i\_1_3908_000.cor.gz -grb tmp_grb.dat -noproc
	exec 3<&0
	exec 0<$FILE
	while read line
	do
		# use $line variable to process line in processLine() function
		processLine $line
	done
#	grep $i $FILE >> /AGILE_DATA/HLT/MCALBURST/mcal_grb.dat
	exec 0<&3	
fi

#exit 0
