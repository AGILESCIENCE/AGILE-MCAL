Dir.chdir("/AGILE_DATA/AUX")
dir0 = Dir["/AGILE_DATA/AUX/*SOE.ok"]
dir0 = dir0.sort
SOE_path="/home/mcal/RM_search/boost/SOE.index"
findex = File.new(SOE_path,"a");
dir0.each do  | nomefile |
flag="YES"
	#1) leva il .ok al nomefile
	farr = nomefile.split(".");
	nomefile =  farr[0] + "." + farr[1];
	#2) cerca nel SOE.index se nomefile esiste già
	File.open(SOE_path).each_line do | index_line |
		b=index_line.split(" ");
		if b[0] == nomefile then
			flag="NO"
			break
		end
	end
	#3) se non esiste, appendi
	if flag == "YES" then
		File.open(nomefile).each_line do | fileline |
			a = fileline.split(" "); 
			if a[0] == "<Process_TR>"
				starttime =  a[1] + " " + a[2];
				endtime = a[3] + " " + a[4];
				bb = nomefile + " " + starttime + " " + endtime;
				puts "ADDED"
				puts bb;
				findex.write(bb);
				findex.write("\n");
			end        
		end
	else 
	end	
end
findex.close();
