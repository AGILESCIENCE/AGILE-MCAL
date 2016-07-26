#! /bin/tcsh -f

#	INPUT PARAMETERS :
#	$1	HOME PATH TO WORK
#	$2	COMPLETE PATH OF 3913 FILE
#	$3	OUTPUT PATH
# 	$4	no_mail keyword

set inputfile = $1\start_GRB.in
if (-e $inputfile) then 
    mv $inputfile $inputfile.old
endif

touch $inputfile

if ($#argv >= 3) then
	#echo "GRB_search : ............................. Starting IDL  "
	#echo "GRB_search : ............................. Compiling all IDL modules  "
	#echo "GRB_search : ............................. Searching GRB by MCAL RMs  "
	echo "GRB_search : "
	
	echo ".COMPILE RMsReader_MCAL read_soe read_soe_index clean_saa_boost main " >> $inputfile
	echo "main , '"$1"' , '"$2"' , '"$3"' "  >> $inputfile
		
	cd $1
	
	idl < $inputfile  # >> & $logname
	
	set orbc=`expr index $2 PKP`
	set orbc=`expr $orbc + 3 `
	set orb=`expr substr $2 $orbc 6`
	
	if ( $4 == "")	then
			echo "running tool with sending mail enabled"
			ruby $1\mail_to_GRB.rb $3 $2 $orb
		else 
			echo "running local tool with no mail"
			ruby $1\local/mail_to_GRB_local.rb $3 $2 $orb
	endif
else
	echo "no parameters for GRB.sh"
endif

rm $inputfile

exit
