function main_sync,home,RMs_names,path_output,start_orbit , orb_max, orbit , silent = silent
;loadct,39
col_define,39
!p.color=0
!p.background=16777215
;**************************************************************************************************
; 	INITIALIZE VARIABLES
;**************************************************************************************************

open_trigger=0
open_burst=0

;**************************************************************************************************
;	READ RMs DATA AND REBIN ENERGY CHANNELS AS DEFINED IN "channel_name" 
;**************************************************************************************************

channel_name=[' X_.17-.7',' X_.7-1.4','X_1.4-2.8','   X_>2.8',' Z_.17-.7',' Z_.7-1.4','Z_1.4-2.8','   Z_>2.8']
LV1=RMsReader_MCAL(RMs_names)
RMs=LV1.Data
contact=FIX(strmid(rms_names,strpos(rms_names,'PKP')+3,6))
print,format='(/,"working on file:    ",A0,/)',RMs_names


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
;	CHANGE RMs DATA FOR SAA PERIOD AND TRANSITION POWER PERIOD (BOOST) TO 0 VALUE
;**************************************************************************************************

;good=clean_saa_boost(home , path_output , lv1 , contact , BKG_SEC , DELAY , RMs_names)
clean_saa_boost,orb_max
readcol , format='(f30.10,f30.10,f30.10,f30.10,f30.10)' , "/home/mcal/RM_search/syncerr/SAS.dat", start_saa , stop_saa , stop_saa_long , start_boost , stop_boost , /silent
;print, transpose([start_saa , stop_saa , stop_saa_long ]), format='(f0)'
;openr , unit , /get_lun , "/home/mcal/RM_search/syncerr/SAS.dat"
;readf, format='(3f30.10)' , saa_time(0) , saa_time(1) , saa_time(2)
;free_lun , unit

if 	((start_boost gt start_saa) and (start_boost lt stop_saa)) or $
	((stop_boost gt start_saa) and (stop_boost lt stop_saa)) then begin
		print,format='(/,/,"IN",/,/)' 
		flag_boost = 1
	endif else begin
		print,format='(/,/,"OUT",/,/)' 
		flag_boost = 0
	endelse

;if (good(0) eq -2) then 	print,format='(/,"NO SAA DATA, FILE SKIPPED",/)'
;if (good(0) eq -4) then print,format='(/,"NO ECLIPSE DATA, FILE SKIPPED",/)'
;if (good(0) eq -1) then print,"NO SOE FILE FOUND"

rms_time=reform(RMs(0,*))
start_saa_tmp=min(abs(rms_time-start_saa(0)),start_saa_ind)
stop_saa_tmp=min(abs(rms_time-stop_saa(0)),stop_saa_ind)
stop_long_saa_tmp=min(abs(rms_time-stop_saa_long(0)),stop_long_saa_ind)

if start_saa_tmp ge 60.0 then begin
	print,"SAA difference time Grater than 60.0 second, skiped"
	return,[-1 , -1 , -1]
endif

;pro read_cot , orbit , t_start , t_stop , start_cot , stop_cot
read_cot , orbit , tstart , tstop , start_cot , stop_cot

start_cot_tmp=min(abs(rms_time-start_cot),start_cot_ind)

stop_long_saa_ind=stop_long_saa_ind+150
start_cot_ind=start_cot_ind-30

if N_ELEMENTS(rms(1,*)) LT start_cot_ind THEN RETURN,[-1,-1 , -1]
if (start_cot_ind-30-stop_long_saa_ind) lt 0 then RETURN,[-1,-1 , -1]

;if N_ELEMENTS(rms(1,*)) LT 5200 THEN RETURN,[-1,-1]

if keyword_set(silent) ne 1 then begin
	plot,rms(1,*),xrange=[start_saa_ind-100,n_elements(rms(1,*))],title=orbit
	;oplot,[replicate(0.0d,stop_saa_ind),reform(rms(1,stop_saa_ind:*))],color=100
	;oplot,[replicate(0.0d,stop_long_saa_ind),reform(rms(1,stop_long_saa_ind:*))],color=200
	oplot,[replicate(-10.0d,stop_long_saa_ind),reform(rms(1,stop_long_saa_ind:start_cot_ind-30))+100.0],color=200
	oplot,[replicate(-10.0d,start_saa_ind),reform(rms(1,start_saa_ind:stop_saa_ind))],color=!col.green
	;oplot,[replicate(-10.0d,stop_long_saa_ind),reform(rms(1,stop_long_saa_ind:start_cot_ind-30))+100.0],color=200
endif

value=moment(rms(1,stop_long_saa_ind:start_cot_ind-30),/double,sdev=st_dev) 
print,value(0),st_dev
elem=orb_max-start_orbit+1

skip:
return , [value(0) , st_dev , flag_boost]
end
