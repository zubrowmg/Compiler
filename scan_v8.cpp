#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector> 
#include <stdio.h>
#include <ctype.h>

#include "ParseTree.h"
#include "List.h"
#include "Symbol.h"
#include "Error.h"


using namespace std;

Error error_handler;
Symbol sym;

/*---------------------------------------------  
	Recieve an input token and clear all values 
---------------------------------------------*/
tokens clear_token(tokens token){
	for (int i = 0; i < 256; i++){
		token.stringValue[i] = '\0';
	}
	for (int i = 0; i < (token.type).length(); i++){
		token.type[i] = '\0';
	}
	return token;
}

/*-------------------------------------------------------------------------------------------------  
	Return str length by checking every char in the array, if it's NULL or a space it doesn't count 
-------------------------------------------------------------------------------------------------*/
int str_length(char str[256]){
	int length = 0;
	for (int i = 0; i < 256; i++){ 
		if (str[i] == '\0' || str[i] == ' '){			
		} else {
			length++;
		}
	}
	return length;
}


/*------------------------------------------------------------------------------------------------------------------------  
	Recieves a token and a str (str is a reserved word), compares the two to determine if the token is reserved word token  
------------------------------------------------------------------------------------------------------------------------*/
bool reserved_word_checker(tokens token, string str){
	bool match = false; int length = 0;
	length = str_length(token.stringValue);
	if (str.length() != length){
		match = false;
		return match;
	}

	for (int i = 0; i < str.length(); i++){ 
		if (str[i] == (token.stringValue)[i]){
			match = true;
		} else {
			match = false;
			return match;
		}
	}
	return match;
}

/*--------------------------------  
	Checks if the char is a letter  
--------------------------------*/
int is_letter(char c){
	if (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' || c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x' || c == 'y' || c == 'z'){
		return 1;
	} else {
		return 0;
	}
}

/*--------------------------------  
	Checks if the char is a number  8
--------------------------------*/
int is_num(char c){
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'){
		return 1;
	} else {
		return 0;
	}
}

int symbol_gen(list scan_list){
	list global_list; tokens temp; bool symbol_table_done = false;
	tokens hold;
	

	
	scan_list.reset_pos();

	while (!symbol_table_done){
		temp = scan_list.get_one();
		hold = temp;

		if (temp.type == "T_ENDFILE"){
			symbol_table_done = true;
		

		} else if (temp.type == "T_PROGRAM"){ 			
			temp = scan_list.get_one();
			if (temp.type == "T_IDENTIFIER"){
				temp.type = "T_PROGRAM";
				global_list.createnode(temp);
			} else if (temp.type == "T_PERIOD"){

			} else {
				error_handler.error(temp.line, 6);
			}


		} else if (temp.type == "T_PROCEDURE"){
			temp = scan_list.get_one();
			if (temp.type == "T_IDENTIFIER"){
				temp.type = "T_PROCEDURE";
				global_list.createnode(temp);
			} else if (temp.type == "T_SEMICOLON"){

			} else {
				error_handler.error(temp.line, 7);
			}
		} 

		clear_token(temp); clear_token(hold);		
	}
	
	//global_list.display();

	return 0;
}

int scan_init(){

	return 0;
}

/*---------------  
	Scan function 
---------------*/
list scan(char *argv[]){
	list token_list; tokens current_token; tokens temp;

	int line_counter = 0, peeker = 0, num_of_comments = 0, char_length = 0;
	bool contains_num = false, contains_letter = false, grab_prev_c = false, nest_comment = false;
	bool number_checker = false, skip = false, ident_checker = false, record_token = false, leave_while = false; 
	
	int i = 0; char c;
	ifstream inFile; 

	inFile.open(argv[1]);
	if (!inFile) {
 	   	cout << "Unable to open file txt" << endl;
    	exit(1);   											// call system to stop
	}


	while (inFile.get(c)){

		

/*---------------------------------------------------------  
	There are times where we need to reread a prevoius char
---------------------------------------------------------*/
		if (grab_prev_c){
			inFile.seekg(-2, inFile.cur);  		// -2 because we want to read the previous char, -1 will move the position to the current char again 
			if (inFile.good()) {
			  	inFile.get(c);
			}
			grab_prev_c = false;
		}

		c = tolower(c);	

/*---------------------  
	Number Token Section 
-----------------------*/
		if (is_num(c) || c == '.'){				//Number tokens can start with either a number or a period							
			
			/*-----------------------------------------------------  
				Checks the first input to see if it is a num or a . 
			-----------------------------------------------------*/
			if (is_num(c)){
				current_token.stringValue[i] = c;
				i++;
				number_checker = true;
			} else {
				current_token.stringValue[i] = c;
				inFile.get(c); c = tolower(c);
				i++;
				if (is_num(c)){										//If the next char is a num
					current_token.stringValue[i] = c;
					i++;
					number_checker = true;
				} else {
					number_checker = false;
					current_token.type = "T_PERIOD"; current_token.line = line_counter; token_list.createnode(current_token);
					current_token = clear_token(current_token);
				}

			}
			
			/*---------------------------------------------  
				While loop will get the entire number token 
			---------------------------------------------*/
			while (number_checker){
				inFile.get(c); c = tolower(c);
				if (is_num(c)){
					current_token.stringValue[i] = c; 				
				} else if (c == '.'){
					if (skip){
						number_checker = false;	
					}
					else{
						current_token.stringValue[i] = c;
						skip = true;						
					}
				} else if(c == '_'){ 
					i--;
				} else {
					number_checker = false;				
				}
				i++;
				record_token = true;					
			}
			
			/*-------------------------------------------------------------------------------  
				We only want to record the token if it was a number token, not a period token 
			-------------------------------------------------------------------------------*/
			if (record_token){
				current_token.type = "T_NUMBERVAL"; 
				current_token.line = line_counter;
				token_list.createnode(current_token);
				current_token = clear_token(current_token);
			}

			grab_prev_c = true;	skip = false; record_token = false;
			i = 0;			
		} 
/*--------------------------  
	Identifier Token Section 
--------------------------*/
		else if (is_letter(c)){						//Checks to see if it is a letter, triggers flag and stores char
			current_token.stringValue[i] = c;
			i++;

			ident_checker = true;
			while (ident_checker){
				inFile.get(c); c = tolower(c);
				if (is_num(c) || is_letter(c) || c == '_'){
					current_token.stringValue[i] = c;
				} else {
					ident_checker = false;
					if (c == '\n' || c == '\t' || c == ' ' || c == '['|| c == ']' || c == '(' || c == ')' || c == ';' || c == '.'){
						
					} else {
						error_handler.error(line_counter, 5);
						//cout << c << endl;
					}
				}
				i++;
			}

			/*------------------------  
				Reserved Token Section 
			------------------------*/
			if (reserved_word_checker(current_token, "program")){current_token.type = "T_PROGRAM"; current_token.line = line_counter; token_list.createnode(current_token); }
			else if (reserved_word_checker(current_token, "is")){current_token.type = "T_IS"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "begin")){current_token.type = "T_BEGIN"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "end")){current_token.type = "T_END"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "global")){current_token.type = "T_GLOBAL"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "procedure")){current_token.type = "T_PROCEDURE"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "in")){current_token.type = "T_IN"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "out")){current_token.type = "T_OUT"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "inout")){current_token.type = "T_INOUT"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "integer")){current_token.type = "T_INTEGER"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "float")){current_token.type = "T_FLOAT"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "bool")){current_token.type = "T_BOOL"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "char")){current_token.type = "T_CHAR"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "if")){current_token.type = "T_IF"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "then")){current_token.type = "T_THEN"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "else")){current_token.type = "T_ELSE"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "for")){current_token.type = "T_FOR"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "return")){current_token.type = "T_RETURN"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "true")){current_token.type = "T_TRUE"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "false")){current_token.type = "T_FALSE"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "not")){current_token.type = "T_NOT"; current_token.line = line_counter; token_list.createnode(current_token);}
			else if (reserved_word_checker(current_token, "string")){current_token.type = "T_STRING"; current_token.line = line_counter; token_list.createnode(current_token);}
			else {
				current_token.type = "T_IDENTIFIER";
				current_token.line = line_counter;
				token_list.createnode(current_token);
				current_token = clear_token(current_token);
			}
			current_token = clear_token(current_token);

			ident_checker = false; grab_prev_c = true;
			i = 0;
		} 
/*---------------------  
	Char Token Section 
-----------------------*/
		else if (c == '\''){						
			char_length = 0; leave_while = false;
			
			inFile.get(c); c = tolower(c);

			while (!leave_while){
				current_token.stringValue[i] = c;
				i++; char_length++;

				inFile.get(c); c = tolower(c);

				peeker = inFile.peek();

				if (c == '\''){					//Char ends with ' so we can leave the while loop
					leave_while = true;
				}
				else if (c == ';' || c == '_' || c == ':' || c == '.' || c == '\"' || is_letter(c) || is_num(c)){} 		//Do nothing for these accepted chars
				
				else if (peeker == EOF){		//Error for forgeting the ending '
					error_handler.error(line_counter, 3);
					break;
				} else {
					error_handler.error(line_counter, 4);		//Error for illegal chars
				}

				if (char_length > 1 && !(skip)){	//Error for char that is longer than 1
					error_handler.error(line_counter, 2);
					skip = true;
				} 
			}

			current_token.type = "T_CHARVAL"; 
			current_token.line = line_counter;
			token_list.createnode(current_token);
			current_token = clear_token(current_token);
			skip = false; leave_while = false;
			i = 0;
		} 
/*---------------------  
	String Token Section 
-----------------------*/
		else if (c == '\"'){						
			leave_while = false;
			
			inFile.get(c); c = tolower(c);

			while (!leave_while){
				current_token.stringValue[i] = c;
				i++; 

				inFile.get(c); c = tolower(c);

				peeker = inFile.peek();

				if (c == '\"'){
					leave_while = true;
				}
				else if (c == ';' || c == '_' || c == ':' || c == '.' || c == '\'' || is_letter(c) || is_num(c) || c == ' '){}
				else if (peeker == EOF){
					error_handler.error(line_counter, 3);
					break;
				} else {
					//out << c << endl;
					error_handler.error(line_counter, 4);
				}

				
				
			}

			current_token.type = "T_STRINGVAL"; 
			current_token.line = line_counter;
			token_list.createnode(current_token);
			current_token = clear_token(current_token);
			leave_while = false;
			i = 0;
		}
		/*else if (c == '_' || c == ':'){

			current_token.stringValue[i] = c;
			i++;
		} */

/*--------------------------------------------------  
	All other chars (like spaces, newlines, symbols)
--------------------------------------------------*/

		else {										
			
			i = 0;
			
			current_token = clear_token(current_token);

			if (c == '\n') {line_counter++;} else if (c == '\t' || isspace(c) ){} 
			else if (c == '='){ 
				inFile.get(c); c = tolower(c);
				if (c == '='){
					temp.type = "T_EQUALTO"; current_token.line = line_counter; token_list.createnode(temp);
					
				} else {
					//temp.type = "T_ASSIGN"; current_token.line = line_counter; token_list.createnode(temp);
					grab_prev_c = true;
				}	
			}
			else if (c == ':'){ 
				
				inFile.get(c); c = tolower(c);
				if (c == '='){
					temp.type = "T_ASSIGN"; current_token.line = line_counter; token_list.createnode(temp);
					
				} else {
					temp.type = "T_COLON"; current_token.line = line_counter; token_list.createnode(temp);
					grab_prev_c = true;
				}	
			}
			else if (c == '&'){ temp.type = "T_AND"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == '|'){ temp.type = "T_OR"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == '+'){ temp.type = "T_ADD"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == '-'){ temp.type = "T_MINUS"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == '<'){ 
				inFile.get(c); c = tolower(c);
				if (c == '='){
					temp.type = "T_LESSTHANEQUAL"; current_token.line = line_counter; token_list.createnode(temp);
				} else {
					temp.type = "T_LESSTHAN"; current_token.line = line_counter; token_list.createnode(temp);
					grab_prev_c = true;
				}				
			}
			else if (c == '>'){ 			
				inFile.get(c); c = tolower(c);
				if (c == '='){
					temp.type = "T_GREATERTHANEQUAL"; current_token.line = line_counter; token_list.createnode(temp);
				} else {
					temp.type = "T_GREATERTHAN"; current_token.line = line_counter; token_list.createnode(temp);
					grab_prev_c = true;
				}			
			}
			else if (c == '!'){
				inFile.get(c); c = tolower(c); 
				if (c == '='){
					temp.type = "T_NOTEQUALTO"; current_token.line = line_counter; token_list.createnode(temp);
				} else {
					temp.type = "T_EXLAMANTION"; current_token.line = line_counter; token_list.createnode(temp);
					grab_prev_c = true;
				}
			}
			else if (c == '*'){ temp.type = "T_MULT"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == '/'){ 
				inFile.get(c); c = tolower(c);
				if (c == '/'){
					inFile.get(c);
					while (c != '\n' || c == (-1)){
						inFile.get(c);
					}
				} else if(c == '*'){
					inFile.get(c);
					nest_comment = true;
					while (nest_comment){
						if (c == '*'){
							inFile.get(c);
							if (c == '/'){
								if (num_of_comments > 0){
									num_of_comments--;
								} else if (num_of_comments == 0){
									nest_comment = false;
								}
							}
						} else if (inFile.eof()){
							nest_comment = false;
						} else if (c == '/'){
							inFile.get(c);
							if (c == '*'){
								num_of_comments++; 
							}
						} else if (c == '\n'){
							line_counter++;
						}
						inFile.get(c);
					}
					grab_prev_c = true;
					num_of_comments = 0;
					nest_comment = false;
				} else {
					temp.type = "T_DIVIDE"; current_token.line = line_counter; token_list.createnode(temp);
					grab_prev_c = true;
				}
			}
			else if (c == ','){ temp.type = "T_COMMA"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == ';'){ temp.type = "T_SEMICOLON"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == '('){ temp.type = "T_LPARANTH"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == ')'){ temp.type = "T_RPARANTH"; current_token.line = line_counter; token_list.createnode(temp);} 
			else if (c == '['){ temp.type = "T_LBRACKET"; current_token.line = line_counter; token_list.createnode(temp);}
			else if (c == ']'){ temp.type = "T_RBRACKET"; current_token.line = line_counter; token_list.createnode(temp);}
			

			else {temp.type = "T_UNKOWN"; current_token.line = line_counter; token_list.createnode(temp); error_handler.error(line_counter, 1);}
				
			temp = clear_token(temp);
		} 

		peeker = inFile.peek();

		if (peeker == EOF){
			current_token.type = "T_ENDFILE"; 
			current_token.line = line_counter;
			token_list.createnode(current_token);
			current_token = clear_token(current_token);
		}


	}
	inFile.close();	
	return token_list;
}

bool parameter_check(int argc, char *argv[]){								
	if (argc != 2){										//this condition makes sure that there is a second parameter
        cout << "Enter a txt file after the program name" << endl;
        cout << "example: program txt" << endl;
   		return false;
	 } else if (argc > 2){  							//this condition makes sure that there aren't more than 2 parameters
        cout << "You entered too many parameters" << endl;
        cout << "example: program txt" << endl;
    	return false;
	} 	
}

void parser(list scan_list){
	ParseTree tree; tokens temp; int error_count = 0; symbolNode symbol_temp; bool type_match = true;

/*--------------------------------  
	Symbol Table and Parser 			reorder ST and Par
--------------------------------*/
  	scan_list.reset_pos();
	for (int i = 0; i < scan_list.get_size(); i++){
		temp = scan_list.get_one();

		tree.setNewNode(temp.type);
		tree.createnode_2(temp.type);
		tree.clearNewNode();

		symbol_temp.type = temp.type; 
		symbol_temp.line_num = temp.line;
		symbol_temp.str_val = temp.stringValue;
		sym.init(temp.type,temp.stringValue, symbol_temp);

		if (tree.getExpressFlag() && (temp.type == "T_IDENTIFIER" || temp.type == "T_NUMBERVAL" || temp.type == "T_STRINGVAL" || temp.type == "T_CHARVAL" || temp.type == "T_FALSE" || temp.type == "T_TRUE")){
			type_match = sym.insertTC(temp.stringValue, temp.type);

		}  if (!tree.getExpressFlag()){		
			sym.clearTC();
		}

		if (!type_match){error_handler.error(temp.line, 8); type_match = true; }
	
		if (!(tree.getLegit()) && error_count < 2){
			error_handler.error(temp.line, 2, temp.stringValue);			
			error_count++;
		} 
		
	}

 	sym.printAll();
/*----------------------------------------------------  
	Checks to see if the Parser passed or failed
----------------------------------------------------*/
	cout << endl << "========== PARSE ==========" << endl;
	if (tree.getLegit()){
		cout << "Is Legit" << endl;
	} else {
		cout << "Is Not Legit" << endl;
	}
	cout << endl;
	cout << "===========================" << endl << endl;


}	


int main(int argc, char *argv[]){	
	bool correct_input = false; list scan_list;
	correct_input = parameter_check(argc, argv);

	if (correct_input) {
		scan_list = scan(argv);
		symbol_gen(scan_list);
		parser(scan_list);
	}

	return 0;
}