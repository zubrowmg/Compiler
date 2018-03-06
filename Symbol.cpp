#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include <iterator>
#include <vector>

#include "ParseTree.h"
#include "List.h"
#include "Symbol.h"
#include "Error.h"

using namespace std;
/*------------------------------------------------------
	Symbol Class
------------------------------------------------------*/
Symbol::Symbol()
{
	size = 0; order_size = 0; current_proc = "";
	pos = NULL; tail = NULL; head = NULL;
	prev_TT_glob = false; prev_TT_proc = false; prev_TT_end = false;
	prev_TT_int = false; prev_TT_flt = false; prev_TT_str = false;
	prev_TT_bool = false; prev_TT_char = false; prev_TT_prog = false;
	prev2_TT_glob = false;
}

void Symbol::clearTC(){ type_check.clear();}

bool Symbol::insertTC(std::string ident, std::string TT){
	bool type_match = true;
	std::string type;
	type = returnValType(ident);
	
	//if (type != "T_ADD" || type != "T_MINUS" || type != "T_AND" || type != "T_OR" || type != "T_NOT" || type != "T_LESSTHAN" || type != "T_LESSTHANEQUAL" || type != "T_GREATERTHAN" || type != "T_GREATERTHANEQUAL" || type != "T_EQUALTO" || type != "T_NOTEQUALTO" || type != "T_ASSIGN" || type != "T_MULT" || type != "T_DIVIDE" ){
	if (TT == "T_NUMBERVAL"){  type = "V_INTEGER"; }
	else if (TT == "T_STRINGVAL"){  type = "V_STRING"; }
	else if (TT == "T_CHARVAL"){  type = "V_CHAR"; }

	if (type_check.size() == 0){
		type_check.push_back(type);
		
	} else { 
		for (int i = 0; i < type_check.size(); i++){
			if ((type_check[i] == "V_INTEGER" && ident == "V_FLOAT") || (type_check[i] == "V_FLOAT" && ident == "V_INTEGER")){
				// Floats and Integers should be able to be compared
			} else if (type_check[i] != type){
				type_match = false; 
			}
		}
		if (type_match){ 
			type_check.push_back(type);
		}
	}
	//}
	return type_match;
}


void Symbol::resetPos(){ pos = head; }

// Finds the type of the input identifier
std::string Symbol::returnValType(std::string ident){
	std::string type; bool type_found = false;

	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == ident){
        	type = (itr->second).str_val;
        	type_found = true;
        }
    }

    if (!type_found){
    	for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				type = pos->returnValType(ident);
				if (type != "NULL"){
					type_found = true;
				}
				break;
			} else {
				pos = pos->getNext();
			}
		}
    }

    if (!type_found){
    	cout << "returnType function failed" << endl;
    }

	return type;
}

void Symbol::init(std::string token, std::string value, symbolNode sym)
{	
	if (token == "T_IDENTIFIER"){
		if (prev_TT_int){
			sym.str_val = "V_INTEGER";
		} else if (prev_TT_flt){
			sym.str_val = "V_FLOAT";
		} else if (prev_TT_str){
			sym.str_val = "V_STRING";
		} else if (prev_TT_bool){
			sym.str_val = "V_BOOL";
		} else if (prev_TT_char){
			sym.str_val = "V_CHAR";
		}

		//If tok was glabol 2 toks ago && value doesn't exist in global && prev tok is a type mark
		if ((prev2_TT_glob && newCheckGlobal(value)) && (prev_TT_int || prev_TT_flt || prev_TT_str || prev_TT_bool || prev_TT_char)){
			

			insertGlobal(value, sym);
		} else if (prev_TT_proc){
			newProc(value);
		} else if (prev_TT_int || prev_TT_flt || prev_TT_str || prev_TT_bool || prev_TT_char){
			insertValue(value, sym);
		} else if (prev_TT_prog){

		} else {
			//Check the scopes
			check(value);
		}
	}

	if (prev_TT_end && token == "T_PROCEDURE"){
		endProc();
	}

	if (token == "T_INTEGER"){prev_TT_int = true;} else {prev_TT_int = false;}
	if (token == "T_FLOAT"){prev_TT_flt = true;} else {prev_TT_flt = false;}
	if (token == "T_STRING"){prev_TT_str = true;} else {prev_TT_str = false;}
	if (token == "T_BOOL"){prev_TT_bool = true;} else {prev_TT_bool = false;}
	if (token == "T_CHAR"){prev_TT_char = true;} else {prev_TT_char = false;}

	if (prev_TT_glob){prev2_TT_glob = true;} else {prev2_TT_glob = false;}

	if (token == "T_GLOBAL"){prev_TT_glob = true;} else {prev_TT_glob = false;}
	if (token == "T_PROGRAM"){prev_TT_prog = true;} else {prev_TT_prog = false;}
	if (token == "T_PROCEDURE"){prev_TT_proc = true;} else {prev_TT_proc = false;}
	if (token == "T_END"){prev_TT_end = true;} else {prev_TT_end = false;}	
}

void Symbol::insertValue(std::string key, symbolNode sym)
{ 
	if (newCheck(key)){
		resetPos();
		for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				pos->insertValue(key, sym);
				break;
			} else {
				pos = pos->getNext();
			}
		}
	}	  
}
void Symbol::insertGlobal(std::string key, symbolNode sym){ 
	if (newCheckGlobal(key)){
		global[key] = sym;
	}
}

bool Symbol::newCheckGlobal(std::string input)
{
	bool checker = true;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == input){
        	error(input, 0);
        	checker = false;
        }
    }
	return checker;
}

void Symbol::nextPos()
{ 
	if (pos->getNext() == tail){
		resetPos();
	} else {
		pos = pos->getNext();
	}
}

void Symbol::newProc(std::string input)
{
	ProcedureNode *temp = new ProcedureNode;
	temp->setName(input); current_proc = input;
	
	if(head == NULL) {
		head = temp;
		tail = temp;
		temp = NULL;

		pos = head;
	}
	else {	
		tail->setNext(temp);
		tail = temp;
		pos = temp;
	}

	//order[order_size] = input;
	order.push_back(input);
	size = size + 1; order_size = order_size + 1;	

}

void Symbol::endProc()
{
	if (current_proc == order[order_size]){
		order.erase(order.begin() + order_size);

		order_size = order_size - 1;
		current_proc = order[order_size];
	}
}
void Symbol::printAll(){
	cout << endl << "========= Symbol Table =========" << endl;
	printGlobal(); printTable();
	cout << "================================" << endl << endl;
}

void Symbol::printGlobal()
{
	std::map <std::string, symbolNode> :: iterator itr;
	std::cout << "Global: "<< '\n';
	for (itr = global.begin(); itr != global.end(); ++itr){
        std::cout  <<  '\t' << itr->first 
              <<  '\t' << (itr->second).type <<  '\t' << (itr->second).str_val << '\n';
    }
}
void Symbol::printTable()
{
	resetPos();
	for (int i = 0; i < size; i++){
		pos->printTable();
		pos = pos->getNext();
	}
}

bool Symbol::procCheck(std::string input)
{
	bool checker = false;
	resetPos();
	for (int i = 0; i < size; i++){
		if (input == pos->getName()){
			checker = true;
			break;
		} else {
			pos = pos->getNext();
		}
	}
	return checker;
}

bool Symbol::newCheck(std::string input)
{
	bool checker = true;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == input){
        	error(input, 0);
        	checker = false;
        }
    }

    if (checker){
	    resetPos();
		for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				checker = pos->newCheck(input);
				break;
			} else {
				pos = pos->getNext();
			}
		}
	}
	return checker;
}

//Checks to see if identifier is declared
bool Symbol::check(std::string input)
{
	bool declared = false;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){  	//Global
        if (itr->first == input){
        	declared = true;
        }
    }

    if (!declared){
	    resetPos();
		for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){				//Current scope
				declared = pos->check(input);
				break;
			} else {
				pos = pos->getNext();
			}
		}
	}

	if(!declared){
		declared = procCheck(input);							//Checks procedure names
	}


	if (!declared){
		error(input, 1);
	}

	return declared;
}

void Symbol::error(std::string input, int i){
	if (i == 0){
		std::cout << "Error Redeclared Variable: " << input << '\n';
	} else if (i == 1){
		std::cout << "Error Variable Not Declared: " << input << '\n';
	}
}

/*------------------------------------------------------
	Symbol Node Class
------------------------------------------------------*/
ProcedureNode::ProcedureNode()
{
	next = NULL;
}

void ProcedureNode::insertValue(std::string key, symbolNode sym){ table[key] = sym; }
void ProcedureNode::setName(std::string input){ name = input; }
void ProcedureNode::setNext(ProcedureNode *input){ next = input; }
std::string ProcedureNode::getName(){ return name; }
ProcedureNode *ProcedureNode::getNext(){ return next; }

std::string ProcedureNode::returnValType(std::string ident){
	std::string type = "NULL";

	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->first == ident){
        	type = (itr->second).str_val;
        }
    }

	return type;
}

void ProcedureNode::printTable()
{ 
	std::map <std::string, symbolNode> :: iterator itr;

	std::cout << "Name: " << name << '\n'<< '\n';
	for (itr = table.begin(); itr != table.end(); ++itr){
        std::cout  <<  '\t' << itr->first 
              <<  '\t' << (itr->second).type <<  '\t' << (itr->second).str_val << '\n';
    }
}

bool ProcedureNode::newCheck(std::string input){
	bool checker = true;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->first == input){
        	error(input);
        	checker = false;
        }
    }
	return checker;
}

bool ProcedureNode::check(std::string input){
	bool declared = false;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->first == input){
        	declared = true;
        }
    }
	return declared;
}

void ProcedureNode::error(std::string input){
	std::cout << "Error Redeclared Variable: " << input << '\n';
}



