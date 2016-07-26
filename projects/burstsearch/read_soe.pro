function read_SOE , FILE_NAME_SOE , tstart , tstop , semi_period , SAA = SAA , BOOST = BOOST
wait,0.001

;**********************************************************************************
; OPEN SOE FILE
;**********************************************************************************

restore,'./template_soe.sav'
soe_file=read_ascii(FILE_NAME_SOE,template=template_soe)

;**********************************************************************************
;	EXTRACTING ONLY INORMATION SELECTED BY KEYWORD & PARAMETERS
;**********************************************************************************
;stop
if keyword_set(SAA) then begin
	ind1=where(strpos(soe_file.field2,'SAA') ne -1)
	for kkk=0,n_elements(ind1)-1 do begin
		if (ind1(kkk) ne -1) then begin
			;SOE = { PHASE : soe_file.field2(ind1)+' '+soe_file.field2(ind1+1) , START_UTC : soe_file.field2(ind1+2) ,$
		     ;	START_TT : dblarr(n_elements(ind1)) , DURATION : double(soe_file.field2(ind1+3)) }
		     SOE = { PHASE : soe_file.field2(ind1), START_UTC : soe_file.field2(ind1+2) ,$
		     	START_TT : dblarr(n_elements(ind1)) , DURATION : double(soe_file.field2(ind1+3)) }	
			goto, out
		endif else begin
			print, 'SAA correlation NOT FOUND in file  ',FILE_NAME_SOE
			return , -2
			goto, jump
		endelse
	endfor
endif else if keyword_set(BOOST) then begin
	ind1=where(strpos(soe_file.field2,'ECLIPSE') ne -1)
	;help,ind1
	for kkk=0,n_elements(ind1)-1 do begin
		if (ind1(kkk) ne -1) then begin
			SOE = { PHASE : soe_file.field2(ind1+1) , START_UTC : soe_file.field2(ind1+2) ,$
				START_TT : dblarr(n_elements(ind1)) , DURATION : double(soe_file.field2(ind1+3)) }	
			goto, out
		endif else begin
			;print, 'ECLIPSE correlation NOT FOUND in file ',FILE_NAME_soe
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
;stop
if keyword_set(SAA) then begin
	ind000=where((tstart lt SOE.start_tt) and (SOE.start_tt+SOE.duration lt tstop),totale) 
	if (ind000(0) eq -1) then return,-2 else $
		return,transpose([[SOE.start_tt(ind000)],[SOE.duration(ind000)]])
endif else if keyword_set(BOOST) then begin
	ind		=where(strpos(SOE.PHASE,'UMBRA')eq 0)
	;ind_penumbra	=where(strpos(SOE.PHASE(ind_umbra+1)	,'PENUMBRA'	) eq 0)
	;ind=ind_umbra(ind_penumbra)+1
	;ind_1=ind_umbra(ind_penumbra)
	;print,[transpose(ind),transpose(ind_1)]
	;help,ind
	;help,ind_umbra
	;help,ind_penumbra
	;stop
	;help,ind_umbra
	;help,ind1
	;print,[SOE.PHASE(ind_umbra),SOE.PHASE(ind_umbra+1)]
	;ind000=where((tstart lt SOE.start_tt(ind_umbra+1)) and (SOE.start_tt(ind_umbra+1)+SOE.duration(ind_umbra+1)+ 386.74286d lt tstop),totale)
	start_boost=SOE.start_tt(ind)+SOE.duration(ind)+ 386.74286d - semi_period
	stop_boost=SOE.start_tt(ind)+SOE.duration(ind)+ 386.74286d + semi_period
	;print,format='(F30.15,"  ")',start_boost
	ind000=where(((start_boost gt tstart) and (start_boost lt tstop)) or $
				((stop_boost gt tstart) and (stop_boost lt tstop)),totale)
	if (ind000(0) eq -1) then return,-4 else $
		return,transpose([[start_boost(ind000)],[stop_boost(ind000)]])
endif

;if (ind000(0) eq -1) then begin
;	print,-2
;	return,-2
;endif
;print,format='(F30.15,"  ",F30.15)',tstart,tstop
;print,[SOE.phase(ind(ind000)),string(SOE.start_tt(ind(ind000)),format='(f30.15)'),string(SOE.duration(ind(ind000)),format='(f30.15)'),string(start_boost(ind000),format='(f30.15)')]
;print,[SOE.phase(ind_1(ind000)),string(SOE.start_tt(ind_1(ind000)),format='(f30.15)'),string(SOE.duration(ind_1(ind000)),format='(f30.15)')]
;,[SOE.start_tt(ind_penumbra(ind000))+SOE.duration(ind_penumbra(ind000))+386.74286d]])])
;print,format='(F30.15,"  ",F30.15,"  ",F30.15)',[transpose([SOE.start_tt(ind_umbra(ind000))]),transpose([[SOE.duration(ind_umbra(ind000))],[SOE.start_tt(ind_umbra(ind000))+SOE.duration(ind_umbra(ind000))+386.74286d]])]
;print,format='(F30.15,"  ",F30.15,"  ",F30.15)',[transpose([SOE.start_tt(ind_umbra(ind000)+1)]),transpose([[SOE.duration(ind_umbra(ind000)+1)],[SOE.start_tt(ind_umbra(ind000)+1)+SOE.duration(ind_umbra(ind000)+1)+386.74286d]])]
;return,transpose([[SOE.start_tt(ind000)],[SOE.duration(ind000)]])

jump:

end
