
##1) Extract the 5 Genes from the exon file by looking for the gene ids in the $4 column
##BEGIN{print "--- Start ---"}
##NM_032291 (+)
##NM_024066 (-)
##NM_001199739 (-)
##NM_003689 (-)
##NM_001201547 (-)
#{
#	if(index($4, "NM_032291_") || 
#		index($4, "NM_024066_") || 
#		index($4, "NM_001199739_") || 
#		index($4, "NM_003689_") || 
#		index($4, "NM_001201547_"))
#	{print $0;}
#}
## Found 56 lines
## Then sorted the lines based on starting index (increaseing order)
## sort -k 2 -n < 5RefSeqGene > 5RefSeqGene_sorted
##END{print "--- End ---"}
##1) END ------------------------------------------------

##2) find the min starting index and max ending index to trim chr1 file
#BEGIN{
#	getline;
#	max=$3;
#	min=$2;
#	print("START\nMin: ", min, "\nMax: ", max);
#}
#
#{
#	if($2<min){min=$2; print("Min: ", $2)} 
#	if($3>max){max=$3; print("Max: ", $3)}
#}
#
#END{
#	print("Min: ", min); #min = 19629201 => line 392585 in chr1.fa
#	print("Max: ", max); #max = 150738433 => line 3014769 in chr1.fa
#	print("Highest row: ", )
#}
##2) END ------------------------------------------------

##3) only get the lines that we need. right trim ending lines
#BEGIN{
#	lineCount = -1; #for the beginin >chr1 line
#	maxLine = 3014769;
#}
#
#{
#	lineCount++;
#	if(lineCount<= maxLine){print $0;}
#}
#
#END{print("Lines: ", lineCount);}
##3) END  ------------------------------------------------

##4) do the left trim. removing all the indexs that are lower than the lowest index we want
#BEGIN{
#	lineCount=-1;
#	maxLine = 392585;
#}
#
#{
#	lineCount++;
#	if(lineCount >= maxLine){print $0;}
#}
#
#END{print("Lines: ", lineCount);}
##So starting line is 392585
##4) END  ------------------------------------------------

{
	if(index($4, "NM_032291_")){
		print $0 "\tNM_032291";
	}else if (index($4, "NM_024066_")){
		print $0 "\tNM_024066";
	}else if (index($4, "NM_001199739_")){
		print $0 "\tNM_001199739";
	}else if(index($4, "NM_003689_")){
		print $0 "\tNM_003689";
	}else if(index($4, "NM_001201547_")){
		print $0 "\tNM_001201547";
	}

}
