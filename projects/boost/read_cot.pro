pro read_cot , COT_PATH = cot_path , OUT_PATH = out_path

; 	COT_PATH	path where .COT data are found (default "/AGILE_DATA/AUX/")
;	OUT_PATH	path where to save output data (default "/home/mcal/RM_search/boost/COT.dat")

mjdref=53005.0d 		
if keyword_set(out_path) ne 1 then out_path="/home/mcal/RM_search/boost/COT_FF.dat"
if keyword_set(cot_path) ne 1 then cot_path="/AGILE_DATA/AUX"
cot_path = cot_path + '/*.COT'

index=reverse(file_search(cot_path,count=tot))

cot=dblarr(3)

for i=0,tot-1 do begin
;for i=0,3 do begin
	print,i
	readcol, index(i) , start_d , start_h , stop_d , stop_h , duration , malindi , pass , contact , skipline=1 , $
	format='(a0,a0,a0,a0,a0,a0,a0,i4)',/silent
	;ind=where(strpos(contact,orbit) ne -1)
	;if (ind(0) ne -1) then begin
		;print,index(i),contact(ind),orbit
	for k=0 , n_elements(start_d)-1 do begin
		start = [strsplit( start_d(k) , "/" , /extract ),strsplit( start_h(k) , ":" , /extract )]
		stop  = [strsplit( stop_d(k)  , "/" , /extract ),strsplit( stop_h(k)  , ":" , /EXTRACT )]
		;print,[start,stop]
		start_cot=((JULDAY(START(1),START(2),START(0),START(3),START(4),START(5))) - 2400000.5d - mjdref )*86400.0d
		STOP_cot=((JULDAY(STOP(1),STOP(2),STOP(0),STOP(3),STOP(4),STOP(5))) - 2400000.5d - mjdref )*86400.0d
		cot=[[cot],[double(contact(k)),start_cot,stop_cot]]
	endfor
	;endif
endfor
cot=cot(*,1:*)
cot_g=cot[*,UNIQ(cot(0,*), SORT(cot(0,*)))]  
;stop
;openw,unit,/get_lun, 
;!TEXTUNIT = unit
forprint,cot_g(0,*),cot_g(1,*),cot_g(2,*),textout=out_path
;free_lun, unit
;printf,unit,transpose(cot_g)

end
