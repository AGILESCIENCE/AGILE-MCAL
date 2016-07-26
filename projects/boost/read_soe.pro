function read_SOE , FILE_NAME_SOE , tstart , tstop , semi_period , SAA = SAA , BOOST = BOOST , ECLIPSE = ECLIPSE
wait,0.001

;**********************************************************************************
; OPEN SOE FILE
;**********************************************************************************

restore,'./template_soe.sav'
soe_file=read_ascii(FILE_NAME_SOE,template=template_soe)

;**********************************************************************************
;	EXTRACTING ONLY INORMATION SELECTED BY KEYWORD & PARAMETERS
;**********************************************************************************

if keyword_set(SAA) then begin
	ind1=where(strpos(soe_file.field2,'SAA') ne -1)
	for kkk=0,n_elements(ind1)-1 do begin
		if (ind1(kkk) ne -1) then begin
				SOE = { PHASE : soe_file.field2(ind1), START_UTC : soe_file.field2(ind1+2) ,$
		     	START_TT : dblarr(n_elements(ind1)) , DURATION : double(soe_file.field2(ind1+3)) }	
			goto, out
		endif else begin
			print, 'SAA correlation NOT FOUND in file  ',FILE_NAME_SOE
			return , -2
			goto, jump
		endelse
	endfor
endif else if (keyword_set(BOOST) or keyword_set(eclipse)) then begin
	ind1=where(strpos(soe_file.field2,'ECLIPSE') ne -1)
	for kkk=0,n_elements(ind1)-1 do begin
		if (ind1(kkk) ne -1) then begin
			SOE = { PHASE : soe_file.field2(ind1+1) , START_UTC : soe_file.field2(ind1+2) ,$
				START_TT : dblarr(n_elements(ind1)) , DURATION : double(soe_file.field2(ind1+3)) }	
			goto, out
		endif else begin
			return , -4
			goto,jump
		endelse
	endfor
endif else begin
	print,'NO KEYWORD SELECTION FOR READ_SOE.PRO, END'
	goto, jump
endelse
out:

;*************************************************************************************
;	CONVERT EXTRACT SOE TIME BY UTC TO TT 
;*************************************************************************************

mjdref=double(53005)+double(0.000754444444444444)
for i=0,n_elements(ind1)-1 do begin
	date=strsplit(SOE.START_UTC(i),' ',/extract)
	UTC=[uint(strsplit(date(0),'/',/extract)),float(strsplit(date(1),':',/extract))]
	mjd=date2mjd(utc(0),utc(1),utc(2))
	TT=(mjd-mjdref)*86400.0+utc(3)*3600.0+utc(4)*60.0+utc(5)
	SOE.START_TT(i)=TT
endfor
wait,0.001

;*************************************************************************************
;	SEARCHING FOR CORRESPONDANCE WITH INPUT TIME (START & STOP) & RETURNING VALUE
;*************************************************************************************

if keyword_set(SAA) then begin
	ind000=where((tstart lt SOE.start_tt) and (SOE.start_tt+SOE.duration lt tstop),totale) 
	if (ind000(0) eq -1) then return,-2 else $
		return,transpose([[SOE.start_tt(ind000)],[SOE.duration(ind000)]])
endif else if keyword_set(BOOST) then begin
	ind		=where(strpos(SOE.PHASE,'UMBRA')eq 0)

	start_boost=SOE.start_tt(ind)+SOE.duration(ind)+ 386.74286d - semi_period
	stop_boost=SOE.start_tt(ind)+SOE.duration(ind)+ 386.74286d + semi_period

	ind000=where(((start_boost gt tstart) and (start_boost lt tstop)) or $
				((stop_boost gt tstart) and (stop_boost lt tstop)),totale)
	if (ind000(0) eq -1) then return,-4 else $
		return,transpose([[start_boost(ind000)],[stop_boost(ind000)]])
endif else if keyword_set(ECLIPSE) then begin
	ind		=where(strpos(SOE.PHASE,'UMBRA')eq 0)
	DELAY_MEAN=54.008149d
	ECL_START=SOE.start_tt(ind)+DELAY_MEAN-5
	ECL_STOP =SOE.start_tt(ind)+DELAY_MEAN+5
	ind111=where((ECL_START gt tstart) and (ECL_START lt tstop)) OR $
		((ECL_STOP gt tstart) and (ECL_STOP lt tstop))
	if (ind111(0) eq -1) then return,-4 else $
		return,transpose([[ECL_start(ind111)],[ECL_stop(ind111)]])
endif

jump:

end
