# questo script sostituisce grb.sh

# ARGV[0] current orbit number
# ARGV[1] output path

# -----------------------------------------------------------

# ChangeLog:

# MM 29/07/2010 aggiunta opzione -ts a plotgrb, altrimenti l'analisi automatica non funziona

# -----------------------------------------------------------

# processdata > 0 ? process cor files and build archive
processdata = 1

# processsw > 0 ? process light curves for SW triggers
processsw = 1

# processhw > 0 ? process light curves for HW triggers
# MM 18/8/2010 NB: if = 0 the automatic TGF processing in mcal@agilehp1:~/TGF_analysis/compile_global_info_3.rb will not work!
processhw = 1

# sendmail > 0 ? email enabled : email disabled
sendmail = 1

# doidl > 0 ? perform IDL analysis on orbital parameters : do nothing
doidl = 0

# dophonecall > 0 ? do telephone call
dophonecall = 0

# movedata > 0 ? move data to /AGILE_DATA/HLT/MCALBURST/ (automatic use on gtb)
movedata = 1

# updateglobal > 0 ? update global GRB/TGF/ECLIPSE file
updateglobal = 1

# doeph > 0 ? use idl procedure to create ephemeris file
doeph = 0

# writedb > 0 ? write trigger info to db via ruby-on-rails Model
writedb = 0

# -----------------------------------------------------------

if ARGV.size == 2  then


# ---------------------- grb4.rb: innesto di test -------------------


	input_3908 = String.new("/AGILE_PROC3/DATA/COR/"+ARGV[0]+"/PKP"+ARGV[0]+"_1_3908_000.cor.gz");
	input_3917 = String.new("/AGILE_PROC3/DATA/COR/"+ARGV[0]+"/PKP"+ARGV[0]+"_1_3917_000.cor.gz");
	input_3916 = String.new("/AGILE_PROC3/DATA/LV1/"+ARGV[0]+"/PKP"+ARGV[0]+"_1_3916_000.lv1.gz");
	# tmp_filename = String.new("/home/mcal/data_analysis/grboutput.txt")
	tmp_filename = String.new("tmp_grb.dat")
	
	archive_path  = String.new("/MCAL_PROC/ARCHIVE/"+ARGV[0]+"/")
	analysis_path = String.new("/MCAL_PROC/ANALYSIS/"+ARGV[0]+"/")
	archive_logfile  = String.new(archive_path+"process_"+ARGV[0]+".log")
	analysis_logfile = String.new(analysis_path+"analysis_"+ARGV[0]+".log")
	
	# create output dir if needed
	if !FileTest.directory?(archive_path) then
		Dir.mkdir(archive_path)
	end
	if !FileTest.directory?(analysis_path) then
		Dir.mkdir(analysis_path)
	end
		
	# process input 3917 file and move data
	if FileTest.file?(input_3917) && processdata > 0 then
		system "mcalanalyzer "+input_3917+" >> "+archive_logfile
		system "mv $MCALSW/bin/mcalsw_output/RT/RT"+ARGV[0]+"_3917.root "+archive_path
		system "rm $MCALSW/bin/mcalsw_output/H/H"+ARGV[0]+"_3917.root "
		root_file_3917 = String.new(archive_path+"RT"+ARGV[0]+"_3917.root")
		system "root2fits "+root_file_3917+" "+archive_path+ARGV[0]+"_3917.fits.gz"+" >> "+archive_logfile
	end
		
	# process input 3908 file and move data
	if FileTest.file?(input_3908) && processdata > 0 then
		puts String.new("grb4.rb: "+input_3908+" is a regular file: processing...")
		
		if FileTest.file?(tmp_filename) then	# remove temporary file if needed
			File.delete(tmp_filename)
		end

		system "mcalanalyzer "+input_3908+" -grb "+tmp_filename+" >> "+archive_logfile
		system "mv $MCALSW/bin/mcalsw_output/RT/RT"+ARGV[0]+"_3908.root "+archive_path
		system "rm $MCALSW/bin/mcalsw_output/H/H"+ARGV[0]+"_3908.root "
		root_file_3908 = String.new(archive_path+"RT"+ARGV[0]+"_3908.root")
		system "root2fits "+root_file_3908+" "+archive_path+ARGV[0]+"_3908.fits.gz"+" >> "+archive_logfile
		
		
		
		# update DB
		if writedb > 0 then
			puts %Q!~/Rails/mcal_trg/script/runner 'Trigger.create_entry_from_file("!+tmp_filename+%Q!")' !
			system %Q!~/Rails/mcal_trg/script/runner 'Trigger.create_entry_from_file("!+tmp_filename+%Q!")' !
		end
		
		
		
		# ------------ process each line of temp file -------------
		File.open(tmp_filename).each_line do | fileline | 
			# puts "grb4.rb: processing "+tmp_filename+" line: "+fileline
			a = fileline.split(" ")
			puts "processing orbit "+a[0]+" trigger "+a[1]+" at t0="+a[2]+"  classified of type "+a[9]
			
			# create output dir if needed and process data only if new
			outpath =  String.new(analysis_path+"/trg_"+a[1]+"/")
			if !FileTest.directory?(outpath) then
				Dir.mkdir(outpath)
			
				# --------- process software triggers -------
				if a[9] != "BOOST" && a[15].to_i > 1 && processsw > 0
					for i in [160, 320, 640, 1280, 2560, 5120]
						if !FileTest.directory?(outpath+"/tbin_"+i.to_s) then
							Dir.mkdir(outpath+"/tbin_"+i.to_s)
						end
						tbin = 0.0001*i
						cmd = String.new("plotgrb -r "+root_file_3908+" -t0 "+a[2]+" -15 60 -tbin "+tbin.to_s+" -batch -o "+outpath+"/tbin_"+i.to_s+"  -save")
						# puts cmd
						system  cmd+" >> "+analysis_logfile
						
						cmd = "replace nan 0 -- "+outpath+"/tbin_"+i.to_s+"/VSB_info.txt >> "+analysis_logfile
						system (cmd)
					end
				end
				
				
				# --------- process hardware triggers -------
				if a[9] != "BOOST" && a[15].to_i <= 1 && processhw > 0
					for i in [5, 10, 20, 40]
						if !FileTest.directory?(outpath+"/tbin_"+i.to_s) then
							Dir.mkdir(outpath+"/tbin_"+i.to_s)
						end
						tbin = 0.0001*i
						cmd = String.new("plotgrb -r "+root_file_3908+" -t0 "+a[2]+" -1 3 -tbin "+tbin.to_s+" -batch -o "+outpath+"/tbin_"+i.to_s+"  -bkg -1 0 -grb 0 0.020  -subms 0.016 23 -ts 0.020 -diag -save")
						# puts cmd
						system  cmd+" >> "+analysis_logfile
						
						cmd = "replace nan 0 -- "+outpath+"/tbin_"+i.to_s+"/VSB_info.txt >> "+analysis_logfile
						system (cmd)

					end
				end
				
				# --------- calculate ephemeris for every trigger
				if doeph > 0 then
					launch_idl_eph=String.new(".compile MCAL_eph_v0_2.pro \n MCAL_eph, '"+input_3916+"' , '"+outpath+"' , 'MCAL_"+ARGV[0]+"_"+a[1]+"' , "+t0);
					puts launch_idl_eph
					idl_eph_in=File.new("./idl_eph.in","w+");
					idl_eph_in.write(launch_idl_eph);
					idl_eph_in.close;
					system "idl < ./idl_eph.in";
				end

			else
				puts "directory "+outpath+" already exist. Stop processing."
			end
		end

	else
		puts "grb4.rb: "+input_3908+" does not exist: exiting!"
	end


# -----------------------------------------------------------


	input_filename = String.new("/AGILE_PROC3/DATA/COR/"+ARGV[0]+"/PKP"+ARGV[0]+"_1_3908_000.cor.gz");
	#tmp_filename = String.new("tmp_grb.dat")
	global_filename = String.new("/AGILE_DATA/HLT/MCALBURST/mcal_grb.dat")
	grb_global_filename = String.new("/AGILE_DATA/HLT/MCALBURST/MCAL_GRB/grb_global_info.dat")
	tgf_global_filename = String.new("/AGILE_DATA/HLT/MCALBURST/MCAL_TGF/tgf_global_info.dat")
	ecl_global_filename = String.new("/AGILE_DATA/HLT/MCALBURST/MCAL_ECLIPSE/ecl_global_info.dat")
	
	if FileTest.file?(input_filename) then
		puts String.new("grb3.rb: "+input_filename+" is a regular file: processing...")
		
		if FileTest.file?(tmp_filename) then	# remove temporary file if needed
			File.delete(tmp_filename)
		end
		
		# create output dir if needed
		if !FileTest.directory?(ARGV[1]) then
			Dir.mkdir(ARGV[1])
		end

		
		# check if file is BOOST, ECLIPSE or GRB and write info in temp file
		system "mcalanalyzer "+input_filename+" -grb tmp_grb.dat -noproc"
		#system "mcalanalyzer "+input_filename+" -grb tmp_grb.dat"
		#root_filename = String.new("$MCALSW/bin/mcalsw_output/RT/RT"+ARGV[0]+"_3908.root")  
                root_filename = String.new(archive_path+"RT"+ARGV[0]+"_3908.root")
		
		#process each line of temp file
		File.open(tmp_filename).each_line do | fileline | 
			puts "grb3.rb: processing "+tmp_filename+" line: "+fileline
			
			# update global trigger file
			if updateglobal > 0 then
				global_file = File.open(global_filename,"a")
				global_file << fileline
			end
			
			a = fileline.split(" ")
			t0 = a[2]
			tstop = a[3]
			dt = a[3].to_f - a[2].to_f + 6.0
			
			if a[9] == "GRB" && a[15].to_i > 1 then
				puts "grb3.rb: This event is a GRB: processing..."
				#system "mcalanalyzer "+input_filename
				
				# find out the proper time binning
				nrm = a[15].to_i
				idrm = Array.new(nrm)
				idrm.each_index{|id| idrm[id] = a[16 + id*4]}
				idsit = Array.new(nrm)
				idsit.each_index{|id| idsit[id] = a[17 + id*4]}
				tbin = "0.032"
				case idsit.min
					when "5" then tbin = "0.512"
					when "4" then tbin = "0.256"
					when "3" then tbin = "0.064"
					when "2" then tbin = "0.016"
					when "1" then tbin = "0.001"
					when "0" then tbin = "0.001"
					else tbin = "0.032"
				end
				puts "grb3.rb: minimum SIT number = "+idsit.min+" --> use time binning "+tbin+" s"
				
				# plot light curve (image)
				#system "plotgrb -r "+root_filename+" -t0 "+t0+" -80 "+dt.to_s+" -tbin "+tbin+" -eband 0. 1. -batch -o "+ARGV[1]+" -save"
				puts "plotgrb -r "+root_filename+" -t0 "+t0+" -15 "+dt.to_s+" -tbin "+tbin+" -ts "+dt.to_s+" -batch -o "+ARGV[1]+" -save"
				system "plotgrb -r "+root_filename+" -t0 "+t0+" -15 "+dt.to_s+" -tbin "+tbin+" -ts "+dt.to_s+" -batch -o "+ARGV[1]+" -save"
				
				# prepare light curve for IPN
				system "plotgrb -r "+root_filename+" -t0 "+t0+" -15 "+dt.to_s+" -tbin 0.032 -eband 0. 1. -batch -o "+ARGV[1]+" -ipn -15. "+dt.to_s
			
				# prepare attachmentes for emails
				mail_attach  = String.new()
				mail_body    = String.new(ARGV[1]+"/mcal_grb3908_msg.txt")
				files = Dir.entries(ARGV[1])
				files.each {|x| 
					if x.include?("MCAL") && !x.include?("_lc") && !x.include?("_eph") then  
						mail_attach << " -a " << ARGV[1] << x
					end
					}
				
				# check if it is really a GRB or a candidate TGF (look for T90)
				info_filename = String.new(ARGV[1]+"/grb_info.dat")
				t90 = 2.0
				if FileTest.file?(info_filename) then
					l = File.open(info_filename).readline()
					b = l.split(" ")
					t90 = b[7].to_f
				else
					puts "grb3.rb: "+info_filename+" does not exist: something wrong with plotgrb!"
					t90 = 10.0
				end
				#launch_idl=String.new(".compile eclipse_index eclipse_aux MCAL_transient_html.pro \n MCAL_transient_html, '"+ARGV[1]+"' , '"+ARGV[0]);
				
				# create ephemeris file for time T0
				if doeph > 0 then
					launch_idl_eph=String.new(".compile MCAL_eph_v0_2.pro \n MCAL_eph, '/AGILE_DATA/LV1/"+ARGV[0]+"/PKP"+ARGV[0]+"_1_3916_000.lv1.gz' , '"+ARGV[1]+"' , 'MCAL_"+ARGV[0]+"' , "+t0);
					puts launch_idl_eph
					idl_eph_in=File.new("./idl_eph.in","w+");
					idl_eph_in.write(launch_idl_eph);
					idl_eph_in.close;
					system "idl < ./idl_eph.in";
				end

				# do specific analysis for GRB and TGF
				if t90 > 0.1 then
					# I assume it is a GRB
					puts "grb3.rb: t90 = "+t90.to_s+"  probable GRB"
					
					# launch Fabio IDL tool for orbital parameters extraction
					if doidl > 0 then
						launch_idl << "', /GRB"
						puts launch_idl
						idl_in=File.new("./idl.in","w+");
						idl_in.write(launch_idl);
						idl_in.close;
						system "idl < ./idl.in";
					end
					
					# send alert email
					mail_subject = String.new("'MCAL GRB on orbit "+ARGV[0]+" ")
					mail_subject += " SIT= "+idsit.min+"  RM= "
					idrm.each_index{|id| mail_subject += idrm[id]+" " }
					mail_subject += "'"
					#mail_address = String.new("mcal@iasfbo.inaf.it")
					mail_address = String.new("marisaldi@iasfbo.inaf.it, fuschino@iasfbo.inaf.it, delmonte@iasf-roma.inaf.it , feroci@iasf-roma.inaf.it")
					#mail_address = String.new("agile@iasfbo.inaf.it, burst@asdc.asi.it, franz@lambrate.inaf.it, dicocco@iasfbo.inaf.it, sa.grb@iasf-roma.inaf.it, agile_grid_burst@iasfbo.inaf.it")
					if sendmail > 0 then
						puts "nail -s "+mail_subject+" -A mcal "+mail_attach+" "+mail_address+" < "+mail_body
						system "nail -s "+mail_subject+" -A mcal "+mail_attach+" "+mail_address+" < "+mail_body
						#system "nail -s "+mail_subject+" -A MM marty74@vodafone.it < "+mail_body
					end
					
					# do telephone call
					if dophonecall > 0 then
						system "wvdial --config ~/DIALING/wvdial.cfg  MartinoCell"
						system "wvdial --config ~/DIALING/wvdial.cfg  FabioCell"
					end
					
					# if it is a VERY INTENSE EVENT do telephone call
					fluence = b[20].to_f
					if fluence > 5000.0 then
						puts "RED ALERT!!! High fluence event: fluence = "+b[20]
						if dophonecall > 0 then
							# system "wvdial --config ~/DIALING/wvdial.cfg  TavaniCell"
						end
					end
					
					# update global file
					#if updateglobal > 0 then
					#	grb_global_file = File.new(grb_global_filename,"a")
					#	File.open(ARGV[1]+"/grb_info.dat").each_line { | fileline | grb_global_file << fileline }
					#end
					
					# move analysis directory to the appropriate repository
					if movedata > 0 then 
						web_dirname = String.new("/AGILE_DATA/HLT/MCALBURST/GRB/"+ARGV[0])
						if !FileTest.directory?(web_dirname) then
							Dir.mkdir(web_dirname)
						end
						puts "cp "+ARGV[1]+"/* "+web_dirname
						system "cp "+ARGV[1]+"/* "+web_dirname
						
						grb_dirname = String.new("/AGILE_DATA/HLT/MCALBURST/MCAL_GRB/"+ARGV[0]+"_"+a[5]+"_"+a[6])
						if !FileTest.directory?(grb_dirname) then
							Dir.mkdir(grb_dirname)
						end
						puts "mv "+ARGV[1]+"/* "+grb_dirname
						system "mv "+ARGV[1]+"/* "+grb_dirname
					end
					
				else
					# I assume it is a candidate TGF
					#puts "grb3.rb: t90 = "+t90.to_s+"  candidate TGF"
				
					# replot light curve (image) with 0.1 ms time bin
					#system "plotgrb -r "+root_filename+" -t0 "+t0+" -80 "+dt.to_s+" -tbin 0.0001 -eband 0. 1. -batch -z -0.010 0.010 -o "+ARGV[1]+" -save"
				
					# launch Fabio IDL tool for orbital parameters extraction
					#if doidl > 0 then
					#	launch_idl << "', /TGF"
					#	puts launch_idl
					#	idl_in=File.new("./idl.in","w+");
					#	idl_in.write(launch_idl);
					#	idl_in.close;
					#	system "idl < ./idl.in";
					#end
					
					# send alert email
					#mail_subject = String.new("'MCAL candidate TGF on orbit "+ARGV[0]+"'")
					#mail_address = String.new("marisaldi@iasfbo.inaf.it")
					#mail_address = String.new("mcal@iasfbo.inaf.it")
					#if sendmail > 0 then
						#system "nail -s "+mail_subject+" -A mcal "+mail_attach+" "+mail_address+" < "+mail_body
					#end
					
					# update global file
					#if updateglobal > 0 then
					#	tgf_global_file = File.new(tgf_global_filename,"a")
					#	File.open(ARGV[1]+"/grb_info.dat").each_line { | fileline | tgf_global_file << fileline }
					#end
					
					# move analysis directory to the appropriate repository
					#if movedata > 0 then 
					#	tgf_dirname = String.new(ARGV[1]+"/../MCAL_TGF/"+ARGV[0]+"_"+a[5]+"_"+a[6])
					#	if !FileTest.directory?(tgf_dirname) then
					#		Dir.mkdir(tgf_dirname)
					#	end
					#	system "mv "+ARGV[1]+"/* "+tgf_dirname
					#end
				end
				
	
			elsif a[9] == "ECLIPSE" then
				puts "grb3.rb: This event is an ECLIPSE: processing..."
				system "mcalanalyzer "+input_filename
				system "plotgrb -r "+root_filename+" -t0 "+t0+" -80 "+dt.to_s+" -tbin 0.0001 -ts "+dt.to_s+" -eband 0. 1. -batch -z -0.010 0.010 -o "+ARGV[1]+" -save"
			
				# send alert email
				mail_subject = String.new("'MCAL probable ECLIPSE event on orbit "+ARGV[0]+"'")
				mail_address = String.new("marisaldi@iasfbo.inaf.it")
				#mail_address = String.new("mcal@iasfbo.inaf.it")
				mail_attach  = String.new()
				mail_body    = String.new(ARGV[1]+"/mcal_grb3908_msg.txt")
				files = Dir.entries(ARGV[1])
				files.each {|x| 
					if x.include?("MCAL") then  
						mail_attach << " -a " << ARGV[1] << x
					end
					}
				if sendmail > 0 then
					# do not send emails for eclipse events anymore
					# system "nail -s "+mail_subject+" -A mcal "+mail_attach+" "+mail_address+" < "+mail_body
				end
				
				# update global file
				if updateglobal > 0 then
					ecl_global_file = File.new(ecl_global_filename,"a")
					File.open(ARGV[1]+"/grb_info.dat").each_line { | fileline | ecl_global_file << fileline }
				end
				
				# move analysis directory to the appropriate repository
				if movedata > 0 then 
					ecl_dirname = String.new("/AGILE_DATA/HLT/MCALBURST/MCAL_ECLIPSE/"+ARGV[0]+"_"+a[5]+"_"+a[6])
					if !FileTest.directory?(ecl_dirname) then
						Dir.mkdir(ecl_dirname)
					end
					system "mv "+ARGV[1]+"/* "+ecl_dirname
				end
								
			elsif a[9] == "BOOST" then
				puts "grb3.rb: This event is a BOOST: nothing to be done."
								
			elsif a[9] == "UNK" then
				puts "grb3.rb: This event is not in the BOOST.dat range: check manually."
			
				# send alert email
				mail_subject = String.new("'MCAL UNKNOWN event on orbit "+ARGV[0]+"'")
				#mail_address = String.new("mcal@iasfbo.inaf.it")
				mail_address = String.new("marisaldi@iasfbo.inaf.it")
				mail_body    = String.new("/AGILE_DATA/HLT/MCALBURST//unknown_msg.dat")
				if !FileTest.file?(mail_body) then
					mail_file = File.new(mail_body,"a")
					mail_file << "Probable missed BOOST event due to reprocessing. Check manually!"
					mail_file.close					
				end
				if sendmail > 0 then
					system "nail -s "+mail_subject+" -A mcal "+mail_address+" <"+mail_body
				end
			else
				puts "grb3.rb: Unknown identifier "+a[9]
			end
		end
		
		if movedata > 0 then
			Dir.rmdir(ARGV[1])
		end

	else
		puts "grb3.rb: "+input_filename+" does not exist: exiting!"
	end

else 
	puts "grb3.rb: NO CORRECT PARAMETERS: need ORBIT (6 digits) and OUTPUT_PATH"
end

# ------------- remove everything in mcalsw_output (temporary solution MM 26/05/10) --------------

system "rm $MCALSW/bin/mcalsw_output/H/*.root "
system "rm $MCALSW/bin/mcalsw_output/RT/*.root "
system "rm $MCALSW/bin/mcalsw_output/misc/* "


