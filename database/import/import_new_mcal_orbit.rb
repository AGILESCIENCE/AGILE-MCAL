
# Copyright (c) 2017, AGILE team
# Authors: Nicolo' Parmiggiani <nicolo.parmiggiani@gmail.com>,
#
# Any information contained in this software is property of the AGILE TEAM
# and is strictly private and confidential. All rights reserved.


require 'rubygems'
require 'mysql'
require './config'

slurm = 1

begin

  time2 = Time.now
  puts "Current Time : " + time2.inspect

  config = getConfig

  con = Mysql.new config[0], config[1], config[2], config[3]

  #orbit_archive_dir = ARGV[0]
  orbit_archive_dir = "/ANALYSIS3/AGILE-MCAL"

  Dir[orbit_archive_dir+"/*"].each do |dir|

    orbit_dir = dir.split("/").last
    puts "dir"+orbit_dir

    find = false

    query = "select orbit_number from orbit"
    rs = con.query(query)
    rs.each_hash do |row|

      orbit_number = row['orbit_number']
      #puts "num"+orbit_number

      if(orbit_number==orbit_dir)
        find = true
      end

    end

    if(!find)

      job_list =  %x[squeue --format=%j]

      job_list =  job_list.split("\n")
      find_generation = false
      job_list.each do |job_name|
        puts job_name
        if(job_name.delete("\n") == "CALPIPE_"+orbit_dir )
          puts "gia esistente"
          find_generation = true
        end

      end

      job_list =  %x[squeue --format=%j]

      job_list =  job_list.split("\n")
      find_import = false
      job_list.each do |job_name|
        puts job_name
        if(job_name.delete("\n") == "import_"+orbit_dir )
          puts "gia esistente"
          find_import = true
        end

      end

      if(!find_generation && !find_import)

        if(slurm==1)
          cmd =  "sbatch --partition=large submit_mcal_orbit.sh "+orbit_archive_dir+"/"+orbit_dir
          system(cmd)
        else
          puts "importa "+orbit_archive_dir+"/"+orbit_dir
          system("ruby import_mcal_orbit.rb "+orbit_archive_dir+"/"+orbit_dir)
        end
      end
    else
      puts "non importa"
    end
  end




rescue Mysql::Error => e
  puts e.errno
  puts e.error
ensure

  con.close if con

end
