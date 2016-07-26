#!/bin/bash
# produce the file BOOST.dat necessary for the elimination of boost

# 	$1	COT_NAME		:complete path and name of temporary COT file
#	$2	BOOST_PREDICTION	:complete path and name of prediction for BOOST information necessary for on-line analysis
#	$3	BOOST_UP_NOW		:complete path and name for update the pre-existent BOOST file necessary for off-line analysis

#if test -f $MCALSW/share/mcal_conf/BOOST.dat
#then
#	mv $MCALSW/share/mcal_conf/BOOST.dat $MCALSW/share/mcal_conf/BOOST.dat.old
#fi

# cd $MCALSW/boost

if test -f COT.dat
then
	rm COT.dat
fi



#while [ 1 ] 
#do

	inputfile="start.in"
	if test -f $inputfile
	then
 		mv $inputfile $inputfile.old
	fi
	touch $inputfile
   	echo ".compile read_cot write_boost_info"  >> $inputfile	
   	echo "read_cot , OUT_PATH = '"$1"'"   >> $inputfile
   	echo "write_boost_info , COT_PATH = '"$1"' , BOOST_PREDICTION = '"$2"' , BOOST_UP_NOW = '"$3"'" >> $inputfile

   	idl < $inputfile  # >> & $logname

   	rm $inputfile
	cp $2 /AGILE_DATA/HLT/MCALBURST/BOOST.dat
	cp $3 /AGILE_DATA/HLT/MCALBURST/BOOST_up_to_now.dat
#done

