#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "Error.h"

using namespace std;

void Error::error(int line, int type){

	cout << "[" << line << "]" << "\t" << "Error:  ";

/*------------------------------------------------------
	Scanner Errors
------------------------------------------------------*/	
	if (type == 1){
		cout << "Illegal character" << line << endl;
	} else if (type == 2){
		cout << "Char should only be 1 character in length"  << endl;
	} else if (type == 3){
		cout << "Char close \' is missing or string close \" is missing" << endl;
	} else if (type == 4){
		cout << "Illegal char used for CHAR type or STRING type" << endl;
	} else if (type == 5){
		cout << "Illegal char used for IDENTIFIER type" << endl;
	} else if (type == 6){
		cout << "Missing IDENTIFIER or PERIOD after PROGRAM" << endl;
	} else if (type == 7){
		cout << "Missing IDENTIFIER or SEMICOLON after PROCEDURE" << endl;
	} 


}

void Error::error(int line, int type, std::string var){

	cout << "[" << line << "]" << "\t" << "Error:  ";

/*------------------------------------------------------
	Symbol Errors
------------------------------------------------------*/
	if (type == 0){
		cout << "Redeclared Variable -  " << var << endl;
	} else if (type == 1){
		cout << "Variable Not Declared -  " << var << endl;
	}

}