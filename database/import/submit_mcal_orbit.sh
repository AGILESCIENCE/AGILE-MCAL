#!/bin/bash


LOG=/ANALYSIS3/log

. ~/.bashrc
cd /home/rt/AGILE-MCAL/database/import
ruby ./import_mcal_orbit.rb $1 >> $LOG/import_mcal_orbit.log  2>&1
