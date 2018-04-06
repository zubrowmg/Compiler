#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>

#include "CodeGen.h"
#include "Symbol.h"
#include "Error.h"
#include "ParseTree.h"

using namespace std;

CodeGen::CodeGen()
{	
	
	myfile.open ("CodeGen.c");
	myfile2.open ("CodeGenReg.c");

	prog_start = false; 
	prog_begin = false; 
	prog_end = false; 
	proc_start = false; 
	proc_begin = false; 
	//proc_end = false;

	prev_TT_prog = false; 
	prev_TT_proc = false; 
	prev_TT_ident = false; prev_TT_ident2 = false;
	prev_TT_is = false;
	prev_TT_begin = false;
	prev_TT_end = false;
	prev_TT_semi = false;
	prev_TT_LPAR = false;

	prog_declare = false;

	MM_Index = 0;
	current_array_index = 0;
	//assign_option = 0;
}

void CodeGen::init(tokens tok, Symbol sym)
{
	list empty; 
	sym_table = sym;
	//-------- Initialized Program Space Variables --------//
	if (prog_declare){
		temp_list.setCG("prog_declare");
		temp_list.createnode(tok);

		

		if (tok.type == "T_SEMICOLON"){
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		}
	} 


	else if (prog_begin) {
		temp_list.setCG("prog_begin");
		temp_list.createnode(tok);
//cout << "    |" << tok.type << tok.stringValue << endl;;
		if (tok.type == "T_SEMICOLON"){
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		}
	} else {
		temp_list = empty;
	}


	

	if (prog_end && tok.type == "T_ENDFILE"){
		printCode();
	}
	set_flags(tok);
}

void CodeGen::output(list temp_list2){
	tokens tok_temp;

	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one(); 
		if (tok_temp.type == "T_CHAR"){
			myfile << "char";
		} else if (tok_temp.type == "T_INTEGER"){
			myfile << "int";
		} else if (tok_temp.type == "T_STRING"){
			myfile << "string";
		} else if (tok_temp.type == "T_FLOAT"){
			myfile << "float";
		} else if (tok_temp.type == "T_BOOL"){
			myfile << "bool";
		} else if (tok_temp.type == "T_LPARANTH"){
			myfile << "(";
		} else if (tok_temp.type == "T_RPARANTH"){
			myfile << ")";
		} else if (tok_temp.type == "T_LBRACKET"){
			myfile << "[";
		} else if (tok_temp.type == "T_RBRACKET"){
			myfile << "]";
		} else if (tok_temp.type == "T_ADD"){
			myfile << "+";
		} else if (tok_temp.type == "T_ASSIGN"){
			myfile << "=";
		} else if (tok_temp.type == "T_NUMBERVAL"){
			myfile << tok_temp.stringValue;
		}

		else if (tok_temp.type == "T_IDENTIFIER"){
			myfile << " " << tok_temp.stringValue;
			MM_Index = sym_table.setMMIndex(tok_temp.stringValue, MM_Index);
		} else if (tok_temp.type == "T_SEMICOLON"){
			myfile << ";" << "\n";
		}
	}

}

void CodeGen::outputValType(tokens tok_temp){
		if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_INTEGER"){
			myfile2 << ".int_val";
		} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_BOOL"){
			myfile2 << ".bool_val";
		} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_CHAR"){
			myfile2 << ".char_val";
		} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_STRING"){
			myfile2 << ".char_val";
		} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_FLOAT"){
			myfile2 << ".float_val";
		}	
	
}

void CodeGen::output2(list temp_list2){
	int reg_index; bool string_detected = false; bool go_back = false; bool str_ident = false;
	tokens tok_temp2; int bs1, bs2, bs3;
	tokens tok_temp; tokens tok_val_type; int count = 1; int count2 = 1;
	bool for_state, if_state, ret_state, assign_state, proc_state;
	for_state = false; if_state = false; assign_state = false, ret_state = false; proc_state = false;
	//-------- Check to See Statement type --------//;
	temp_list2.reset_pos();
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one();
		if (count == 1){
			tok_val_type = tok_temp;
			count = count + 1;
		}
		if (tok_temp.type == "T_FOR"){
			for_state = true;
			break;
		} else if (tok_temp.type == "T_IF"){
			if_state = true;
			break;
		} else if (tok_temp.type == "T_RETURN"){
			ret_state = true;
			break;
		} else if (tok_temp.type == "T_ASSIGN"){
			assign_state = true;
			break;
		} else if (tok_temp.type == "T_SEMICOLON"){
			proc_state = true;
			break;
		}
	}
	temp_list2.reset_pos();


	
	if (if_state){
		
	} else if (for_state){
		
	} else if (ret_state){
		//myfile2 << "\t" << "return 0;" << "\n";
	} else if (proc_state) {

	} else if (assign_state){
		//temp_list.reset_pos();
		for (int j = 0; j < temp_list2.get_size(); j++){
			tok_temp = temp_list2.get_one();
			if (tok_temp.type == "T_ASSIGN"){
				
				break;
			}
		}

		count = 1; count2 = 1; int str_array_index = 0;
		for (int j = 0; j < temp_list2.get_size(); j++){
			tok_temp = temp_list2.get_one(); 
//cout << tok_temp.type << " " << tok_temp.stringValue << " 3: " << temp_list2.look_ahead().stringValue << endl;
			//count = count + 1;
			if (count == 1){
				//if (tok_temp.type == "T_IDENTIFIER"){
					if (tok_temp.type == "T_STRINGVAL"){
						reg_index = 0;
						string_detected = true;
						for (int f = 0; f < strLength(tok_temp.stringValue); f++){
							
							myfile2 << "R[" << f << "]";
							outputValType(tok_val_type);
							myfile2 << "=";

							myfile2 << "\'" << tok_temp.stringValue[f] << "\'" ;
							myfile2 << ";" << "\n" << "\t";
							reg_index++;
						}
					} else if (tok_temp.type == "T_IDENTIFIER" && (sym_table.returnValType(tok_temp.stringValue)).is_array){
						reg_index = 0;
						string_detected = true;
						for (int f = 0; f < (sym_table.returnValType(tok_temp.stringValue)).array_size; f++){
							
							myfile2 << "R[" << f << "]";
							outputValType(tok_val_type);
							myfile2 << "=";

							myfile2 << "MM[";
							myfile2 << sym_table.getMMIndex(tok_temp.stringValue) + f;
							myfile2 << "]";
							outputValType(tok_val_type);
							myfile2 << ";" << "\n" << "\t";
							
							reg_index++;
						}
					} else {
						myfile2 << "R[0]";
						outputValType(tok_val_type);
						myfile2 << "=";
					}
					count = count + 1;
				//}
			} else {
				//If the next token is a STRING VALUE
				if (temp_list2.look_ahead_no_wrap().type == "T_STRINGVAL"){
					myfile2  << "R[" << str_array_index << "]";
					outputValType(tok_val_type);
					myfile2 << "=R[" << str_array_index << "]";
					outputValType(tok_val_type);
					go_back = true;

				} else if (temp_list2.look_ahead_no_wrap().type == "T_IDENTIFIER" && !(temp_list2.look_ahead_no_wrap().single_array_access) && (sym_table.returnValType(temp_list2.look_ahead_no_wrap().stringValue)).is_array){
					myfile2  << "R[" << str_array_index << "]";
					outputValType(tok_val_type);
					myfile2 << "=R[" << str_array_index << "]";
					outputValType(tok_val_type);
					go_back = true;
					str_ident = true;
	
				} 
				else if (tok_temp.type != "T_CHARVAL" ){
				if (tok_temp.type != "T_STRINGVAL" ){
				if (tok_temp.type != "T_NUMBERVAL" ){
				if (tok_temp.type != "T_SEMICOLON" ){
				if (tok_temp.type != "T_IDENTIFIER" ){
					myfile2  << "R[0]";
					outputValType(tok_val_type);
					myfile2 << "=R[0]";
					outputValType(tok_val_type);
					count = count + 1;
				}}}}}
			}
cout << tok_temp.type << " " << tok_temp.stringValue << " " << tok_temp.single_array_access << " " << go_back << endl;
			temp_list2 = outputMain(temp_list2, tok_temp, tok_val_type, reg_index, str_array_index, go_back);
		if (go_back && (tok_temp.type == "T_STRINGVAL" || tok_temp.type == "T_IDENTIFIER")){
				str_array_index = str_array_index + 1;
				
				if ((str_array_index  >= 50) && tok_temp.type == "T_IDENTIFIER") {
					go_back = false; str_ident = false;
					str_array_index = 0;
				} else if ((str_array_index  >= strLength(tok_temp.stringValue)) && tok_temp.type == "T_STRINGVAL" ){
					go_back = false; str_ident = false;
					str_array_index = 0;
				} else {
					temp_list2.goBackOne();
					temp_list2.goBackOne();
					j = j - 2;
				}
				
			}
			if (tok_temp.type == "T_SEMICOLON"){ break; }
		}
	}
	
}

list CodeGen::outputMain(list temp_list2, tokens tok_temp, tokens tok_val_type, int reg_index, int str_array_index, bool go_back){


	if (tok_temp.type == "T_IDENTIFIER" ){
		
		temp_list2 = arrayOutput(temp_list2, tok_temp, reg_index);

		outputValType(tok_val_type);
		myfile2 << ";" << "\n" << "\t";
	}
	else if (tok_temp.type == "T_LPARANTH"){
		//myfile << "(";
	} else if (tok_temp.type == "T_RPARANTH"){
		//myfile << ")";
	} else if (tok_temp.type == "T_LBRACKET"){
		//myfile << "[";
	} else if (tok_temp.type == "T_RBRACKET"){
		//myfile << "]";
	} else if (tok_temp.type == "T_ADD"){	  		myfile2 << "+";
	} else if (tok_temp.type == "T_MINUS"){	  		myfile2 << "-";
	} else if (tok_temp.type == "T_MULT"){	  		myfile2 << "*";
	} else if (tok_temp.type == "T_DIVIDE"){  		myfile2 << "/";
	} else if (tok_temp.type == "T_AND"){     		myfile2 << "&&";
	} else if (tok_temp.type == "T_OR"){      		myfile2 << "||";
	} else if (tok_temp.type == "T_NOT"){     		myfile2 << "!";
	} else if (tok_temp.type == "T_ASSIGN"){  		myfile2 << "=";
	} else if (tok_temp.type == "T_LESSTHAN"){		myfile2 << "<";
	} else if (tok_temp.type == "T_LESSTHANEQUAL"){	myfile2 << "<=";
	} else if (tok_temp.type == "T_GREATERTHAN"){	myfile2 << ">";
	} else if (tok_temp.type == "T_GREATERTHANEQUAL"){myfile2 << ">=";
	} else if (tok_temp.type == "T_NOTEQUALTO"){	myfile2 << "!=";
	} else if (tok_temp.type == "T_EQUALTO"){		myfile2 << "==";
	} else if (tok_temp.type == "T_TRUE"){			myfile2 << "true";
	} else if (tok_temp.type == "T_FALSE"){			myfile2 << "false";
	} else if (tok_temp.type == "T_CHARVAL"){		myfile2 << "\'" << tok_temp.stringValue << "\'";
		myfile2 << ";" << "\n" << "\t";
	} else if (tok_temp.type == "T_NUMBERVAL"){		myfile2 << tok_temp.stringValue;
		myfile2 << ";" << "\n" << "\t";
	} else if (tok_temp.type == "T_STRINGVAL" && go_back){	
		myfile2 << "\'" << tok_temp.stringValue[str_array_index] << "\'" ;
		myfile2 << ";" << "\n" << "\t";
		
	} else if (tok_temp.type == "T_IDENTIFIER" && go_back && !(tok_temp.single_array_access)){	
		myfile2 << "\'" << tok_temp.stringValue[str_array_index] << "\'" ;
		myfile2 << ";" << "\n" << "\t";
		
	} else if (tok_temp.type == "T_SEMICOLON"){
		//myfile2 << ";" << "\n" << "\t";
		temp_list2.reset_pos();
		tok_temp = temp_list2.get_one();
		
		temp_list2 = arrayOutput(temp_list2, tok_temp, reg_index);

		if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_STRING"){
			
			for (int f = 0; f < 50; f++){
				
				myfile2 << "MM[";
				myfile2 << sym_table.getMMIndex(tok_temp.stringValue) + f;
				myfile2 << "]";
				outputValType(tok_val_type);
				
				myfile2 << "=R[" << f << "]";
				outputValType(tok_val_type);
				myfile2 << ";" << "\n";
				if (f == reg_index - 1){}
				else {myfile2 << "\t";}
			}
		} else {
			outputValType(tok_val_type);
			myfile2 << "=R[0]";
			outputValType(tok_val_type);
			myfile2 << ";" << "\n";
			
		}
		
	}
	return temp_list2;
}

int CodeGen::strLength(char str[256]){
	int length = 0;
	for (int i = 0; i < 256; i++){ 
		if (str[i] == '\0' || str[i] == ' '){			
		} else {
			length++;
		}
	}
	return length;
}

list CodeGen::arrayOutput(list temp_list2, tokens tok_temp, int reg_index){
	tokens tok_temp2; int left, right; 
	

	left = (sym_table.returnValType(tok_temp.stringValue)).array_left;
	right = (sym_table.returnValType(tok_temp.stringValue)).array_right;

	if ((temp_list2.look_ahead()).type == "T_LBRACKET"){
		myfile2 << "MM["; 
		tok_temp2 = temp_list2.get_one();
		tok_temp2 = temp_list2.get_one();					
// !!!!!!!!! NEED A BETTER INDEX FOR EGATIVE NUMBERS !!!!
		if (left >= 0 && right >= 0 && left < right){
			myfile2 << (sym_table.getMMIndex(tok_temp.stringValue) + atoi(tok_temp2.stringValue) - left);	
		}		
		tok_temp2 = temp_list2.get_one();
		myfile2 << "]";
	} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_STRING" && temp_list2.look_back().type == "T_SEMICOLON") {
//cout << "HI";			
	} else if ((sym_table.returnValType(tok_temp.stringValue)).is_array) {
		myfile2 << "MM[";
		myfile2 << sym_table.getMMIndex(tok_temp.stringValue) + current_array_index;
		myfile2 << "]";
		current_array_index = current_array_index + 1;
		if (current_array_index >= (sym_table.returnValType(tok_temp.stringValue)).array_size){
			current_array_index = 0;
		}
	} else {
		myfile2 << "MM[";
		myfile2 << sym_table.getMMIndex(tok_temp.stringValue);
		myfile2 << "]";
	}
	

	return temp_list2;
				
}

void CodeGen::printCode()
{
	for (int i = 0; i < code_gen_order.size(); i++){
		code_gen_order[i].reset_pos();
		SinArAChecker(code_gen_order[i]);		
	}
	for (int i = 0; i < code_gen_order.size(); i++){
		code_gen_order[i].reset_pos();
		if (code_gen_order[i].getCG() == "prog_declare"){ 			
			output(code_gen_order[i]);
		}		
	}


	myfile2 << "#include <string.h>" << "\n"
	<< "#include <stdio.h>" << "\n"
	<< "#include <stdbool.h>" << "\n" << "\n";
	
	myfile2 << "union val{" << "\n" 
		<< "\t" << "int int_val;" << "\n" 
		<< "\t" << "bool bool_val;" << "\n"
		<< "\t" << "char char_val;" << "\n"
		<< "\t" << "float float_val;" << "\n"
	 << "};" << "\n" << "\n";
	myfile2 << "union val MM[10000];" << "\n";
	myfile2 << "union val R[100];" << "\n";
	
	myfile << "\n" << "int main(){" << "\n";
	myfile2 << "\n" << "int main(){" << "\n";
	
	for (int i = 0; i < code_gen_order.size(); i++){
		code_gen_order[i].reset_pos();
		if (code_gen_order[i].getCG() == "prog_begin"){ 
			

			output2(code_gen_order[i]);
		}		
	}
		
	myfile << "}" ;
	myfile2 << "}" ;

  	//myfile << "Writing this to a file.\n";
  	myfile.close();
  	myfile2.close();
}

void CodeGen::display()
{
	for (int i = 0; i < code_gen_order.size(); i++){
		cout << "\t" << code_gen_order[i].getCG() << endl;
		code_gen_order[i].display();
	}
}

void CodeGen::set_flags(tokens tok)
{
	//cout << tok.type << endl;

	if (tok.type == "T_PROGRAM" && prev_TT_end && prog_begin ) { proc_begin = false; prog_end = true; }
	if (tok.type == "T_BEGIN" && prog_start && !proc_begin && !proc_start) { prog_start = false; prog_begin = true; prog_declare = false;}

	if (tok.type == "T_END" ) { prev_TT_end = true; } else { prev_TT_end = false; }
	if (tok.type == "T_END" && proc_begin ) { proc_begin = false; prog_declare = true; }
	if (tok.type == "T_BEGIN" && proc_start) { proc_begin = true; proc_start = false;}

	if (tok.type == "T_RPARANTH" && prev_TT_LPAR) { prev_TT_LPAR = false; proc_start = true;  }
	if (tok.type == "T_LPARANTH" && prev_TT_ident2) { prev_TT_LPAR = true; } 
	if (tok.type == "T_IDENTIFIER" && prev_TT_proc) { prev_TT_ident2 = true; } else { prev_TT_ident2 = false; }
	if (tok.type == "T_PROCEDURE"){ prev_TT_proc = true; prog_declare = false; } else { prev_TT_proc = false; }

	if (tok.type == "T_IS" && prev_TT_ident){ prog_start = true; prog_declare = true;}
	if (tok.type == "T_IDENTIFIER" && prev_TT_prog){ prev_TT_ident = true; } else { prev_TT_ident = false; }
	if (tok.type == "T_PROGRAM"){ prev_TT_prog = true; } else { prev_TT_prog = false; }
}

void CodeGen::SinArAChecker(list temp_list2){
	tokens tok_temp; bool last_T_ident = false;
	tokens tok_last_ident;
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one();
		
		if (last_T_ident && tok_temp.type == "T_LBRACKET"){
			temp_list2.modifyLast(tok_last_ident);

		}

		if (tok_temp.type == "T_IDENTIFIER"){
			last_T_ident = true; tok_last_ident = tok_temp;
		} else {
			last_T_ident = false;
		}
	}
}