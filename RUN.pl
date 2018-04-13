system("g++", "-std=c++11", "-o", "test", "scan_v8.cpp", "ParseTree3.cpp", "List.cpp", "Symbol.cpp", "Error.cpp", "CodeGen.cpp" );
system("./test text.txt");

#add runtime shit here