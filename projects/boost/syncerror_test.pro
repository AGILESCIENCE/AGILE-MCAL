pro syncerror_test , start_orbit , orb_max , silent = silent

if n_params() ne 2 then print,foramt='(/,/,"SINTAX:		syncerror_test , start_orbit , orb_max",/,/)'

;pro range_orbit , home , path_out , start_orbit , orb_max
home="~/RM_search/syncerr/"
path='/AGILE_DATA//COR/'

elem=orb_max-start_orbit+1
check=dblarr(elem,5)

restore,'./cor_index.template'
index=read_ascii('/AGILE_DATA/INDEX/3913.cor.index',template=cor_index)

for orbit=start_orbit , orb_max do begin
	file_name=path+string(format='(I06)',orbit)+'/PKP'+strcompress(string(format='(I06)',orbit),/remove_all)+'_1_3913_000.cor.gz'
	ind=where(strpos(index.RMS_NAME,file_name) ne -1)
	if (ind(0) ne -1)	then begin
		v=main_sync(home,index.RMS_NAME(ind(0)),path_out,start_orbit , orb_max , orbit ,/silent)
		if v(0) eq 0.0 then begin
			check(orbit-start_orbit,*) = transpose([orbit , 0 , v(0) , v(1) , V(2)]) 
		endif else if v(0) gt 0.0 then  begin
			check(orbit-start_orbit,*) = transpose([orbit , 1 , v(0) , v(1), v(2)]) 
		endif else if v(0) eq -1.0 then 	check(orbit-start_orbit,*) = transpose([orbit ,-1 , v(0) , v(1) , v(2)])
	endif else begin
		check(orbit-start_orbit,*) = transpose([orbit , -1 , -1 , -1 , -1])
	endelse
endfor
if keyword_set(silent) ne 1 then begin
	window,2
	plot,check,psym=10,yrange=[-1,2]
endif
save,check,filename="./check.sav"
stop
print,'end'
end