#comment in awk
#BEGIN only executed once at the start
BEGIN{print "---- Start ----"}

#{
#	if(index($1, "NR")>0)
#	{
#		print $0;
#	}
#}

#count how many bases (nucleotides) in file ie. not starting with >chr1
#BEGIN{
#	if(index($1, ">") >0)
#	{
#		print $0;
#	}else{
#		len=len+length($1); print len;
#	}
#}

#input would be the HG19-gene-annot-all file
#only want the lines that start with chr1
#BEGIN{
#	if($1=="chr1"){print $0;}
#}

#find duplicate gene ids
{
	if($4==xx){print $4;}
	else{xx=$4;}
}

#END only gets executed once at the end
END{print "---- The End ----"}