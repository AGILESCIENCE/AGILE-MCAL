
# Copyright (c) 2017, AGILE team
# Authors: Nicolo' Parmiggiani <nicolo.parmiggiani@gmail.com>,
#
# Any information contained in this software is property of the AGILE TEAM
# and is strictly private and confidential. All rights reserved.



require 'rubygems'
require 'mysql'
require './config'


begin

config = getConfig

con = Mysql.new config[0], config[1], config[2], config[3]

if(ARGV[0]!="pass")
  puts "wrong password"
  exit()
end

query = "delete from geo_tgf"
con.query(query)

query = "delete from lc_ste"
con.query(query)

query = "delete from ste"
con.query(query)

query = "delete from lc_grb"
con.query(query)

query = "delete from grb"
con.query(query)

query = "delete from lc_tgf"
con.query(query)

query = "delete from tgf"
con.query(query)

query = "delete from orbit"
con.query(query)

end
