
# read the data inside orbit file 3908 and 3916 in /ANALYSIS3/commands

orbit_3908 = nil
orbit_3908_path = nil
if(File.file?("/ANALYSIS3/commands/3908") && File.file?("/ANALYSIS3/commands/3916"))


  File.open("/ANALYSIS3/commands/3908", "r").each_line do |line|
    puts line
    ll = line.split("/")
    orbit_3908 = ll[ll.size()-1].split(" ")[0].split("_")[0].split("PKP")[1]
    orbit_3908_path =  line.split(" ")[0]
  end

  orbit_3916 = nil
  orbit_3916_path = nil
  File.open("/ANALYSIS3/commands/3916", "r").each_line do |line|
    puts line
    ll = line.split("/")
    orbit_3916 = ll[ll.size()-1].split(" ")[0].split("_")[0].split("PKP")[1]
    orbit_3916_path = line.split(" ")[0]
  end

  # check if both new file are new orbit and about the same orbit
  if(orbit_3916==orbit_3908)

    new_orbit = orbit_3908
    existing_orbit = false

    Dir["/ANALYSIS3/AGILE-MCAL/*"].each do |dir|
      puts dir
      old_orbit_number = dir.split("/").last
      if(old_orbit_number == new_orbit)
        existing_orbit=true
        puts "orbit already exist "+old_orbit_number
      end
    end

    if(!existing_orbit)
      #check slurm if there is job called CALPIPE
      job_list =  %x[squeue --format=%j]

      job_list =  job_list.split("\n")
      find = false
      job_list.each do |job_name|
          puts job_name
        if(job_name.delete("\n") == "CALPIPE_"+new_orbit )
          puts "gia esistente"
          find = true
        end

      end

      #create config file in /ANALYSIS3/commands  for submit_run.rb
      if(!find)
        cmd = "echo \"CALPIPE\n"+orbit_3908_path+"\n"+orbit_3916_path+"\n"+new_orbit+"\" > /ANALYSIS3/commands/"+(rand * 1000000000).to_i.to_s+".conf"
        puts cmd
        system(cmd)
      else
          puts "import in progress"
      end

    end
  else
    puts "different orbit "+orbit_3916+" "+orbit_3908
  end


else
  puts "file 3908 or 3916 not exist"
end
