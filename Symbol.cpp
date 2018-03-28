#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include <iterator>
#include <vector>
#include <string>

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
	prev2_TT_glob = false; prev_TT_IDENT = false; prev_TT_IDENT = false; prev_TT_LB = false;
	prev_TT_SEMICOLON = false; prev2_TT_LB = false;
	last_ident = "NULL";


}

int Symbol::setMMIndex(std::string ident, int index)
{
//cout << index << endl;
	int new_index = index;
	bool done = false;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == ident){

        	(itr->second).MM_Index = index;

//cout << new_index << endl;

        	if ((itr->second).is_array == true){
        		new_index = index + (itr->second).array_size;
   	
        	} else {
        		new_index = index + 1;   			
        	}
        	done = true; 
        }
    }
    if (!done){
    	for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				new_index = pos->setMMIndex(ident, index);
				break;
			} else {
				pos = pos->getNext();
			}
		}
    }
//cout << new_index << endl;
	return new_index;
}

int Symbol::getMMIndex(std::string ident)
{	
	int index;
	bool done = false;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == ident){
        	index = (itr->second).MM_Index;       	
        	done = true; 
        }
    }
    if (!done){
    	for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				index = pos->getMMIndex(ident);
				break;
			} else {
				pos = pos->getNext();
			}
		}
    }
	return index;
}

void Symbol::clearTC_AS(){ type_check_AS.clear();}

bool Symbol::insertTC_AS(std::string ident, std::string TT){
	bool type_match = true; symbolNode sym; symbolNode temp; TCNode T_C; T_C.ident = ident; 
	std::string type;
//cout << ident << endl;
	sym = returnValType(ident);
//cout << ident << endl;
	T_C.type = sym.str_val;

//cout<<endl;
//cout << type << endl;	
	//if (type != "T_ADD" || type != "T_MINUS" || type != "T_AND" || type != "T_OR" || type != "T_NOT" || type != "T_LESSTHAN" || type != "T_LESSTHANEQUAL" || type != "T_GREATERTHAN" || type != "T_GREATERTHANEQUAL" || type != "T_EQUALTO" || type != "T_NOTEQUALTO" || type != "T_ASSIGN" || type != "T_MULT" || type != "T_DIVIDE" ){
	if (TT == "T_NUMBERVAL"){  T_C.type = "V_INTEGER"; }
	else if (TT == "T_STRINGVAL"){  T_C.type = "V_STRING"; }
	else if (TT == "T_CHARVAL"){  T_C.type = "V_CHAR"; }

	if (type_check_AS.size() == 0){
		type_check_AS.push_back(T_C);
		
	} else { 
		
		if (type_match){ 
			type_check_AS.push_back(T_C);
		}
	}
	//}
	return type_match;
}

void Symbol::clearTC(){ type_check.clear();}

bool Symbol::insertTC(std::string ident, std::string TT){
	bool type_match = true; symbolNode sym; symbolNode temp; TCNode T_C; T_C.ident = ident; 
	std::string type;
	sym = returnValType(ident);
	T_C.type = sym.str_val;  T_C.array_single_access = false;
//cout << type << endl;	
	//if (type != "T_ADD" || type != "T_MINUS" || type != "T_AND" || type != "T_OR" || type != "T_NOT" || type != "T_LESSTHAN" || type != "T_LESSTHANEQUAL" || type != "T_GREATERTHAN" || type != "T_GREATERTHANEQUAL" || type != "T_EQUALTO" || type != "T_NOTEQUALTO" || type != "T_ASSIGN" || type != "T_MULT" || type != "T_DIVIDE" ){
	if (TT == "T_NUMBERVAL"){  T_C.type = "V_INTEGER"; }
	else if (TT == "T_STRINGVAL"){  T_C.type = "V_STRING"; }
	else if (TT == "T_CHARVAL"){  T_C.type = "V_CHAR"; }


	if (type_check.size() == 0){
		type_check.push_back(T_C);
		
	} else { 
		//type_match = MC(ident, TT);
		if (type_match){ 
			type_check.push_back(T_C);
		}
	}
	
	return type_match;
}

/*bool Symbol::MC(std::string ident, std::string TT){
	bool type_match = true; TCNode T_C; T_C.ident = ident; symbolNode sym; symbolNode temp;
	sym = returnValType(ident); T_C.type = sym.str_val;
	for (int i = 0; i < type_check.size(); i++){
		if ((type_check[i].type == "V_INTEGER" && ident == "V_FLOAT") || (type_check[i].type == "V_FLOAT" && ident == "V_INTEGER")){
			// Floats and Integers should be able to be compared
		} else if (type_check[i].type != T_C.type){
			type_match = false; 
		}

		temp = returnValType(type_check[i].ident); 	
		

		if (temp.is_array != sym.is_array){ 		
			type_match = false;
		} 

		if (temp.array_size != sym.array_size){ 		
			type_match = false;
		} 
	}
	return type_match;
}*/

bool Symbol::MC(){
	bool type_match = true; bool flag = false;
	TCNode head;

	symbolNode h_sym; symbolNode temp_sym;
	
	for (int j = 0; j < type_check.size(); j++){
		head = type_check[j];
		h_sym = returnValType(head.ident);
//cout << "EX" << " " << head.ident << " " << head.type << " " << head.array_single_access << endl;
		for (int i = 0; i < type_check.size(); i++){
			if ((type_check[i].type == "V_INTEGER" && head.type == "V_FLOAT") || (type_check[i].type == "V_FLOAT" && head.type == "V_INTEGER")){
				// Floats and Integers should be able to be compared
			} else if (type_check[i].type != head.type){
				type_match = false; 
			}

			temp_sym = returnValType(type_check[i].ident); 	
			
			if ((!h_sym.is_array && temp_sym.is_array && !type_check[i].array_single_access)
					|| (h_sym.is_array && head.array_single_access && temp_sym.is_array && !type_check[i].array_single_access)
					|| (h_sym.is_array && !head.array_single_access && temp_sym.is_array && type_check[i].array_single_access)
					|| (!h_sym.is_array && head.array_single_access) ){
				
				type_match = false; flag = true;
			}

			if (flag && (h_sym.is_array != temp_sym.is_array)){
				type_match = false; 
			}

			if (h_sym.is_array && head.array_single_access && temp_sym.is_array && type_check[i].array_single_access && (temp_sym.array_size != h_sym.array_size)){ 		
				type_match = false; 
			}

			flag = false;
		}
	}
	return type_match;
}

bool Symbol::MC_AS(){
	bool type_match = true; bool flag = false;
	TCNode head;

	symbolNode h_sym; symbolNode temp_sym;

	for (int j = 0; j < type_check_AS.size(); j++){
		head = type_check_AS[j];
		h_sym = returnValType(head.ident);
//cout << head.ident << " " << head.type << " " << head.array_single_access << endl;
		for (int i = 0; i < type_check_AS.size(); i++){
			if ((type_check_AS[i].type == "V_INTEGER" && head.type == "V_FLOAT") || (type_check_AS[i].type == "V_FLOAT" && head.type == "V_INTEGER")){
				// Floats and Integers should be able to be compared
			} else if (type_check_AS[i].type != head.type){
				type_match = false; 
			}

			temp_sym = returnValType(type_check_AS[i].ident); 	
			
			if ((!h_sym.is_array && temp_sym.is_array && !type_check_AS[i].array_single_access)
					|| (h_sym.is_array && head.array_single_access && temp_sym.is_array && !type_check_AS[i].array_single_access)
					|| (h_sym.is_array && !head.array_single_access && temp_sym.is_array && type_check_AS[i].array_single_access)
					|| (!h_sym.is_array && head.array_single_access) ){
				
				type_match = false; flag = true;  
			} 

			if (flag && (h_sym.is_array != temp_sym.is_array)){
				type_match = false; 
			}

			if (h_sym.is_array && head.array_single_access && temp_sym.is_array && type_check_AS[i].array_single_access && (temp_sym.array_size != h_sym.array_size)){ 		
				type_match = false; 
			}

			flag = false;
		}
	}
	return type_match;
}

bool Symbol::modifyTC(std::string ident, std::string TT){ 
	bool modified = false; bool type_match = true;
	for (int i = 0; i < type_check.size(); i++){
		if (last_ident == type_check[i].ident){
			type_check[i].array_single_access = true; 
		}
	}

	return type_match;
}

bool Symbol::modifyTC_AS(std::string ident, std::string TT){ 
	bool modified = false; bool type_match = true;
	for (int i = 0; i < type_check_AS.size(); i++){
		if (last_ident == type_check_AS[i].ident){
			type_check_AS[i].array_single_access = true; 
		}
	}

	return type_match;
}

void Symbol::modify(std::string ident){
	bool done = false;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == ident){
        	(itr->second).is_array = true;
        	done = true; 
        }
    }
    if (!done){
    	for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				pos->modify(ident);
				break;
			} else {
				pos = pos->getNext();
			}
		}
    }
}





void Symbol::modify(std::string ident, std::string num, char c){ 
	bool done = false;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == ident){
        	if (c == 'L'){ 
        		(itr->second).array_left = stoi(num);
        	} else if (c == 'R'){
        		(itr->second).array_right = stoi(num);
        		(itr->second).array_size = abs((itr->second).array_left - (itr->second).array_right); 	
        	}
        	done = true; 
        }
    }
    if (!done){
    	for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				pos->modify(ident, num, c);
				break;
			} else {
				pos = pos->getNext();
			}
		}
    }
}

void Symbol::resetPos(){ pos = head; }

// Finds the type of the input identifier
symbolNode Symbol::returnValType(std::string ident){			
	symbolNode sym; bool type_found = false;

	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == ident){
        	sym = (itr->second);
        	type_found = true;
        }
    }

    if (!type_found){ 
    	resetPos();	
    	for (int i = 0; i < size; i++){ //
    		
			if (current_proc == pos->getName()){ 	//LLLLL
				sym = pos->returnValType(ident);		
				if (sym.type != "NULL"){
					type_found = true;
				}

				break;
			} else { 

				pos = pos->getNext();
			}
		} 
    }

    if (!type_found){
//    	cout << "returnType function failed" << endl;
    }

	return sym;
}

void Symbol::init(std::string token, std::string value, symbolNode sym)
{	
	//if (prev2_TT_LB ){ cout << token << endl;}
//cout << value << endl;
	if (prev_TT_SEMICOLON && token == "T_NUMBERVAL"){ 
		modify(last_ident, value, 'R'); 
	}

	if (prev2_TT_LB && token == "T_COLON"){ 
		prev_TT_SEMICOLON = true;
	} else {prev_TT_SEMICOLON = false;} 

	if (prev_TT_LB && token == "T_NUMBERVAL"){ 
		modify(last_ident, value, 'L'); prev2_TT_LB = true; 	 		
	} else {prev2_TT_LB = false;}

	if ((prev2_TT_int || prev2_TT_flt || prev2_TT_str || prev2_TT_bool || prev2_TT_char) && prev_TT_IDENT && token == "T_LBRACKET"){ 						// NNNNNNN
		modify(last_ident); prev_TT_LB = true; //cout << "HI";
	} else {prev_TT_LB = false; }
//last_ident = "NULL";

	if (token == "T_IDENTIFIER"){
		last_ident = value;


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
		if ((prev2_TT_glob && newCheckGlobal(value, sym)) && (prev_TT_int || prev_TT_flt || prev_TT_str || prev_TT_bool || prev_TT_char)){
			insertGlobal(value, sym);
		} else if (prev_TT_proc){
			newProc(value);
		} else if (prev_TT_int || prev_TT_flt || prev_TT_str || prev_TT_bool || prev_TT_char){
			insertValue(value, sym);
		} else if (prev_TT_prog){

		} else {
			//Check the scopes
			check(value, sym);
		}
		prev_TT_IDENT = true;
	} else {prev_TT_IDENT = false;  }

	

	if (prev_TT_end && token == "T_PROCEDURE"){
		endProc();
	}


	if (prev_TT_int){prev2_TT_int = true;} else {prev2_TT_int = false;}
	if (prev_TT_flt){prev2_TT_flt = true;} else {prev2_TT_flt = false;}
	if (prev_TT_str){prev2_TT_str = true;} else {prev2_TT_str = false;}
	if (prev_TT_bool){prev2_TT_bool = true;} else {prev2_TT_bool = false;}
	if (prev_TT_char){prev2_TT_char = true;} else {prev2_TT_char = false;}

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

	//cout << "END" << endl;
}

void Symbol::insertValue(std::string key, symbolNode sym)
{ 
	if (newCheck(key, sym)){
		resetPos();
		for (int i = 0; i < size; i++){
			if (current_proc == pos->getName()){
				sym.proc = current_proc;
				pos->insertValue(key, sym);
				break;
			} else {
				pos = pos->getNext();
			}
		}
	}	  
}
void Symbol::insertGlobal(std::string key, symbolNode sym){ 
	if (newCheckGlobal(key, sym)){
		sym.proc = "GLOBAL";
		global[key] = sym;
	}
}

bool Symbol::newCheckGlobal(std::string input, symbolNode sym)
{ 	

	bool checker = true;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == input){										//KKKKK
        	sym_error_handler.error(sym.line_num, 0, input);
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
              <<  '\t' << (itr->second).type <<  '\t' << (itr->second).str_val << ' '<< '\t' ;
      	if ((itr->second).is_array){
      		cout << "Is Array" << ' ' << '\t' << (itr->second).array_size << endl;
      	} else {
    		cout << "Not Array" << endl;
    	}
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

bool Symbol::newCheck(std::string input, symbolNode sym)
{
	bool checker = true;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = global.begin(); itr != global.end(); ++itr){
        if (itr->first == input){
        	sym_error_handler.error(sym.line_num, 0, input);
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
bool Symbol::check(std::string input, symbolNode sym)
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
		sym_error_handler.error(sym.line_num, 1, input);
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

symbolNode ProcedureNode::returnValType(std::string ident){
	symbolNode sym;
	sym.type = "NULL";

	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->first == ident){
        	sym = (itr->second);
        }
    }

	return sym;
}

void ProcedureNode::modify(std::string ident){
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->first == ident){
        	(itr->second).is_array = true; 
        }
    }
}

void ProcedureNode::modify(std::string ident, std::string num, char c){
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->first == ident){
        	if (c == 'L'){
        		(itr->second).array_left = stoi(num);
        	} else if (c == 'R'){
        		(itr->second).array_right = stoi(num);
        		(itr->second).array_size = abs((itr->second).array_left - (itr->second).array_right);
        	}
        }
    }
}

int ProcedureNode::setMMIndex(std::string ident, int index){
	int new_index = index;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->first == ident){
        	(itr->second).MM_Index = index;
        	if ((itr->second).is_array == true){
        		new_index = index + (itr->second).array_size;
        	} else {
        		new_index = index + 1;
        	}
        }
    }    
    return new_index;
}

int ProcedureNode::getMMIndex(std::string ident){
	int index;
	std::map <std::string, symbolNode> :: iterator itr;
	for (itr = table.begin(); itr != table.end(); ++itr){
        if (itr->first == ident){
        	index = (itr->second).MM_Index;
        }
    }
    return index;
}

void ProcedureNode::printTable()
{ 
	std::map <std::string, symbolNode> :: iterator itr;

	std::cout << "Name: " << name << '\n'<< '\n';
	for (itr = table.begin(); itr != table.end(); ++itr){
        std::cout  <<  '\t' << itr->first 
              <<  '\t' << (itr->second).type <<  '\t' << (itr->second).str_val << ' '  <<  '\t';
      	if ((itr->second).is_array){
      		cout << "Is Array" << ' ' << '\t' << (itr->second).array_size << endl;
      	} else {
    		cout << "Not Array" << endl;
    	}
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



