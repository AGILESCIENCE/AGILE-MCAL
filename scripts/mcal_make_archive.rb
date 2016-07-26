#!/usr/bin/env ruby

# File ruby ORI mcal_make_archive.rb da M. Marisaldi
# MM 30/04/2015: commentate chiamate a root2fits (non necessario per generare archivio)
# Modificato da C. Pittori per ASDC 3-3-2015
# usage mcal_make_archive.rb XXXXXX YYYYYY 
# ARGV[0] initial orbit number
# ARGV[1] final orbit number

<<<<<<< mcal_make_archive.rb
# MM 22/10/2015 change the MCAL archive path to cope with data access rights (workaround for TGF pipeline). Add leading zeroes.
# MM 17/12/2014 temporary file with grb info deleted only at the beginning. Can be appended to file mcal_grb.dat

# tmp_filename = String.new("/home/mcal/data_analysis/grboutput.txt")
tmp_filename = String.new("tmp_grb.dat");

# remove temporary file if needed
if FileTest.file?(tmp_filename) then
        File.delete(tmp_filename)
end

=======
>>>>>>> 1.2
for orbit in ARGV[0] .. ARGV[1]

	puts("mcal_make_archive.rb: Processing orbit "+orbit); 

<<<<<<< mcal_make_archive.rb
        input_3908 = String.new("/AGILE_PROC3/DATA/COR/0"+orbit+"/PKP0"+orbit+"_1_3908_000.cor.gz");
        input_3917 = String.new("/AGILE_PROC3/DATA/COR/0"+orbit+"/PKP0"+orbit+"_1_3917_000.cor.gz");
        # input_3916 = String.new("/AGILE_PROC3/DATA/LV1/"+orbit+"/PKP"+orbit+"_1_3916_000.lv1.gz");
=======
        input_3908 = String.new("/storage1/agile/agile2/LV1corr/"+orbit+"/VC1/PKP"+orbit+"_1_3908_000.lv1.cor.gz");
        input_3917 = String.new("/storage1/agile/agile2/LV1corr/"+orbit+"/VC1/PKP"+orbit+"_1_3917_000.lv1.cor.gz");

# provo a copiare in locale
#        input_3908 = String.new("/home/agileuser/MCAL_PROC/ARCHIVE"+orbit+"_1_3908_000.lv1.cor.gz");

        # input_3916 = String.new("/storage1/agile/agile2/LV1corr/"+orbit+"/PKP"+orbit+"_1_3916_000.lv1.gz");
        # tmp_filename = String.new("/home/mcal/data_analysis/grboutput.txt")
        tmp_filename = String.new("tmp_grb.dat");
>>>>>>> 1.2

<<<<<<< mcal_make_archive.rb
        #archive_path  = String.new("/MCAL_PROC/ARCHIVE/"+orbit+"/");
        #analysis_path = String.new("/MCAL_PROC/ANALYSIS/"+orbit+"/");
        archive_path  = String.new("/home/mcal/data_analysis/MCAL_PROC2/ARCHIVE/0"+orbit+"/");
        analysis_path = String.new("/home/mcal/data_analysis/MCAL_PROC2/ANALYSIS/0"+orbit+"/");
=======

        archive_path  = String.new("/home/agileuser/MCAL_PROC/ARCHIVE/"+orbit+"/");
        analysis_path = String.new("/home/agileuser/MCAL_PROC/ANALYSIS/"+orbit+"/");
>>>>>>> 1.2
        archive_logfile  = String.new(archive_path+"process_"+orbit+".log");
        # analysis_logfile = String.new(analysis_path+"analysis_"+orbit+".log")

        output_3908 = String.new(archive_path+"RT0"+orbit+"_3908.root");
        output_3917 = String.new(archive_path+"RT0"+orbit+"_3917.root");

        # create output dir if needed
        if !FileTest.directory?(archive_path) then
                Dir.mkdir(archive_path)
        end

        # process input 3917 file and move data
	if !FileTest.file?(output_3917) then
		if FileTest.file?(input_3917) then
<<<<<<< mcal_make_archive.rb
			#system "mcalanalyzer "+input_3917+" >> "+archive_logfile
			#system "mv $MCALSW/bin/mcalsw_output/RT/RT0"+orbit+"_3917.root "+archive_path
			#system "rm $MCALSW/bin/mcalsw_output/H/H0"+orbit+"_3917.root "
			#root_file_3917 = String.new(archive_path+"RT0"+orbit+"_3917.root")
			#system "root2fits "+root_file_3917+" "+archive_path+orbit+"_3917.fits.gz"+" >> "+archive_logfile
=======
			system "mcalanalyzer "+input_3917+" >> "+archive_logfile
			system "mv $MCALSW/bin/mcalsw_output/RT/RT"+orbit+"_3917.root "+archive_path
			system "rm $MCALSW/bin/mcalsw_output/H/H"+orbit+"_3917.root "
			root_file_3917 = String.new(archive_path+"RT"+orbit+"_3917.root")
			#system "root2fits "+root_file_3917+" "+archive_path+orbit+"_3917.fits.gz"+" >> "+archive_logfile
>>>>>>> 1.2
		end
        end


        # process input 3908 file and move data
	#if !FileTest.file?(output_3908) then #overwrite previous file. needed to rebuild the archive after contact 043612 on 29/09/2015
		if FileTest.file?(input_3908) then
			puts String.new("grb4.rb: "+input_3908+" is a regular file: processing...")
	
			#if FileTest.file?(tmp_filename) then    # remove temporary file if needed
			#	File.delete(tmp_filename)
			#end
	
			system "mcalanalyzer "+input_3908+" -grb "+tmp_filename+" >> "+archive_logfile
<<<<<<< mcal_make_archive.rb
			system "mv $MCALSW/bin/mcalsw_output/RT/RT0"+orbit+"_3908.root "+archive_path
			system "rm $MCALSW/bin/mcalsw_output/H/H0"+orbit+"_3908.root "
			root_file_3908 = String.new(archive_path+"RT0"+orbit+"_3908.root")
			system "root2fits "+root_file_3908+" "+archive_path+orbit+"_3908.fits.gz"+" >> "+archive_logfile
=======
			system "mv $MCALSW/bin/mcalsw_output/RT/RT"+orbit+"_3908.root "+archive_path
			system "rm $MCALSW/bin/mcalsw_output/H/H"+orbit+"_3908.root "
			root_file_3908 = String.new(archive_path+"RT"+orbit+"_3908.root")
			#system "root2fits "+root_file_3908+" "+archive_path+orbit+"_3908.fits.gz"+" >> "+archive_logfile
>>>>>>> 1.2
		end
	#end

end
