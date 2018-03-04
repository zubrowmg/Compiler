#include <iostream>
#include <iomanip>
#include <map>
#include <iterator>
#include <vector>

#include "ParseTree.h"
#include "List.h"
#include "Symbol.h"
#include "Error.h"

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


void Symbol::resetPos(){ pos = head; }

void Symbol::init(std::string token, std::string value)
{	
	if (token == "T_IDENTIFIER"){
		
		//If tok was glabol 2 toks ago && value doesn't exist in global && prev tok is a type mark
		if ((prev2_TT_glob && newCheckGlobal(value)) && (prev_TT_int || prev_TT_flt || prev_TT_str || prev_TT_bool || prev_TT_char)){
			insertGlobal(value);
		} else if (prev_TT_proc){
			newProc(value);
		} else if (prev_TT_int || prev_TT_flt || prev_TT_str || prev_TT_bool || prev_TT_char){
			insertValue(value);
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

void Symbol::insertValue(std::string key)
{ 
	if (newCheck(key)){
		resetPos();
		for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				pos->insertValue(key);
				break;
			} else {
				pos = pos->getNext();
			}
		}
	}	  
}
void Symbol::insertGlobal(std::string key){ 
	if (newCheckGlobal(key)){
		global[key] = key;
	}
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
void Symbol::printAll(){printGlobal(); printTable();}

void Symbol::printGlobal()
{
	std::map <std::string, std::string> :: iterator itr;
	std::cout << "Global: "<< '\n';
	for (itr = global.begin(); itr != global.end(); ++itr){
        std::cout  <<  '\t' << itr->first 
              <<  '\t' << itr->second << '\n';
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

bool Symbol::newCheckGlobal(std::string input)
{
	bool checker = true;
	std::map <std::string, std::string> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->second == input){
        	error(input, 0);
        	checker = false;
        }
    }
	return checker;
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
	std::map <std::string, std::string> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->second == input){
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
	std::map <std::string, std::string> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){  	//Global
        if (itr->second == input){
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

void ProcedureNode::insertValue(std::string key){ table[key] = key; }
void ProcedureNode::setName(std::string input){ name = input; }
void ProcedureNode::setNext(ProcedureNode *input){ next = input; }
std::string ProcedureNode::getName(){ return name; }
ProcedureNode *ProcedureNode::getNext(){ return next; }

void ProcedureNode::printTable()
{ 
	std::map <std::string, std::string> :: iterator itr;

	std::cout << "Name: " << name << '\n'<< '\n';
	for (itr = table.begin(); itr != table.end(); ++itr){
        std::cout  <<  '\t' << itr->first 
              <<  '\t' << itr->second << '\n';
    }
}

bool ProcedureNode::newCheck(std::string input){
	bool checker = true;
	std::map <std::string, std::string> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->second == input){
        	error(input);
        	checker = false;
        }
    }
	return checker;
}

bool ProcedureNode::check(std::string input){
	bool declared = false;
	std::map <std::string, std::string> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->second == input){
        	declared = true;
        }
    }
	return declared;
}

void ProcedureNode::error(std::string input){
	std::cout << "Error Redeclared Variable: " << input << '\n';
}



