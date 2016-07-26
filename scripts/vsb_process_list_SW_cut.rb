# questo script processa una serie di orbite riportate in un file con il relativo t0
# questo script e' basato su grb_process_list.rb r grb3.rb
# il file di input e' TGF_analyzer_1.out prodotto da FF con le coordinate geografiche dei trigger
# in output viene riportato lo stesso file con, per ogni riga, aggiunte le informazioni sul trigger

# questa versione riprocessa il .cor con il piu' recente file di calibrazione (*gain3.conf)
# ed esegue una selezione in energia barra per barra.

# ARGV[0] input file
# ARGV[1] output path

if ARGV.size == 2  then
	vsb_global_filename = String.new(ARGV[1]+"/SW_global_info_cut.dat")
	vsb_global_file = File.new(vsb_global_filename,"a")

	if FileTest.file?(ARGV[0]) then
		File.open(ARGV[0]).each_line do | fileline | 
			a = fileline.split(" ")
			puts "processing orbit "+a[0]+" at t0="+a[2]
			
			if a[7]=="GRB" && a[11].to_i>1 then	# process SW triggers
				vsb_global_file << fileline.tr("\n", " ")
			
				# process .COR file
				cor_filename = String.new("/AGILE_DATA/COR/"+a[0]+"/PKP"+a[0]+"_1_3908_000.cor.gz")
				system "mcalanalyzer "+cor_filename
				
				# create output dir if needed
				outpath =  String.new(ARGV[1]+"/"+a[0]+"-"+a[1]+"/")
				if !FileTest.directory?(outpath) then
					Dir.mkdir(outpath)
				end
				
				# do energy selection
				#system "cd /data1/mcal/mcalsw_output/RT/"
				system "mcalselect -batch -r RT"+a[0]+"_3908.root -eband 0.5 500"
				
				# get root file from standard archive
				root_filename = String.new("/data1/mcal/mcalsw_output/RT/RT"+a[0]+"_3908_sel_01.root")
				
				#do VSB analysis
				system "plotgrb -r "+root_filename+" -t0 "+a[2]+" -3 3 -tbin 0.0001 -batch -o "+outpath+" -diag -save"
				
				# update global file
				File.open(outpath+"/VSB_info.txt").each_line { | fileline2 | vsb_global_file << fileline2 }
			end
			
		end
		
	else
		puts "vsb_process_list.rb: "+input_filename+" does not exist: exiting!"
	end

else 
	puts "vsb_process_list.rb: NO CORRECT PARAMETERS: need INPUT FILE and OUTPUT_PATH"
end
