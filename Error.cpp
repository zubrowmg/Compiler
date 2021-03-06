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
/*------------------------------------------------------
	Type Check Errors
------------------------------------------------------*/
	else if (type == 8){
		cout << "Expression Type Mismatch" << endl;
	} else if (type == 9){
		cout << "Assignment Type Mismatch" << endl;
	} 
	// Type Check Errors in Code Gen
 	else if (type == 10){
		cout << "putInteger does not accept floats" << endl;
	} else if (type == 11){
		cout << "putInteger only accepts integer indentifiers" << endl;
	} else if (type == 12){
		cout << "putInteger only accepts integers" << endl;
	} else if (type == 13){
		cout << "putInteger only accepts char indentifiers" << endl;
	} else if (type == 14){
		cout << "putInteger only accepts chars" << endl;
	} else if (type == 15){
		cout << "putInteger does not accept integers" << endl;
	} else if (type == 16){
		cout << "putInteger only accepts float indentifiers" << endl;
	} else if (type == 17){
		cout << "putInteger only accepts floats" << endl;
	} else if (type == 18){
		cout << "putInteger only accepts bool indentifiers" << endl;
	} else if (type == 19){
		cout << "putInteger only accepts bools" << endl;
	} else if (type == 20){
		cout << "putInteger only accepts string indentifiers" << endl;
	} else if (type == 21){
		cout << "putInteger only accepts strings" << endl;
	}

	else if (type == 22){
		cout << "Array size difference" << endl;
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
/*------------------------------------------------------
	Parser Errors
------------------------------------------------------*/
	else if (type == 2){
		cout << "Incorrect Symantix -  " << var << endl;
	}
}