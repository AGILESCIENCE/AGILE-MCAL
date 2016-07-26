function clean_saa_boost , home , path_output , lv1 , contact , bkg_sec , delay , file_name 

flag=''
rms=lv1.data

start_tt=RMs(0,0)
stop_tt=RMs(0,n_elements(rms(0,*))-1)

;window,5,title=contact
;plot,rms(0,*),rms(1,*)

;************************************************************************************************
; 	FIX TO 0 VALUE THE "SAA" PERIOD READ BY SOE
;************************************************************************************************

SAA=read_soe_index(start_tt,stop_tt,0.0d,/SAA)
if SAA(0) eq -2 then begin
	rms = -2
	print,'no SAA data'
	goto,no_write
endif else if (saa(0) eq -3) then begin
	print,"ERROR IN read_soe_index"
	rms=-3
	goto,no_write
endif else if (saa(0) eq -1) then begin
	print,"NO SOE FILE"
	rms=-1
	goto,no_write
endif 


start_saa = SAA(0)
stop_saa  = SAA(0)+SAA(1)+50.0d
s_in=min(abs(rms(0,*)-start_saa(0)),ind_start_saa)
s_fn=min(abs(rms(0,*)-stop_saa(0)),ind_stop_saa)

openw,unit,path_output+'stop_saa',/get_lun
printf,unit,format='(F30.10)',stop_saa
free_lun,unit

;oplot,rms(0,ind_start_saa:ind_stop_saa),rms(1,ind_start_saa:ind_stop_saa),color=!col.red

rms(1:*,ind_start_saa:ind_stop_saa)=0.0d


;************************************************************************************************
; 	FIX TO 0 VALUE THE "BOOST" PERIOD 
;	WILL BE USED TWO DIFFERENT METHOD:
;		- for ORBIT LT 1790 THE "BOOST" POSITION WILL BE CALCULATED
;		- for ORBIT GE 1790 THE "BOOST" POSITION WILL BE READ BY SOE
;************************************************************************************************


;************************************************************************************************
; 		METHOD for ORBIT LT 1790
;************************************************************************************************

if contact lt 1790 then begin
;	openr,unit,path_output+'ref_boost',/get_lun
;	readf,unit,format='(I6,/,F30.10,/,I6)',a,b,c
;	free_lun,unit

	restore,home+'ref_boost.index.template'
	index=read_ascii(home+'ref_boost.index_',template=boost_index)

	sel_ind=where(index.contact eq contact)	
	if sel_ind(0) eq -1 then begin
		print,'CONTACT NOT FOUND FOR ORBIT LT 1790'
		goto,no_write
	endif
	a=index.CONTACT_REF(sel_ind(0))
	b=index.TIME_REF(sel_ind(0))
	c=index.PERIOD_REF(sel_ind(0))
	
	prec=0
	start_boost     =b+(contact-a-2)*c-80; 120 sec boost duration
	start_boost_next=b+(contact+1-a-2)*c-80
	stop_boost	=b+(contact-a-2)*c+80; 120 sec boost duration
	stop_boost_next	=b+(contact+1-a-2)*c+80
	while (start_boost lt start_tt) and (stop_boost lt start_tt) do begin
		wait,0.001
		prec+=1
		start_boost=b+(contact+prec-a-2)*c-80
		start_boost_next=b+(contact+PREC+1-a-2)*c-80
		stop_boost= b+(contact+prec-a-2)*c+80
		stop_boost_next=	b+(contact+PREC+1-a-2)*c+80
	endwhile
	isa=min(abs(rms(0,*)-start_boost(0)),ind_start)
	iso=min(abs(rms(0,*)-stop_boost(0)),ind_stop)
	if ((ind_start-50 lt 0) or (ind_stop+50 gt n_elements(rms(0,*))+1)) then begin
		boost=max(rms(1,ind_start:ind_stop),ind_boost)
		flag='norm'
	endif else begin
		boost=max(rms(1,ind_start-50:ind_stop+50),ind_boost)
		flag='ext'
	endelse
	;oplot,rms(0,ind_start:ind_stop),rms(1,ind_start:ind_stop),color=!col.green
	rms(1:*,ind_start:ind_stop)=0.0d

	if ((start_boost_next gt start_tt) and (start_boost_next lt stop_tt)) then begin
		isa_next=min(abs(rms(0,*)-start_boost_next(0)),ind_start_next)
		iso_next=min(abs(rms(0,*)-stop_boost_next(0)),ind_stop_next)
		if ind_stop_next ge n_elements(rms(0,*)-1) then begin
			ind_stop_next=n_elements(rms(0,*)-1)
		endif
		boost=max(rms(1,ind_start_next:ind_stop_next),ind_boost_next)
		;oplot,rms(0,ind_start_next:ind_stop_next),rms(1,ind_start:ind_stop),color=!col.green
		rms(1:*,ind_start_next:ind_stop_next)=0.0d
	endif
	ind_ne_0=where(rms(1,*) ne 0,tot_ne_0)

;******************************************************************************************
; condition to update boost_ref => 
;		max value in boost range lt 1.0
;		total non zero event lt then BKG_SEC+DELAY 
;		max time distance between good data and START_BOOST gt 30 sec
;		max time distance between good data and STOP_BOOST gt 30 sec
;******************************************************************************************
	
	if ((tot_ne_0 lt BKG_SEC+DELAY) or (boost lt 1.0) or (isa gt 5) or (iso gt 5) or $
			(start_boost gt stop_tt ) or (stop_boost lt start_tt) or  $
			(start_boost lt start_tt) or (stop_boost gt stop_tt) or $
			((start_boost gt start_saa) and (start_boost lt stop_saa)) or $
			((stop_boost gt start_saa) and (stop_boost lt stop_saa))) then begin
		print,'No Update boost_ref'
		goto,no_write
	endif
	
;************************************************************************************************
; 	METHOD for ORBIT GE 1790
;************************************************************************************************

endif else begin
	semi_period_boost=80.0d
	ECLIPSE=read_soe_index(start_tt,stop_tt,semi_period_boost,/BOOST)
	if (n_elements(eclipse) eq 2) then begin
		start_boost=eclipse(0)
		stop_boost =eclipse(1)
		min_start=min(abs(rms(0,*)-start_boost(0)),ind_start)
		min_stop=min(abs(rms(0,*)-stop_boost(0)),ind_stop)
		;oplot,rms(0,ind_start:ind_stop),rms(1,ind_start:ind_stop),color=!col.green
		rms(1:*,ind_start:ind_stop)=0.0d
	endif else if (n_elements(eclipse) eq 4) then begin
		start_boost=eclipse(0,0)
		stop_boost =eclipse(1,0)
		min_start=min(abs(rms(0,*)-start_boost(0)),ind_start)
		min_stop=min(abs(rms(0,*)-stop_boost(0)),ind_stop)
		;oplot,rms(0,ind_start:ind_stop),rms(1,ind_start:ind_stop),color=!col.green
		rms(1:*,ind_start:ind_stop)=0.0d	
		start_boost_next=eclipse(0,1)
		stop_boost_next =eclipse(1,1)
		min_start_next=min(abs(rms(0,*)-start_boost_next(0)),ind_start_next)
		min_stop_next =min(abs(rms(0,*)-stop_boost_next(0)),ind_stop_next)
		;oplot,rms(0,ind_start_next:ind_stop_next),rms(1,ind_start_next:ind_stop_next),color=!col.green
		rms(1:*,ind_start_next:ind_stop_next)=0.0d	
	endif else if (eclipse(0) eq -4) then begin
		print,"NO ECLIPSE DATA"
		rms=-4
		goto,no_write
	endif else if (eclipse(0) eq -3) then begin
		print,"ERROE IN read_soe_index"
		rms=-3
		goto,no_write
	endif else if (eclipse(0) eq -1) then begin
		print,"NO SOE FILE"
		rms=-1
		goto,no_write
	endif else begin 
		print,"MANY elements in BOOOST selection OR BHOOOOOOOOOOOOO"
		help,eclipse
		rms=-5
		goto,no_write
	endelse
endelse

no_write:

return,rms
end
