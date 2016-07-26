function read_soe_index , start_run , stop_run , semi_period , SAA = SAA , BOOST = BOOST
print, start_run , stop_run , semi_period 
restore,'./SOE_INDEX_template.sav'

 readcol , '/AGILE_DATA/INDEX/SOE.index' ,  NAME , DATE_START , HOUR_START , $
			DATE_STOP , HOUR_STOP  , format='(A0,A0,A0,A0,A0,AO)',/silent
index_soe= { NAME : NAME , DATE_START : DATE_START , HOUR_START : HOUR_START , $
		DATE_STOP : DATE_STOP , HOUR_STOP : HOUR_STOP }   

lines=n_elements(index_soe.name)
start_UTC = intarr(6,lines)
stop_UTC  = intarr(6,lines)
start_TT = dblarr(lines)
stop_TT  = dblarr(lines)

for i=0,lines-1 do begin
	start_utc(*,i) = [uint(strsplit(index_soe.(1)(i),'/',/extract)),uint(strsplit(index_soe.(2)(i),':',/extract))]
	stop_utc(*,i)  = [uint(strsplit(index_soe.(3)(i),'/',/extract)),uint(strsplit(index_soe.(4)(i),':',/extract))]
	start_tt(i) = (double(JULDAY(start_utc(1,i) , start_utc(2,i) , start_utc(0,i) , start_utc(3,i) , start_utc(4,i) , $
		start_utc(5,i))) - 2400000.5d -53005.000754444444444444d )*86400.0d
	stop_tt(i)  = (double(julday(stop_utc(1,i) , stop_utc(2,i) , stop_utc(0,i) , stop_utc(3,i) , stop_utc(4,i) , $
		stop_utc(5,i))) - 2400000.5d -53005.000754444444444444d )*86400.0d
endfor

case n_params() of
3:	begin
		a=where(((start_run ge start_tt) and (start_run le stop_tt)) $
		or ((stop_run ge start_tt) and (stop_run le stop_tt)),t)	; cambiato "AND" con "OR"
		for ggg=0,n_elements(a)-1 do begin
			if (a(ggg) eq -1) then begin
				if keyword_set(SAA) then begin
					print, 'no time correspondance in soe data for SAA reference'
					saa = -1
					return, saa
				endif else if keyword_set(BOOST) then begin
					print, 'no time correspondance in soe data for ECLIPSE reference'
					boost = -1
					return, boost
				endif else BEGIN
					print,'NO TIME CORRESPONDANCE & NO KEYWORD SELECTION FOR READ_SOE_INDEX.PRO. END'
					GOTO, jump
				endelse
			endif else begin
				if keyword_set(SAA) then begin
					saa=read_soe(index_soe.name(a(ggg)),start_run,stop_run,0.0,/SAA)
					if (saa(0) EQ -2) then begin
						;print,index_soe.name(a(ggg))
						;print,transpose(index_soe.name(a))
						continue
					endif
					return, saa
				endif else if keyword_set(BOOST) then begin
					boost=read_soe(index_soe.name(a(ggg)),start_run,stop_run,semi_period,/BOOST)
					if (boost(0) EQ -4) then begin
						;print,index_soe.name(a(ggg))
						;print,transpose(index_soe.name(a))
						continue
					endif
					return, boost
				endif else BEGIN
					print,'NO KEYWORD SELECTION FOR READ_SOE_INDEX.PRO. END'
					GOTO, jump
				endelse
			endelse
		endfor
		print,transpose(index_soe.name(a))
	end
else: 	begin
		print,'INCORRECT PARAMETERS NUMBER FOR READ_SOE_INDEX.PRO'
		return, -3
	end
endcase
if keyword_set(SAA)   then return,SAA
if keyword_set(BOOST) then return,BOOST
jump:
;stop
end
