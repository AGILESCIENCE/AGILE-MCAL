pro allign_previous_orbit , out_path , home , start_orbit

path='/AGILE_DATA/COR/'
;home='./'

file_name=path+string(format='(I06)',start_orbit-1)+'/PKP'+strcompress(string(format='(I06)',start_orbit-1),/remove_all)+'_1_3913_000.cor.gz'

;print,file_name
restore,'./cor_index.template'
index=read_ascii('/AGILE_DATA/INDEX/3913.cor.index',template=cor_index)

ind_name=where(index.rms_name eq file_name)	

if ind_name(0) eq -1 then begin
	print,format='(/,"DATA OF CONTACT",i5," TO REFEREE last_rms NOT FOUND",/)',start_orbit-1
	last_rms=0
	save,last_rms,filename=out_path+'last_rms.sav'
	goto,after
endif

LV1=RMsReader_MCAL(file_name)
rms=lv1.data
RMs_tmp=[rms(0,*) , rms( 2,*)+rms( 3,*) , rms( 4,*) , rms( 5,*) , rms( 6,*)+rms( 7,*)+rms( 8,*)+rms( 9,*)+rms(10,*)+rms(11,*) , $
rms(13,*)+rms(14,*) , rms(15,*) , rms(16,*) , rms(17,*)+rms(18,*)+rms(19,*)+rms(20,*)+rms(21,*)+rms(22,*)]
RMs=RMs_tmp
if  n_elements(rms(0,*)) lt 90 then begin
	print,FORMAT='(/,"NO LONG LAST_RMs FOR ORBIT ", i5,/)',start_orbit-1
	last_rms=0
	save,last_rms,filename=out_path+'last_rms.sav'
	goto, jump
endif else begin
	print,FORMAT='(/,"Saved RMs of orbit ",I5 ," as last_rms",/)',start_orbit-1
	last_rms=rms(*,n_elements(rms(0,*))-90:*)
	save,last_rms,filename=out_path+'last_rms.sav'
endelse

after:
;restore,home+'ref_boost.index.template'
;index=read_ascii(home+'ref_boost.index_',template=boost_index)
;ind=where(index.contact eq start_orbit)	
;if ind(0) eq -1 then begin
;	print,FORMAT='(/,"Prevoius contact to reference BOOST data NOT FOUND",/)'
;	goto,jump
;endif
;openw,unit,out_path+'ref_boost',/get_lun
;printf,unit,format='(I6,/,F30.10,/,I6)',index.contact_ref(ind(0)),index.time_ref(ind(0)),index.period_ref(ind(0)) 
;free_lun,unit
;print,format='("BOOST DATA REFERRED TO ORBIT ",I5,/)',index.contact_ref(ind(0))
if start_orbit le 1790 then print,'This version cannot handle contact previous to 1790'
jump:
end
