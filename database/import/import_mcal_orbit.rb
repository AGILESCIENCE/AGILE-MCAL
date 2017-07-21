
# Copyright (c) 2017, AGILE team
# Authors: Nicolo' Parmiggiani <nicolo.parmiggiani@gmail.com>,
#
# Any information contained in this software is property of the AGILE TEAM
# and is strictly private and confidential. All rights reserved.


require 'rubygems'
require 'mysql'
require './config'

# TODO togliere controllo nome orbita uguale

begin

	config = getConfig

	con = Mysql.new config[0], config[1], config[2], config[3]

	dirName = ARGV[0]

	time2 = Time.now
	puts "Current Time : " + time2.inspect

	orbit_number = dirName.split("/").last
	puts "read orbit data "+orbit_number
	#entro nella directory

	Dir.chdir(dirName)

	puts Dir.pwd
	query = "select id_orbit from orbit where orbit_number = '"+orbit_number+"'"
	rs = con.query(query)
	n_rows = rs.num_rows

	id_orbit = ""

	if(n_rows == 0) then
		#creo orbita e aggiungo file name
		query = "insert into orbit (orbit_number,trend) values ('"+orbit_number+"','"+orbit_number+"_trend.png')"
		puts query
		con.query(query)

		query = "select id_orbit from orbit where orbit_number = '"+orbit_number+"'"
		rs = con.query(query)
		rs.each_hash do |row|
			id_orbit = row['id_orbit']
		end
	else

		rs.each_hash do |row|
			id_orbit = row['id_orbit']
		end
	end


	#cerco txt in GRB

	dir = "GRB"


	if (!File.exist?(dir+"/GRB_"+orbit_number+"_list.txt")) then

		puts "no GRB"

	else

		File.open(dir+"/GRB_"+orbit_number+"_list.txt").each do |line|


			puts "new grb "+line
			line_element = line.split(" ")

			contact = line_element[1]

			if(contact.to_s != orbit_number.to_s)
				puts "orbit error"
				next
			end

			t_zero = line_element[2]
			date=line_element[3]
			time=line_element[4]
			bin = line_element[5]


			#insert GRB
			query = "insert into grb (date,time,t_zero,bin,id_orbit) values ('"+date+"','"+time+"','"+t_zero+"','"+bin+"','"+id_orbit+"')"
			puts query
			con.query(query)

			query = "select id_grb from grb where t_zero = '"+t_zero+"'"
			puts query
			rs = con.query(query)
			id_grb= nil
			rs.each_hash do |row|
				id_grb = row['id_grb']
			end

			#inserisci lc
			query = "insert into lc_grb (file_name,id_grb) values ('GRB_"+contact+"_"+t_zero+".png','"+id_grb+"')"
			puts query
			con.query(query)

		end

	end

	#entro in STE
	dir = "STE"


	if (!File.exist?(dir+"/STE_"+orbit_number+"_list.txt")) then

		puts "no STE"

	else
		File.open(dir+"/STE_"+orbit_number+"_list.txt").each do |line|

			puts "new ste "+line
			line_element = line.split(" ")

			contact = line_element[1]

			if(contact.to_s != orbit_number.to_s)
				puts "orbit error"
				next
			end

			t_zero = line_element[2]
			date = line_element[3]
			time = line_element[4]
			bin = line_element[5]
			background = line_element[6]
			threshold = line_element[7]
			counts = line_element[8]
			s_on_n = line_element[9]


			#insert GRB
			query = "insert into ste (date,time,bin,t_zero,s_on_n,background,threshold,counts,id_orbit) values ('"+date+"','"+time+"','"+bin+"','"+t_zero+"','"+s_on_n+"','"+background+"','"+threshold+"','"+counts+"','"+id_orbit+"')"
			puts query
			con.query(query)

			query = "select id_ste from ste where t_zero = '"+t_zero+"' and bin = '"+bin+"'"
			puts query
			rs = con.query(query)
			id_ste= nil
			rs.each_hash do |row|
				id_ste = row['id_ste']
			end

			#inserisci lc
			query = "insert into lc_ste (file_name,id_ste) values ('STE_"+contact+"_"+t_zero+"_"+bin+".png','"+id_ste+"')"
			puts query
			con.query(query)

		end
	end

	#entro in TGF
	dir = "TGF"


	if (!File.exist?(dir+"/TGF_"+orbit_number+"_list.txt")) then

		puts "no TGF"

	else
		File.open(dir+"/TGF_"+orbit_number+"_list.txt").each do |line|


			puts "new tgf "+line
			line_element = line.split(" ")

			contact = line_element[1]

			if(contact.to_s != orbit_number.to_s)
				puts "orbit error"
				next
			end

			t_zero = line_element[2]
			date = line_element[3]
			time = line_element[4]
			local_time = line_element[5]
			counts = line_element[6]
			hr = line_element[7]
			duration = line_element[8]
			e_max = line_element[9]
			e_avg = line_element[10]
			lon = line_element[11]
			lat = line_element[12]

			#insert GRB
			query = "insert into tgf (local_time,date,time,t_zero,counts,hr,duration,e_max,e_avg,lon,lat,id_orbit) values ('"+local_time+"','"+date+"','"+time+"','"+t_zero+"','"+counts+"','"+hr+"','"+duration+"','"+e_max+"','"+e_avg+"','"+lon+"','"+lat+"','"+id_orbit+"')"
			puts query
			con.query(query)

			query = "select id_tgf from tgf where t_zero = '"+t_zero+"'"
			puts query
			rs = con.query(query)
			id_tgf = nil
			rs.each_hash do |row|
				id_tgf = row['id_tgf']
			end

			#inserisci lc
			query = "insert into lc_tgf (file_name,id_tgf) values ('TGF_"+contact+"_"+t_zero+".png','"+id_tgf+"')"
			puts query
			con.query(query)


			#inserisci geo
			query = "insert into geo_tgf (file_name,id_tgf) values ('TGF_"+contact+"_"+t_zero+"_geo.png','"+id_tgf+"')"
			puts query
			con.query(query)



		end
	end

rescue Mysql::Error => e
	puts e.errno
	puts e.error
ensure

	con.close if con

end
