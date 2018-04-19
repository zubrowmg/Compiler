#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "Error.h"
#include "ParseTree.h"

using namespace std;

Error::Error(){
	total_errors = 0;
}

void Error::incrementError(){
	total_errors = total_errors + 1;
}

int Error::getTotalErrors(){
	return total_errors;
}

void Error::error(int line, int type){
	total_errors = total_errors + 1;
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
		cout << "putChar only accepts char indentifiers" << endl;
	} else if (type == 14){
		cout << "putChar only accepts chars" << endl;
	} else if (type == 15){
		cout << "putFloat does not accept integers" << endl;
	} else if (type == 16){
		cout << "putFloat only accepts float indentifiers" << endl;
	} else if (type == 17){
		cout << "putFloat only accepts floats" << endl;
	} else if (type == 18){
		cout << "putBool only accepts bool indentifiers" << endl;
	} else if (type == 19){
		cout << "putBool only accepts bools" << endl;
	} else if (type == 20){
		cout << "putString only accepts string indentifiers" << endl;
	} else if (type == 21){
		cout << "putString only accepts strings" << endl;
	}

	else if (type == 22){
		cout << "Array size difference" << endl;
	}

	else if (type == 23){
		cout << "getInteger only accepts integer indentifiers" << endl;
	} else if (type == 24){
		cout << "get function only accepts indentifiers" << endl;
	} else if (type == 25){
		cout << "getFloat  only accepts float indentifiers" << endl;
	} else if (type == 26){
		cout << "getChar only accepts char indentifiers" << endl;
	} else if (type == 27){
		cout << "getBool only accepts bool indentifiers" << endl;
	} else if (type == 28){
		cout << "getString only accepts string indentifiers" << endl;
	} else if (type == 28){
		cout << "Too Many */" << endl;
	}


}

void Error::error(int line, int type, std::string var){
	total_errors = total_errors + 1;
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

void Error::error(int line, int type, std::string TC_1, std::string TC_2){
	total_errors = total_errors + 1;
	cout << "[" << line << "]" << "\t" << "Error:  ";

	if (type == 0){
		cout << "Indentifiers Are Different Types - " << TC_1 << ", " << TC_2 << endl;
	} else if (type == 1){
		cout << "Array Sizes Don't Match - " << TC_1 << ", " << TC_2 << endl;
	} else if (type == 2){
		cout << "Array Can't Be Compared With Non Array - " << TC_1 << ", " << TC_2 << endl;
	} else if (type == 3){
		cout << "Array Access Can't Be Compared With Array - " << TC_1 << ", " << TC_2 << endl;
	} else if (type == 4){
		cout << "Non Array Can't Be Compared With Array - " << TC_1 << ", " << TC_2 << endl;
	} else if (type == 5){
		cout << "String Value Can't Be Compared With A Non String Indentifier - " << TC_1 << ", " << "\"" << TC_2 << "\"" << endl;
	} else if (type == 6){
		cout << "String Value Can't Be Compared With A Non String Indentifier - " << "\"" << TC_1 << "\"" << ", " << TC_2 << endl;
	} else if (type == 7){
		cout << "Char Value Can't Be Compared With A Non Char Indentifier - " << TC_1 << ", " << "\'" << TC_2 << "\'" << endl;
	} else if (type == 8){
		cout << "Char Value Can't Be Compared With A Non Char Indentifier - " << "\'" << TC_1 << "\'"  << ", " << TC_2 << endl;
	} else if (type == 9){
		cout << "String Value Can't Be Compared With A Char Value - " << "\"" << TC_1 << "\"" << ", "  << "\'"  << TC_2 << "\'"  << endl;
	} else if (type == 10){
		cout << "String Value Can't Be Compared With A Number Value - " << "\"" << TC_1 << "\"" << ", " << TC_2 << endl;
	} else if (type == 11){
		cout << "String Value Can't Be Compared With A Bool Value - " << "\"" << TC_1 << "\"" << ", " << TC_2 << endl;
	} else if (type == 12){
		cout << "Bool Value Can't Be Compared With A Full String Indentifier - " <<  TC_1  << ", " << TC_2 << endl;
	} else if (type == 13){
		cout << "Bool Value Can't Be Compared With A String Value - " << TC_1 << ", " << "\"" << TC_2 << "\"" << endl;
	} else if (type == 14){
		cout << "Bool Value Can't Be Compared With A Char Value - " << TC_1 << ", " << "\'" << TC_2 << "\'" << endl;
	} else if (type == 15){
		cout << "Bool Value Can't Be Compared With A Number Value - " << TC_1 << ", " << TC_2 << endl;
	} else if (type == 16){
		cout << "Char Value Can't Be Compared With A Full String Indentifier - " << "\'" << TC_1 << "\'" << ", " << TC_2 << endl;
	} else if (type == 17){
		cout << "Char Value Can't Be Compared With A String Value - " << "\'" << TC_1 << "\'" << ", " << "\"" << TC_2 << "\"" << endl;
	} else if (type == 18){
		cout << "Char Value Can't Be Compared With A Number Value - " << "\'" << TC_1 << "\'" << ", " << TC_2 << endl;
	} else if (type == 19){
		cout << "Char Value Can't Be Compared With A Bool Value - " << "\'" << TC_1 << "\'" << ", " << TC_2 << endl;
	} else if (type == 20){
		cout << "Number Value Can't Be Compared With A Full String Indentifier - " <<  TC_1  << ", " << TC_2 << endl;
	} else if (type == 21){
		cout << "Number Value Can't Be Compared With A String Value - " << TC_1  << ", " << "\"" << TC_2 << "\"" << endl;
	} else if (type == 22){
		cout << "Number Value Can't Be Compared With A Char Value - " << TC_1  << ", " << "\'" << TC_2 << "\'" << endl;
	} else if (type == 23){
		cout << "Number Value Can't Be Compared With A Bool Value - " << TC_1  << ", " << TC_2 << endl;
	}

	else if (type == 24){
		cout << "Boolean Expressions Need to Have A Destination Type of Int or Bool - " << TC_1  << ", " << TC_2 << endl;
	}

	 else if (type == 25){
	 	cout << "Single Entity Indentifiers Can't Be Compared With A Full String Indentifier - " << TC_1  << ", " << TC_2 << endl;
	} else if (type == 26){
		cout << "Destination Needs to Be an Indentifier - " << TC_1 << endl;
	}


	else if (type == 27){
	 	cout << "A Destination of Type String Can't Be Equal A Single Entity Indentifier - " << TC_1  << ", " << TC_2 << endl;
	} else if (type == 28){
	 	cout << " A Single Entity Destination Can't Be Equal an Indentifier of Type String - " << TC_1  << ", " << TC_2 << endl;
	} else if (type == 29){
		cout << "A Destination of Type String Can't Be Equal to A Single Entity - " << TC_1 << ", "  << TC_2  << endl;
	} else if (type == 30){
		cout << "A Destination of Type Char Can't Be Equal to A Full String - " << TC_1 << ", " << "\"" << TC_2 << "\"" << endl;
	} else if (type == 31){
		cout << "A Destination of Type Char Can't Be Equal to A Bool - " << TC_1 << ", " << TC_2 << endl;
	} else if (type == 32){
		cout << "A Destination of Type Bool Can't Be Equal to A Char - " << TC_1 << ", " << TC_2 << endl;
	} else if (type == 33){

	} else if (type == 34){

	} else if (type == 35){

	} else if (type == 36){

	} else if (type == 37){

	} else if (type == 38){

	} else if (type == 39){

	} else if (type == 40){

	}

}