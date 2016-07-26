pro clean2view,contact
home='../'
col_define,39,/verbose
!p.background = !col.white
!p.color = !col.black

restore,home+'ref_boost.index.template'
index=read_ascii(home+'ref_boost.index_',template=boost_index)

ind=where(index.contact eq contact)	
if ind(0) eq -1 then begin
	print,'CONTACT NOT FOUND'
	goto,jump
endif
lv1=RMsReader_MCAL(index.name(ind))
rms=lv1.data
RMs=[rms(0,*) , rms( 2,*)+rms( 3,*) , rms( 4,*) , rms( 5,*) , rms( 6,*)+rms( 7,*)+rms( 8,*)+rms( 9,*)+rms(10,*)+rms(11,*) , $ X chennels
					rms(13,*)+rms(14,*) , rms(15,*) , rms(16,*) , rms(17,*)+rms(18,*)+rms(19,*)+rms(20,*)+rms(21,*)+rms(22,*)]

a=index.contact_ref(ind(0))
b=index.time_ref(ind(0))
c=index.period_ref(ind(0))
prec=0
start_boost     =b+(contact-a-2)*c-80; 120 sec boost duration
start_boost_next=b+(contact+1-a-2)*c-80
stop_boost	=b+(contact-a-2)*c+80; 120 sec boost duration
stop_boost_next	=b+(contact+1-a-2)*c+80
start_tt=RMs(0,0)
stop_tt=RMs(0,n_elements(rms(0,*))-1)
;while (start_boost-start_tt lt 0) do begin
while (start_boost lt start_tt) and (stop_boost lt start_tt) do begin
		prec+=1
		start_boost=b+(contact+prec-a-2)*c-80
		start_boost_next=b+(contact+PREC+1-a-2)*c-80
		stop_boost= b+(contact+prec-a-2)*c+80
		stop_boost_next=	b+(contact+PREC+1-a-2)*c+80
endwhile
soe=read_soe_index(start_tt,stop_tt,/saa)
start_saa = soe(0)
stop_saa  = soe(0)+soe(1)+50.0d
isa=min(abs(rms(0,*)-start_boost(0)),ind_start)
iso=min(abs(rms(0,*)-stop_boost(0)),ind_stop)
s_in=min(abs(rms(0,*)-start_saa(0)),ind_start_saa)
s_fn=min(abs(rms(0,*)-stop_saa(0)),ind_stop_saa)
window,0,title=string(contact),YPOS=200,retain=2
plot,rms(0,*),rms(1,*);,rms(1,*)
;if ((((start_boost gt start_saa) and (start_boost lt stop_saa)) or $
;		((stop_boost gt start_saa) and (stop_boost lt stop_saa))) $
;		or ((ind_start-50 lt 0) or (ind_stop+50 gt n_elements(rms(0,*))+1))) then begin

;if ((start_boost gt start_saa) and (start_boost lt stop_saa)) or $
;		((stop_boost gt start_saa) and (stop_boost lt stop_saa)) or $
if ((ind_start-50 lt 0) or (ind_stop+50 gt n_elements(rms(0,*))+1)) then begin
	boost=max(rms(1,ind_start:ind_stop),ind_boost)
	print,'norm'
	flag='norm'
	print,boost,ind_boost+ind_start,rms(1,ind_boost+ind_start)
	print,ind_start,ind_stop
	oplot,rms(0,ind_start:ind_stop),rms(1,ind_start:ind_stop),color=!col.yellow
endif else begin
	boost=max(rms(1,ind_start-50:ind_stop+50),ind_boost)
	print,'ext'
	print,boost,ind_boost+ind_start-50,rms(1,ind_boost+ind_start-50)
	print,ind_start-50,ind_stop+50
	print,ind_start,ind_stop
	flag='ext'
	oplot,rms(0,ind_start-50:ind_stop+50),rms(1,ind_start-50:ind_stop+50),color=!col.green
endelse
;boost=max(rms(1,ind_start:ind_stop),ind_boost)
stop
rms(1:*,ind_start:ind_stop)=0.0d
rms(1:*,ind_start_saa:ind_stop_saa)=0.0d

if ((start_boost_next-start_tt gt 0) and start_boost_next-start_tt lt stop_tt) then begin
	isa=min(abs(rms(0,*)-start_boost_next(0)),ind_start)
	iso=min(abs(rms(0,*)-stop_boost_next(0)),ind_stop)
	if ind_stop ge n_elements(rms(0,*)-1) then begin
		ind_stop=n_elements(rms(0,*)-1)
	endif
	boost=max(rms(1,ind_start:ind_stop),ind_boost)
	rms(1:*,ind_start:ind_stop)=0.0d
endif
ind_ne_0=where(rms(1,*) ne 0,tot_ne_0)
oplot,rms(0,*),rms(1,*),color=!col.red
window,2
plot,rms(1,*)
;******************************************************************************************
; condition to update boost_ref => 
;		max value in boost range lt 1.0
;		total non zero event lt then BKG_SEC+DELAY 
;		max time distance between good data and START_BOOST gt 30 sec
;		max time distance between good data and STOP_BOOST gt 30 sec
;******************************************************************************************

stop
jump:
end
