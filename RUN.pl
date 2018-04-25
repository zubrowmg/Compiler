#print "$ARGV[0] \n";
system("g++", "-std=c++11", "-o", "test", "C++_files/scan_v8.cpp", "C++_files/ParseTree3.cpp", "C++_files/List.cpp", "C++_files/Symbol.cpp", "C++_files/Error.cpp", "C++_files/CodeGen.cpp" );
system("./test $ARGV[0]");

#Checks If there were errors, won't run if there are any
my $filename = 'C++_files/Error_Free.txt';
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
	system("gcc CodeGen_files/CodeGenReg.c -std=c99");
	system("./a.out");
} else{
	print "* Could Not Compile C Code Due To Errors *\n";		
}