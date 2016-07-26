pro main,home,RMs_names,path_output

;**************************************************************************************************
; 	INITIALIZE VARIABLES
;**************************************************************************************************

open_trigger=0
open_burst=0

;**************************************************************************************************
;	READ RMs DATA AND REBIN ENERGY CHANNELS AS DEFINED IN "channel_name" 
;**************************************************************************************************

channel_name=[' X_.17-.7',' X_.7-1.4','X_1.4-2.8','   X_>2.8',' Z_.17-.7',' Z_.7-1.4','Z_1.4-2.8','   Z_>2.8']
LV1=RMsReader_MCAL(RMs_names);'D:\Documenti\AGILE\Commissioning\PKP000552_1_3913_000.lv1.gz')
RMs=LV1.Data
contact=FIX(strmid(rms_names,strpos(rms_names,'PKP')+3,6))
print,format='(/,"working on file:    ",A0,/)',RMs_names
restore,path_output+'last_rms.sav'

RMs_tmp=[rms(0,*) , rms( 2,*)+rms( 3,*) , rms( 4,*) , rms( 5,*) , rms( 6,*)+rms( 7,*)+rms( 8,*)+rms( 9,*)+rms(10,*)+rms(11,*) , $
		rms(13,*)+rms(14,*) , rms(15,*) , rms(16,*) , rms(17,*)+rms(18,*)+rms(19,*)+rms(20,*)+rms(21,*)+rms(22,*)]
RMs=RMs_tmp
lv1={data :rms, time : lv1.time}
RMs_tmp=0

;**************************************************************************************************
;	DEFINE INITIAL & END TIME, SEARCHING AND READ SOE DATA (for only SAA orbital phase in this version)
;**************************************************************************************************

tstart=RMs(0,0)
tstop=RMs(0,n_elements(rms(0,*))-1)

;**************************************************************************************************
;	DEEFINE BKG WINDOW & DELAY TO SEARCHING FOR GRB --- 
;		DEFINE ALSO PARAMETER TO FILTER GOOD TRIGGER AND DETECT BURST
;**************************************************************************************************

BKG_SEC = 60
DELAY=30
N_SIGMA = 5.
min_sign=7.
min_duration=2

;**************************************************************************************************
;	DEFINE PATH AND NAME FOR OUTPUT FILE 
;**************************************************************************************************

trigger_path=strcompress(path_output+'LOG.trigger',/remove_all)
burst_path=strcompress(path_output+'LOG.burst',/remove_all)
light_curve_path=strcompress(path_output+'light_curve.launch',/remove_all)
GRB_path=strcompress(path_output+'GRB_'+string(contact)+'.ok',/remove_all)
trigger_DAT_path=strcompress(path_output+'trigger.dat',/remove_all)
burst_DAT_path=strcompress(path_output+'burst.dat',/remove_all)

;**************************************************************************************************
;       SEARCHING FOR INPUT FILE AND IF DO NOT EXSIST UPDATE "last_rms" VARIABLE TO 0 VALUE AND SKIP
; 		IF LAST_RMs IS LONG TIME DISTANT DO NOT APPEND
;**************************************************************************************************

r_name=file_search(RMs_names)
if (r_name(0) eq '') then begin
	print,'FILE NOT FOUND. END PROGRAM'
	last_rms=0
	save,last_rms,filename=path_output+'last_rms.sav'
	print,format='(/,"last_rms = 0 ",/)'
	goto, no_mail
endif

last_time = abs(tstart - last_rms(0,n_elements(last_rms(0,*))-1))

if (last_time le delay) then begin
	RMs=[[last_rms],[RMs]]
	print,format='(/,"APPENDED last_rms",/)'
endif else begin
	print,format='(/,"no last_rms",/)'
endelse

;**************************************************************************************************
;	CHANGE RMs DATA FOR SAA PERIOD AND TRANSITION POWER PERIOD (BOOST) TO 0 VALUE
;**************************************************************************************************

good=clean_saa_boost(home , path_output , lv1 , contact , BKG_SEC , DELAY , RMs_names)
if (good(0) eq -2) then begin  
	print,format='(/,"NO SAA DATA, FILE SKIPPED",/)'
	Mail_obj="No SAA reference in SOE data for "+string(fix(contact),format='(I4)')+" Orbit"
	openw, unit, trigger_path, /GET_LUN,/append
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO SAA DATA",/)'
	FREE_LUN, unit
	
	openw,unit,burst_path,/GET_LUN,/append
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO SAA DATA",/)'
	free_lun,unit
	
	openw,unit,GRB_path,/GET_LUN,/append
		printf,unit,format='(/,/,A0,/,/)',Mail_obj
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO SAA DATA",/)'
	free_lun,unit

	goto,no_mail
endif else if (good(0) eq -4) then begin  
	print,format='(/,"NO ECLIPSE DATA, FILE SKIPPED",/)'
	Mail_obj="No ECLIPSE reference in SOE data for "+string(fix(contact),format='(I4)')+" Orbit"
	openw, unit, trigger_path, /GET_LUN,/append
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO ECLIPSE DATA",/)'
	FREE_LUN, unit
	
	openw,unit,burst_path,/GET_LUN,/append
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO ECLIPSE DATA",/)'
	free_lun,unit
	
	openw,unit,GRB_path,/GET_LUN,/append
		printf,unit,format='(/,/,A0,/,/)',Mail_obj
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO ECLIPSE DATA",/)'
	free_lun,unit

	goto,no_mail
endif else if (good(0) eq -1) then begin
		print,"NO SOE FILE FOUND"
		Mail_obj="No SOE FILE found for "+string(fix(contact),format='(I4)')+" Orbit"
	openw, unit, trigger_path, /GET_LUN,/append
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO SOE FILE FOUND",/)'
	FREE_LUN, unit
	
	openw,unit,burst_path,/GET_LUN,/append
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO SOE FILE FOUND",/)'
	free_lun,unit
	
	openw,unit,GRB_path,/GET_LUN,/append
		printf,unit,format='(/,/,A0,/,/)',Mail_obj
		printf,unit,format='(/,"***********************************************************")'
		printf,unit,format='("ORBIT ",I7)',contact
		printf,unit,format='("*************************************************************",/)'
		printf,unit,format='(/,"NO SOE FILE FOUND",/)'
	free_lun,unit

	goto,no_mail
endif

;**************************************************************************************************
; 	ELIMINATE ALL 0 VALUE BY RMs DATA TO CLEAN BY SAA, BOOST AND IDLE PERIOD
;**************************************************************************************************
;stop
ind0=where(good(1,*) eq 0)

;*************** 	WILL BE ALSO VERIFIED THAT EXTREME OF SINGLE INTERVAL IS NOT NEAR TO
;***************		EXSTREME OF RMs AND WILL BE EVENTUALLY CORRECT THE EXTREME INDEX

if (ind0(0) ne -1) and (ind0(0) ne n_elements(good(1,*))-1) then begin
	count_idle=where(ind0(1:*)-ind0(0:n_elements(ind0)-2) gt 3,tot)
	FOR K= 0,TOT DO BEGIN
		if (count_idle(0) ne -1) then begin
			START_0=[0,count_idle+1]
			STOP_0=[count_idle,N_ELEMENTS(IND0)-1]
		endif else begin
			START_0=0
			STOP_0=n_elements(IND0)-1
		endelse
		;******************************************************************************************
		; controllo sugli estremi dell''intervallo di idle
		;******************************************************************************************
		if (IND0(STOP_0(0))+10 le n_elements(good(0,*))-1 and (IND0(START_0(0)) ge 0)) then begin
	  	if (IND0(START_0(0)) eq 0) then GOOD=[GOOD(*,IND0(STOP_0(0))+10:*)]$
	  	else GOOD=[[GOOD(*,0:IND0(START_0(0))-1)],[GOOD(*,IND0(STOP_0(0))+10:*)]]
		endif else if (n_elements(START_0)eq 1) then begin
			GOOD=[[GOOD(*,0:IND0(START_0(0)))],[GOOD(*,IND0(STOP_0(0))   :*)]]
			goto,out
		endif else print,'NO CONDITION GOOD'
		ind0=where(good(1,*) eq 0)
		if (ind0(0) ne -1) then begin
			if (ind0(0) eq n_elements(good(1,*))-1) then begin 
				good=good(*,0:n_elements(good(1,*))-2)
				goto,out
			endif
			count_idle=where(ind0(1:*)-ind0(0:n_elements(ind0)-2) gt 3,tot_idle)
		endif
	ENDFOR
out:
endif
if (N_ELEMENTS(GOOD(0,*)) lt BKG_SEC+DELAY) then BEGIN
	print,'FILE SKIPPED'
	goto, no_mail
endif
;stop
;*************************************************************************************************
; searching for count upper threshold
;*************************************************************************************************

BKG=uintarr(n_elements(good(0,*)))
THR=dblarr(n_elements(good(0,*)))
LOG_BURST=dblarr(8)
line_burst=0
trig=0
i_fix=bkg_sec
important_trigger=dblarr(9)
INDEX_BURST=0

openr,unit,path_output+'stop_saa',/get_lun
readf,unit,format='(F30.10)',stop_saa
free_lun,unit
print,format='("stop SAA ",f0)',stop_saa

for k=1,8 do begin
	for i=BKG_SEC+DELAY,n_elements(good(0,*))-1 do begin
		BKG(i)=mean(good(k,i-(BKG_SEC+DELAY):i-DELAY),/double)
		THR(i)= BKG(i) + N_SIGMA*sqrt(BKG(i))
		if (trig eq 0) then begin
			if (good(k,i) ge THR(i)) then begin
				if (good(0,i) le stop_saa+BKG_SEC+DELAY) then begin
					print,format='(" trigger near stop SAA ",f0," at orbit ",I0)',good(0,i),contact
					openw, unit, trigger_path, /GET_LUN,/append
					printf,unit,format='(" trigger near stop SAA ",f0," at orbit ",I0)',good(0,i),contact
					FREE_LUN, unit
					continue
				endif
				trig=1
				INDEX_BURST+=1
				i_fix=i
				BKG(i)=mean(good(k,i_fix-(BKG_SEC+DELAY):i_fix-DELAY),/double)
				THR(i)= BKG(i) + N_SIGMA*sqrt(BKG(i))
				line_burst+=1
				LOG_BURST=[[log_burst],[double(INDEX_BURST),double(i),double(k),GOOD(0,i),GOOD(k,i),double(BKG(i)),$
						thr(i),(GOOD(k,i)-BKG(i))/sqrt(BKG(i))]]
				;stop
				openw, unit, trigger_path, /GET_LUN,/append
				if open_trigger eq 0 then begin
					printf,unit,format='(/,"***********************************************************")'
					printf,unit,format='("ORBIT ",I7)',contact
					printf,unit,format='("***********************************************************",/)'
					printf,unit,format='("Index",T18,"Time(TT)",T35,"En_Channel",T50,"Bin_value", T62,"BKG_value", T74,"Threshold",T90,"Significance",/)'
					open_trigger=1
				endif
				printf,unit,format='(I5,T8,F25.10,T33,A15,T50,I6,T62,I6,T73,f8.1,T88,f8.1)',i,good(0,i),channel_name(k-1),$
							good(k,i),BKG(i),thr(i),LOG_BURST(7,line_burst)
				FREE_LUN, unit
				openw, unit, trigger_dat_path, /GET_LUN,/append
				printf,unit,format='(I5,I5,F25.10,A15,I6,I6,f8.1,f8.1)',contact,i,good(0,i),k,good(k,i),BKG(i),thr(i),$
							LOG_BURST(7,line_burst)
				FREE_LUN, unit
			endif else if (abs(i-i_fix) ge delay) then trig=0
		endif else begin
			if (good(k,i) ge THR(i)) then begin
;				i_fix=i
				line_burst+=1
				LOG_BURST=[[LOG_BURST],[double(INDEX_BURST),double(i),double(k),GOOD(0,i),GOOD(k,i),double(BKG(i)),$
							thr(i),(GOOD(k,i)-BKG(i))/sqrt(BKG(i))]]
				openw, unit, trigger_path, /GET_LUN,/append
				printf,unit,format='(I5,T8,F25.10,T33,A15,T50,I6,T62,I6,T73,f8.1,T88,f8.1)',i,good(0,i),channel_name(k-1),$
							good(k,i),BKG(i),thr(i),LOG_BURST(7,line_burst)
				FREE_LUN, unit
				openw, unit, trigger_dat_path, /GET_LUN,/append
				printf,unit,format='(I5,I5,F25.10,I10,I10,I10,f25.1,f25.1)',contact,i,good(0,i),k,good(k,i),BKG(i),$
								thr(i),LOG_BURST(7,line_burst)
				FREE_LUN, unit
			endif else if (abs(i-i_fix) ge delay) then begin
				trig=0
			endif
		endelse
	endfor
	bbb=0
endfor
FLAG_other_ch='NO'
if (line_burst ne 0) then LOG_BURST=LOG_BURST(*,1:*) $
	else goto, save_last 
for z=1,max(LOG_BURST(0,*)) do begin
		many_long=where(LOG_BURST(0,*) eq z,tot_long)
		other_ind=where(LOG_BURST(0,*) ne z,tot_other_ind)
		if (tot_long gt 0) then begin
			;print,' z tot_long gt 0   ',tot_long
			single_burst=log_burst(*,many_long)
			peak_sign=max(SINGLE_BURST(7,*),pos_peak_sign)
			if (tot_long gt 1) then sing_diff_ind=ABS(single_burst(1,1:*)-single_burst(1,0:tot_long-2)) $
				else sing_diff_ind=-1
			if ((min(sing_diff_ind) ge 5) or (sing_diff_ind(0) eq -1)) and (tot_other_ind gt 0) then begin
				for g=0,tot_long-1 do begin
					dist_other_chan=abs(single_burst(1,g)-log_burst(1,other_ind))
					if (min(dist_other_chan) ge 10) then goto,jump ;FLAG_other_ch='OK'
					important_trigger=[[important_trigger],[log_burst(2,many_long(0)),double(z),LOG_BURST(3,manY_long(0)),$
					LOG_BURST(1,manY_long(0)),LOG_BURST(3,manY_long(tot_long-1)),log_burst(1,manY_long(tot_long-1)),$
					(LOG_BURST(3,manY_long(tot_long-1))-LOG_BURST(3,manY_long(0))),tot_long,peak_sign]]
				endfor
				goto, jump
			endif
			if ((tot_long ge min_duration) or (peak_sign gt min_sign)) then begin
				important_trigger=[[important_trigger],[log_burst(2,many_long(0)),double(z),LOG_BURST(3,manY_long(0)),$
						LOG_BURST(1,manY_long(0)),LOG_BURST(3,manY_long(tot_long-1)),log_burst(1,manY_long(tot_long-1)),$
						(LOG_BURST(3,manY_long(tot_long-1))-LOG_BURST(3,manY_long(0))),tot_long,peak_sign]]
				bbb=1
			endif 
		endif
	jump:
endfor

if (n_elements(important_trigger(0,*)) gt 1) then begin
	important_trigger=important_trigger(*,1:*)
		time=important_trigger(2,0)
		;stop
		;CALDAT, double(2400000.5d + 53005.000754444444444444d + double(time) /86400.0), MM, DD, YYYY, Hr, Min, Sec
		CALDAT, double(2400000.5d + 53005.0d + double(time) /86400.0), MM, DD, YYYY, Hr, Min, Sec
		UTC=strcompress(string(YYYY,format='(i04)')+'-'+ string(MM,format='(i02)')+'-'+$
			string(DD,format='(i02)'),/remove_all)+'   '+strcompress(string(Hr,format='(i02)')+':'+$
			string(Min,format='(i02)')+':'+string(Sec,format='(f10.1)'),/remove_all)
		Mail_obj="Possible MCAL GRB @ "+string(fix(contact),format='(I4)')+" orbit UTC  "+UTC
		
		openw,unit,light_curve_path,/GET_LUN;,/append
                printf,unit  ,format='(f0)',important_trigger(2,0)
		free_lun,unit


		openw,unit,burst_path,/GET_LUN,/append
		printf,unit  ,format='(/,"***********************************************************")'
		printf,unit  ,format='("ORBIT ",I7)',contact
		printf,unit  ,format='("***********************************************************",/)'
		printf,unit  ,format='("Channel",T11,"Burst_ID",T27,"START",T40,"Start_index",T58,"STOP",T72,"Stop_index",T84,"Duration(sec)",T98,"n_bins",T107,"Significance")'
		openw,unit_1,GRB_path,/GET_LUN,/append
		printf,unit_1,format='(/,/,A0,/,/)',Mail_obj
		printf,unit_1,format='(/,"***********************************************************")'
		printf,unit_1,format='("ORBIT ",I7)',contact
		printf,unit_1,format='("***********************************************************",/)'
		printf,unit_1,format='("Channel",T11,"Burst_ID",T27,"START",T40,"Start_index",T58,"STOP",T72,"Stop_index",T84,"Duration(sec)",T98,"n_bins",T107,"Significance")'
		for line=0,n_elements(important_trigger(0,*))-1 do begin
			printf,unit,format='(A9,I5, f25.8,I5,f25.8,I5,f13.1,I5,f13.1)',channel_name(fix(important_trigger(0,line)-1)),$
				fix(important_trigger(1,line)),$
				important_trigger(2,line),fix(important_trigger(3,line)),important_trigger(4,line),fix(important_trigger(5,line)),$
				important_trigger(6,line),fix(important_trigger(7,line)),important_trigger(8,line)
			printf,unit_1,format='(A9,I5, f25.8,I5,f25.8,I5,f13.1,I5,f13.1)',channel_name(fix(important_trigger(0,line)-1)),$
				fix(important_trigger(1,line)),$
				important_trigger(2,line),fix(important_trigger(3,line)),important_trigger(4,line),fix(important_trigger(5,line)),$
				important_trigger(6,line),fix(important_trigger(7,line)),important_trigger(8,line)
		endfor
		free_lun,unit,unit_1
endif
save_last:
last_rms=good(*,i-(BKG_SEC+DELAY):*)
save,last_rms,filename=path_output+'last_rms.sav'

no_mail :

end
