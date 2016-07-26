if ARGV.size != 3 then

#	INPUT PARAMETERS :
#	$1	OUTPUT path
#	$2	COMPLETE PATH OF 3913 FILE
#	$3	ORBIT
	
	puts("\nIncorrect number of parameters","for mail_to_GRB_local.rb",ARGV,"","Cannot verify the condition","to send the mail","");
	mail_obj = ""
else
	Dir.chdir(ARGV[0])
	dir0 = Dir[ARGV[0]+"GRB*.ok"]
	dir1 = Dir[ARGV[0]+"light_curve.launch"]
	# dir2 = Dir[ARGV[0]+"MCAL*.png"]
	if dir0[0] != nil  then
		i=0
		dir0.each do | file_name |
			File.open(file_name).each_line do | line |
				word=line.split(" ");
				if word[0] == nil 
					next
				elsif word[0] == "Possible" then
					if dir1[0] != nil then
					File.open(dir1[0]).each_line do | time_tt |
						puts "\ngenerating MCAL light curve\n\n";
						system "mcalanalyzer "+ARGV[1]+" -u > plot_out ";
						system "mv $MCALSW/bin/mcalsw_output/misc/MCAL_global_data.root "+ARGV[0].to_s;
						system "plotmcaldata -f " + ARGV[0].to_s + "MCAL_global_data.root -p " + ARGV[0].to_s + " -b -z " + time_tt.to_s + " >> plot_out ";
						system "rm plot_out";
						alert_grid=String.new("TOMAIL:   fuschino@iasfbo.inaf.it  \nALERT_TYPE:   MCAL 3913 ground trigger  \nALERT_TYPE_CODE:   20  \nBURST_START:   "+time_tt.to_s.chop+"\nORBIT_NUM:   "+ARGV[2] );
						alert_path=ARGV[0]+"alert_grid.tmp";
						alert_txt=File.new(alert_path,"w+");
						alert_txt.write(alert_grid);
						alert_txt.close;
						mail_GRID_send="nail -s 'MCAL trigger @ orbit "+ARGV[2].to_s+"'  -A mcal -c fuschino@iasfbo.inaf.it agile_grid_burst@iasfbo.inaf.it < "+alert_path
#						mail_GRID_send=String.new("nail -s 'MCAL trigger @ orbit "+ARGV[2].to_s+"'  -A mcal fuschino@iasfbo.inaf.it < "+alert_path);
						system(mail_GRID_send);
					end
					dir2 = Dir[ARGV[0]+"MCAL*.png"]
					mail_obj=String.new(line.to_s)
					send_mail=String.new("nail -s '" + mail_obj.chop + "' -a " + dir2[0].to_s + " -A mcal fuschino@iasfbo.inaf.it < " + dir0[i])
					# system send_mail
					puts "\n\nGRB detection by local tool with following Object: \n" + mail_obj.chop + "\nand with " + dir2[0].to_s + "   in attach \n\n"
					puts dir2
					# system "rm "+dir2[0].to_s
					system "rm "+ARGV[0].to_s+"light_curve.launch"
					system "rm "+ARGV[0].to_s+"MCAL_global_data.root"
					system "mv "+dir0[i]+" "+dir0[i]+".old"
					break
					end
				elsif word[0] == "No" then
					mail_obj=String.new(line.to_s)
					# system "nail -s '"+mail_obj+"' -A mcal fuschino@iasfbo.inaf.it < "+dir0[i]
					puts "\nGRB detection by local tool with following Object: \n\n"+mail_obj+"\n\n"
					if word[1] == "SOE" then
                         			mail_obj=String.new(line.to_s)
						# system "nail -s '"+mail_obj+"' -A mcal gtb@iasfbo.inaf.it < "+dir0[i]
						puts "\nGRB detection by local tool with following Object: \n\n"+mail_obj+"\n\n"
						#break
					end
					# system "rm "+dir0[i]
					system "mv "+dir0[i]+" "+dir0[i]+".old"
					break
				else 
					mail_obj =String.new("OBJECT NO DEFINED")
					 #system "nail -s '"+mail_obj+"' -A mcal fuschino@iasfbo.inaf.it < "+dir0[i]
					 system "rm "+dir0[i]
					system "mv "+dir0[i]+" "+dir0[i]+".old"
					puts "\nGRB detection by local tool with following Object: \n\n"+mail_obj+"\n\n"
					puts " line check "+line.to_s
					puts file_name
					break
				end
			end
			i+=1
		end
	else 
		puts "no GRB detection by local tool\n"
	end 
end
