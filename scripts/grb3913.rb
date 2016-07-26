contact_filename = String.new("/home/mcal/data_analysis/grb3913/grb_3913.dat")
if FileTest.file?(contact_filename) then
	File.open(contact_filename).each_line do | fileline | 
		puts "processing "+contact_filename+" line: "+fileline
		a = fileline.split(" ")
		output_dirname = String.new("/home/mcal/data_analysis/grb3913/00"+a[0])
		if !FileTest.directory?(output_dirname) then
			Dir.mkdir(output_dirname)
		end
		root_filename = String.new("/home/mcal/data_analysis/grb3913/rootfiles/"+a[0]+"_"+a[0]+"_MCAL_global_data.root")
		system "plotgrb -r "+root_filename+" -t0 "+a[2]+" -200 200 -batch -o "+output_dirname+" -save"	
					
		# update global file
		global_file = File.open("/home/mcal/data_analysis/grb3913/grb_info.dat","a")
		File.open(output_dirname+"/grb_info.dat").each_line { | fileline | global_file << fileline }
		
	end
else
	puts "No file "+contact_filename
end