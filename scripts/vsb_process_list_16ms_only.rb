# questo script processa una serie di orbite riportate in un file con il relativo t0
# questo script e' basato su grb_process_list.rb r grb3.rb
# il file di input e' TGF_analyzer_1.out prodotto da FF con le coordinate geografiche dei trigger
# in output viene riportato lo stesso file con, per ogni riga, aggiunte le informazioni sul trigger

# ARGV[0] input file
# ARGV[1] output path

# suitable for 16-ms only triggers

if ARGV.size == 2  then
	vsb_global_filename = String.new(ARGV[1]+"/VSB_global_info.dat")
	vsb_global_file = File.new(vsb_global_filename,"a")

	if FileTest.file?(ARGV[0]) then
		File.open(ARGV[0]).each_line do | fileline | 
			a = fileline.split(" ")
			puts "processing orbit "+a[0]+" at t0="+a[2]
			
			# select 16ms triggers only
			if a[7]=="GRB"  &&  a[10].to_i==0  &&  a[11].to_i==1  &&  a[13].to_i==1 then
				vsb_global_file << fileline.tr("\n", " ")
			
				# get root file from the archive
				root_filename = String.new("/data1/mcal/MCAL_ARCHIVE/3908/root_file/RT"+a[0]+"_3908.root")
				
				# create output dir if needed
				outpath =  String.new(ARGV[1]+"/"+a[0]+"-"+a[1]+"/")
				if !FileTest.directory?(outpath) then
					Dir.mkdir(outpath)
				end
				
				#do VSB analysis on different time scales
				#system "plotgrb -r "+root_filename+" -t0 "+a[2]+" -3 3 -tbin 0.001 -batch -o "+outpath+" -diag -save -subms 0.016 22"
				
				for i in [1, 2, 4, 8, 16]
					if !FileTest.directory?(outpath+"/"+i.to_s) then
						Dir.mkdir(outpath+"/"+i.to_s)
					end
					tbin = 0.001*i
					system "plotgrb -r "+root_filename+" -t0 "+a[2]+" -30 30 -tbin "+tbin.to_s+" -batch -o "+outpath+"/"+i.to_s+" -z -0.5 0.5 -diag -save "
				end

				# update global file
				# File.open(outpath+"/VSB_info.txt").each_line { | fileline2 | vsb_global_file << fileline2 }
			end
			
		end
		
	else
		puts "vsb_process_list.rb: "+input_filename+" does not exist: exiting!"
	end

else 
	puts "vsb_process_list.rb: NO CORRECT PARAMETERS: need INPUT FILE and OUTPUT_PATH"
end
