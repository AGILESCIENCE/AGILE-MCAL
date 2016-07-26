# convert a list of 35XY files in root format using AB astroroot script
# based on convert_evt2root.rb  
	
File.open("/data/mcal/ARCHIVE_33XY/COR/COR/33XY.dat").each_line do | fileline | 
	b = fileline.split("\n")
	a = fileline.split(".")

	evt_path  = String.new("/data/mcal/ARCHIVE_33XY/COR/COR/"+b[0]);
	root_path = String.new("/data/mcal/ARCHIVE_33XY/ROOT/"+a[0]+".root");

	if not FileTest.file?(root_path)
		cmd = String.new(%Q!astroroot -b -q '/home/mcal/local/scripts/ConvertFromFITSToRoot.C("!+evt_path+%Q!" , "PACKETS" , "!+root_path+%Q!")'!);
	       	puts "processing file "+b[0]
		system(cmd);
	end
end
