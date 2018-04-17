system("g++", "-std=c++11", "-o", "test", "scan_v8.cpp", "ParseTree3.cpp", "List.cpp", "Symbol.cpp", "Error.cpp", "CodeGen.cpp" );
system("./test TP.txt");

#Checks If there were errors, won't run if there are any
my $filename = 'Error_Free.txt';
$compile_c = 0;
if (open(my $fh, '<:encoding(UTF-8)', $filename)) {
  while (my $row = <$fh>) {
    chomp $row;  
    if ($row eq "Yes"){
    	$compile_c = 1;
    }
  }
}

if ($compile_c eq 1){
	system("gcc CodeGenReg.c -std=c99");
	system("./a.out");
} else{
	print "* Could Not Compile C Code Due To Errors *\n";		
}