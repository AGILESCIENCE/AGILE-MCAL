# questo script produce il file di output per l'analisi con macro di root combinando il file di input (prodotto da FF: dati di trigger + info orbitali) con il risultato dell'analisi puntuale dei trigger a 16ms, su 5 differenti tempi scala
# questo script e' basato su grb_process_list.rb r grb3.rb
# il file di input e' TGF_analyzer_1.out prodotto da FF con le coordinate geografiche dei trigger
# in output viene riportato lo stesso file con, per ogni riga, aggiunte le informazioni sul trigger

# ARGV[0] input file
# ARGV[1] output path

# suitable for 16-ms only triggers

if ARGV.size == 2  then
	

	if FileTest.file?(ARGV[0]) then
		for i in [1, 2, 4, 8, 16]
			vsb_global_filename = String.new(ARGV[1]+"/VSB_global_info_"+i.to_s+".dat")
			vsb_global_file = File.new(vsb_global_filename,"a")	
			puts "creating file "+vsb_global_filename
			
			File.open(ARGV[0]).each_line do | fileline | 
				a = fileline.split(" ")
				# puts "processing orbit "+a[0]+" at t0="+a[2]
				
				# look if processed data do exist
				outpath =  String.new(ARGV[1]+"/"+a[0]+"-"+a[1]+"/"+i.to_s)
				if FileTest.file?(outpath+"/VSB_info.txt") then
					# update global file
					
					vsb_global_file << fileline.tr("\n", " ")
					File.open(outpath+"/VSB_info.txt").each_line { | fileline2 | vsb_global_file << fileline2 }
					
				end			
			end
		end	
	else
		puts "vsb_process_list.rb: "+input_filename+" does not exist: exiting!"
	end

else 
	puts "vsb_process_list.rb: NO CORRECT PARAMETERS: need INPUT FILE and OUTPUT_PATH"
end
