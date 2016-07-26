#! /bin/tcsh -f

#	INPUT PARAMETERS:
#	$1	HOME PATH TO WORK
#	$2	OUTPUT PATH
#	$3	number initial orbit
#	$4	number end orbit

set inputfile = ./start_local_GRB.in
if (-e $inputfile) then
	mv $inputfile $inputfile.old
endif
touch $inputfile

if ($#argv == 4) then
	# START GRB_search
	echo "\nlocal_GRB_search : ............................. Starting IDL  "
	echo "local_GRB_search : ............................. Compiling all IDL modules "
	echo "local_GRB_search : ............................. Searching GRB by MCAL RMs \n"
	
	echo ".COMPILE RMsReader_MCAL read_soe read_soe_index clean_saa_boost allign_previous_orbit range_orbit main" >> $inputfile
	echo "range_orbit,'"$1"', '"$2"'  , "$3" , "$4" "  >> $inputfile
	
	idl < $inputfile  # >> & $logname
else
	echo "\nIncorrect number of parameters"
	echo "for local_GRB.sh script \n"
	echo "SINTAX :\n"
	echo "local_GRB.sh /work_path/ /out_path/ min_orbit max_orbit\n\n"
endif
rm $inputfile
exit
