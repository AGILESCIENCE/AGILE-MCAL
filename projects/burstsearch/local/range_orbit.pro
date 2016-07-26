pro range_orbit , home , path_out , start_orbit , orb_max

path='/AGILE_DATA//COR/'

;**************************************************************************************************
; restore information of previous orbit: last_rms and ref_boost with procedure allign_previous_orbit
;**************************************************************************************************

allign_previous_orbit , path_out , home , start_orbit 

restore,'./cor_index.template'
index=read_ascii('/AGILE_DATA/INDEX/3913.cor.index',template=cor_index)
;help,index,/str

for orbit=start_orbit , orb_max do begin
	file_name=path+string(format='(I06)',orbit)+'/PKP'+strcompress(string(format='(I06)',orbit),/remove_all)+'_1_3913_000.cor.gz'
	;print,file_name
	;print,index.RMS_NAME(0)
	ind=where(strpos(index.RMS_NAME,file_name) ne -1)
	if (ind(0) ne -1)	then begin
		print,format='(/,/,/,"Working on ",A0)',index.RMS_NAME(ind(0))
		;main,home,index.RMS_NAME(ind(0)),path_out
		;print,[ home+'GRB.sh' , home , index.RMS_NAME(ind(0)) , path_out , "no_mail" ]
		spawn, [ home+'GRB.sh' , home , index.RMS_NAME(ind(0)) , path_out , "no_mail" ], res , errres , /noshell
		If (n_elements(ErrRes) eq 1) and (ErrRes(0) eq '') then print,'',ErrRes else print,'',transpose(ErrRes),''
 		if (n_elements(Res) eq 1) and (res(0) eq '') then print,'',RES else print,'',transpose(Res),''
	endif
endfor
jump:
print,'end range_orbit'
end
