#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "Error.h"

using namespace std;

void Error::error(int line, int type){

	cout << "[" << line << "]" << "\t";

/*------------------------------------------------------
	Scanner Errors
------------------------------------------------------*/	
	if (type == 1){
		cout << "Illegal character" << line << endl;
	} else if (type == 2){
		cout << "Char should only be 1 character in length"  << line << endl;
	} else if (type == 3){
		cout << "Char close \' is missing or string close \" is missing" << line << endl;
	} else if (type == 4){
		cout << "Illegal char used for CHAR type or STRING type" << line << endl;
	} else if (type == 5){
		cout << "Illegal char used for IDENTIFIER type" << line << endl;
	} else if (type == 6){
		cout << "Missing IDENTIFIER or PERIOD after PROGRAM" << line << endl;
	}  else if (type == 7){
		cout << "Missing IDENTIFIER or SEMICOLON after PROCEDURE" << line << endl;
	}

}