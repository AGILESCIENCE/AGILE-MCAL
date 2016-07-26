#!/bin/bash
# User define Function (UDF)
processLine(){
  line="$@" # get all args
  #  just echo them, but you may need to customize it according to your need
  # for example, F1 will store first field of $line, see readline2 script
  # for more examples
  # F1=$(echo $line | awk '{ print $1 }')
  echo $line
  TYPE=$(echo $line | awk '{ print $8 }')
  echo $TYPE
  if [ $TYPE == "GRB" ]
  then
  	mcalanalyzer /AGILE_DATA/COR/$i/PKP$i\_1_3908_000.cor.gz 
	T0=$(echo $line | awk '{ print $3 }')
	plotgrb -r $MCALSW/bin/mcalsw_output/RT/RT$i\_3908.root -t0 $T0 -100 30 -eband 0. 1. -batch
	mkdir /AGILE_DATA/HLT/MCALBURST/$i
	mv *.gif /AGILE_DATA/HLT/MCALBURST/$i
	
  fi
}

### Main script stars here ###
# Store file name
#FILE="tmp_mcal_grb.dat"
#FILE="tmp_grb.dat"

# Make sure we get file name as command line argument 
# Else read it from standard input device
export i=$1
FILE="tmp_grb.dat"
while [ $i -le $2 ]
do
#	if test -f $FILE
#	then
#		rm $FILE
#	fi
	rm tmp_grb.dat
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
		
		exec 0<&3	
	fi
	i=`expr $i + 1`
	
done

#exit 0
