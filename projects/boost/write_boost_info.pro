; 	COT_PATH	path where COT summary file (output of read_cot.pro) is found (default "/home/mcal/RM_search/boost/COT.dat")
;	BOOST_NAME	path where output BOOST file is written (default "/home/mcal/RM_search/boost/BOOST.dat")

pro write_boost_info , COT_PATH = cot_path , BOOST_PREDICTION = boost_pred , BOOST_UP_NOW = boost_now

FLAG="PREDICTION"

if keyword_set(boost_pred) eq 1 then boost_name=boost_pred else boost_NAME="/home/mcal/RM_search/boost/BOOST_FF.dat"
if keyword_set(cot_path) ne 1 then cot_path="/home/mcal/RM_search/boost/COT_FF.dat"
print,"start write_boost_info ,",cot_path,boost_name
readcol, cot_path , contact , start_cot , stop_cot , format='(f0,f0,f0)',/silent

out=dblarr(3)
print,minmax(contact(1:*)-contact(0:n_elements(contact)-1))
openw,unit,boost_name,/get_lun
free_lun,unit
orbmin=min(contact)

readcol,"/AGILE_DATA/INDEX/3913.cor.index",names,format='(A0)'
names=reverse(names(uniq(sort(names))))
last_orbit=strsplit(names(0),'/',/extract)
last_orbit=fix((last_orbit(2)))
help,last_orbit
orb_min=last_orbit-100
orb_max=min([last_orbit+30,max(contact)])

re_cicle:

if (FLAG eq "PREDICTION") then print , format='(/,/,/,"Start PREDICTION iteration",/,/,/,I0,I0,A0)',orb_min,orb_max,boost_name $
else if (FLAG eq "UPDATE") then print , format='(/,/,/,"Start UPDATE iteration",/,/,/,I0,I0,A0)',orb_min,orb_max,boost_name

for orbit = orb_min , orb_max do begin
;	if n_elements(orb_max) ne 0 then begin
;		if orbit gt orb_max then goto, no_write
;	endif
	start_tt=stop_cot(orbit-1-orbmin)
	stop_tt=stop_cot(orbit-orbmin)
	
;************************************************************************************************
; 		METHOD for ORBIT LT 1790
;************************************************************************************************
	
	if orbit gt 1790 then begin
		wait,0.1
		print,orbit
		semi_period_boost=80.0d
		help,start_tt,stop_tt,/str
		ECLIPSE=read_soe_index(start_tt,stop_tt,semi_period_boost,/BOOST) ; BOOST data search 
		ECLIPSE1=read_soe_index(start_tt,stop_tt,semi_period_boost,/ECLIPSE) ; Enter ECLIPSE data search
		;stop
		if (n_elements(eclipse) eq 2) then begin
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',[double(orbit),eclipse(0),eclipse(1)],"BOOST"
			free_lun,unit
		endif else if (n_elements(eclipse) eq 4) then begin
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),eclipse(0,0),eclipse(1,0),"BOOST"
			free_lun,unit
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),eclipse(0,1),eclipse(1,1),"BOOST"
			free_lun,unit
		endif else if (eclipse(0) eq -4) then begin
			print,"NO ECLIPSE DATA"
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),-4.0d,-4.0d,"BOOST"
			free_lun,unit
			rms=-4
			goto,no_write
		endif else if (eclipse(0) eq -3) then begin
			print,"ERROE IN read_soe_index"
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),-3.0d,-3.0d,"BOOST"
			free_lun,unit
			rms=-3
			goto,no_write
		endif else if (eclipse(0) eq -1) then begin
			print,"NO SOE FILE"
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),-1.0d,-1.0d,"BOOST"
			free_lun,unit
			rms=-1
			goto,no_write
		endif else begin 
			print,"MANY elements in BOOOST selection OR BHOOOOOOOOOOOOO"
			help,eclipse
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),-5.0d,-5.0d,"BOOST"
			free_lun,unit
			rms=-5
			goto,no_write
		endelse
;	no_write:
;	endif else print,"skip BOOST"
	
	DELAY_MEAN=54.008149d
	help,eclipse1
		if (n_elements(eclipse1) eq 2) then begin
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',[double(orbit),eclipse1(0),eclipse1(1)],"ECLIPSE"
			free_lun,unit
		endif else if (n_elements(eclipse1) eq 4) then begin
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),eclipse1(0,0),eclipse1(1,0),"ECLIPSE"
			free_lun,unit
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),eclipse1(0,1),eclipse1(1,1),"ECLIPSE"
			free_lun,unit
		endif else if (eclipse1(0) eq -4) then begin
			print,"NO ECLIPSE DATA for ecl selection"
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),-44.0d,-44.0d,"ECLIPSE"
			free_lun,unit
			rms=-4
			goto,no_write
		endif else if (eclipse1(0) eq -3) then begin
			print,"ERROE IN read_soe_index for ecl selection"
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),-33.0d,-33.0d,"ECLIPSE"
			free_lun,unit
			rms=-3
			goto,no_write
		endif else if (eclipse1(0) eq -1) then begin
			print,"NO SOE FILE for ecl selection"
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),-11.0d,-11.0d,"ECLIPSE"
			free_lun,unit
			rms=-1
			goto,no_write
		endif else begin 
			print,"MANY elements in BOOOST selection OR BHOOOOOOOOOOOOO for ecl selection"
			stop
			help,eclipse1
			openw,unit,boost_name,/get_lun,/append
			printf, unit , format='(f30.15,f30.15,f30.15,3X,A0)',double(orbit),-55.0d,-55.0d,"ECLIPSE"
			free_lun,unit
			rms=-5
			goto,no_write
		endelse
no_write:
	endif else print,"skip"
endfor

readcol , format='(f30.15,f30.15,f30.15)' , boost_name, uno , due , tre 
print,minmax(uno)
print,minmax(due)
print,minmax(tre)

;boost_name="/home/mcal/RM_search/boost/BOOST_up_to_now.dat"
boost_name=boost_now
readcol , format='(f0)' , boost_name , old_orbit
old_orbit=reverse(old_orbit(uniq(sort(old_orbit))))
print, 'update BOOST.dat between ',old_orbit(0), " orbit and " , last_orbit , " orbit"
orb_min=old_orbit(0)+1
orb_max=last_orbit
if FLAG eq "PREDICTION" then begin
	FLAG="UPDATE"
	goto , re_cicle
endif
end
