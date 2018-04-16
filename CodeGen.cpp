#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string.h>

#include "CodeGen.h"
#include "Symbol.h"
#include "Error.h"
#include "ParseTree.h"

using namespace std;

CodeGen::CodeGen(){	
	myfile.open ("CodeGen.c");
	myfile2.open ("CodeGenReg.c");

	prog_start = false; prog_begin = false; prog_end = false; proc_start = false; proc_begin = false; 

	prev_TT_prog = false; prev_TT_proc = false;	prev_TT_ident = false; prev_TT_ident2 = false;
	prev_TT_is = false;	prev_TT_begin = false; prev_TT_end = false;	prev_TT_semi = false; prev_TT_LPAR = false;

	prog_declare = false;

	prev_TT_int = false; prev_TT_str = false; prev_TT_bool = false; prev_TT_float = false; prev_TT_char = false;

	MM_Index = 0;
	current_array_index = 0;

	inside_if_statment = false;
	
	goto_index = 0; if_count = 0;

	end_if_before = false; normal_if_count = 0; normal_end_if_count = 0; seq_if = 0;

	for_encountered = false; normal_for_count = 0; normal_end_for_count = 0; seq_for = 0; goto_index_for = 0;
	for_count = 0;
}

list CodeGen::preInit(list code_gen_list){
	tokens tok_temp; list temp_list2;
	bool for_state, if_state, ret_state, assign_state, proc_state, end_if_for, if_else_state;
	for_state = false; if_state = false; assign_state = false, ret_state = false; proc_state = false;
	end_if_for = false; if_else_state = false;
	//-------- Check to See Statement type --------//;
	temp_list2.reset_pos();
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one();
		
		if (tok_temp.type == "T_FOR"){
			break;
		} else if (tok_temp.type == "T_END"){
			end_if_for = true;
			break;
		} else if (tok_temp.type == "T_IF"){
			if_state = true;
			break;
		} else if (tok_temp.type == "T_ELSE"){
			if_else_state = true;
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
		inside_if_statment = true;
		
	} else if (if_else_state){

	} else if (end_if_for){
		inside_if_statment = false;
	} 

	return code_gen_list;
}

void CodeGen::init(tokens tok, Symbol sym){
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

		if (tok.type == "T_SEMICOLON" && !for_encountered){
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		} else if (tok.type == "T_THEN" ){			//If statements
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		} else if (tok.type == "T_ELSE"){
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		} else if (tok.type == "T_IF" && prev_TT_end){
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		} else if (tok.type == "T_RPARANTH" && for_encountered){
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		} else if (tok.type == "T_FOR" && prev_TT_end){
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		} 
	} else {
		temp_list = empty;
	}

	
	if (tok.type == "T_FOR"){
		for_encountered = true;
	} else if (tok.type == "T_RPARANTH" && for_encountered){
		for_encountered = false;
	}

	if (prog_end && tok.type == "T_ENDFILE"){
		printCode();
	}
	set_flags(tok);
}

void CodeGen::printCode(){
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
	myfile2 << "union val R[10000];" << "\n";
	myfile2 << "union val R2[10000];" << "\n";
	
	myfile2 << "\n" << "int main(){" << "\n";
		
	myfile2 << "FILE *outfile;" << " outfile = fopen(\"output.txt\",\"w\");" << "\n" << "\n";
	myfile2 << "FILE *infile;" << " infile = fopen(\"input.txt\",\"r\");" << "\n" << "\n";

	for (int i = 0; i < code_gen_order.size(); i++){
		code_gen_order[i].reset_pos();
		if (code_gen_order[i].getCG() == "prog_begin"){ 
			output2(code_gen_order[i]);
		}		
	}

	myfile2 << "\n" << "fclose(outfile);" << "\n";
	myfile2 << "fclose(infile);" << "\n";

	myfile << "}" ;
	myfile2 << "}" ;

  	//myfile << "Writing this to a file.\n";
  	myfile.close();
  	myfile2.close();
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
			//myfile << " " << tok_temp.stringValue;
			MM_Index = sym_table.setMMIndex(tok_temp.stringValue, MM_Index);
		} else if (tok_temp.type == "T_SEMICOLON"){
			myfile << ";" << "\n";
		}
	}

}

void CodeGen::outputValType(tokens tok_temp){
	if (tok_temp.type == "T_IDENTIFIER"){	
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
	} else if (tok_temp.type == "T_STRINGVAL"){
		myfile2 << ".char_val";
	} else if (tok_temp.type == "T_NUMBERVAL"){
		if (isFloat(tok_temp.stringValue)){
			myfile2 << ".float_val";
		} else {
			myfile2 << ".int_val";
		}
	} else if (tok_temp.type == "T_CHARVAL"){
		myfile2 << ".char_val";
	} else if (tok_temp.type == "T_TRUE" || tok_temp.type == "T_FALSE"){
		myfile2 << ".bool_val";
	}
	
}

bool CodeGen::isFloat(char str[256]){
	for (int i = 0; i < 256; i++){
		if (str[i] == '.'){
			return true;
		}
	}
	return false;
}

void CodeGen::output2(list temp_list2){
	tokens tok_val_type; int count = 1; tokens tok_temp; 
	bool for_state, if_state, ret_state, assign_state, proc_state, end_if, if_else_state, end_for = false, last_tok_end = false;
	for_state = false; if_state = false; assign_state = false, ret_state = false; proc_state = false;
	end_if = false; if_else_state = false;
	//-------- Check to See Statement type --------//;
	temp_list2.reset_pos();
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one();
		if (count == 1){
			tok_val_type = tok_temp;
			count = count + 1;
		}
		
		if (tok_temp.type == "T_FOR" && last_tok_end){
			end_for = true;
			break;
		} else if (tok_temp.type == "T_FOR"){
			for_state = true;
			break;
		} else if (tok_temp.type == "T_IF" && last_tok_end){
			end_if = true;
			break;
		} else if (tok_temp.type == "T_IF"){
			if_state = true;
			break;
		} else if (tok_temp.type == "T_ELSE"){
			if_else_state = true;
			break;
		} else if (tok_temp.type == "T_RETURN"){
			ret_state = true;
			break;
		} else if (tok_temp.type == "T_ASSIGN"){
			assign_state = true;
			break;
		} else if (j == temp_list2.get_size() - 1){
			proc_state = true;
		}

		if (tok_temp.type == "T_END"){
			last_tok_end = true;
		} else {
			last_tok_end = false;
		}
	}
	temp_list2.reset_pos();

 //temp_list2.display();
 //cout << "---" << endl;
	
	if (if_state){
		inside_if_statment = true;
		if_count = if_count + 3;
		generalIf(temp_list2);
		normal_if_count = normal_if_count + 1;
	} else if (if_else_state){
		generalIfElse(temp_list2);
	} else if (end_if){
		inside_if_statment = false;
		generalIfEnd();
		if_count = if_count - 3;
		normal_end_if_count = normal_end_if_count + 1;
		if (normal_end_if_count == normal_if_count){
			seq_if = seq_if + 3;
		}
	} else if (for_state){
		for_count = for_count + 3;
		generalFor(temp_list2);
		normal_for_count = normal_for_count + 1;
	} else if (end_for){
		generalForEnd();
		for_count = for_count - 3;
		normal_end_for_count = normal_end_for_count + 1;
		if (normal_end_for_count == normal_for_count){
			seq_for = seq_for + 3;
		}
	} else if (ret_state){

	} else if (proc_state) {
		generalProcStatement(temp_list2);
	} else if (assign_state){	
		generalAssignStatement(temp_list2);		
	}
}

void CodeGen::generalProcStatement(list temp_list2){
	tokens tok_temp;
	
	temp_list2.reset_pos();
	tok_temp = temp_list2.get_one();
	if ( strcmp(tok_temp.stringValue, "putinteger") == 0 || strcmp(tok_temp.stringValue, "putbool") == 0
			|| strcmp(tok_temp.stringValue, "putfloat") == 0 || strcmp(tok_temp.stringValue, "putstring") == 0
			|| strcmp(tok_temp.stringValue, "putchar") == 0 || strcmp(tok_temp.stringValue, "getinteger") == 0
			|| strcmp(tok_temp.stringValue, "getbool") == 0 || strcmp(tok_temp.stringValue, "getfloat") == 0
			|| strcmp(tok_temp.stringValue, "getstring") == 0 || strcmp(tok_temp.stringValue, "getchar") == 0) {
 
		generalIO(temp_list2);

	} else {
		evalProcStatement(temp_list2);
	}
}

void CodeGen::generalIO(list temp_list2){
	tokens tok_temp, tok_temp2; list new_list; bool last_T_LBRACKET = false; 

	temp_list2.reset_pos();
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp2 = temp_list2.get_one();
		if (temp_list2.look_ahead_no_wrap().type == "T_LBRACKET") {
			tok_temp2.single_array_access = true;
			tok_temp2.index = atoi((temp_list2.look_ahead_two_no_wrap()).stringValue);
			new_list.createnode(tok_temp2);
		} else if (last_T_LBRACKET){
			//Do nothing, we don't want the number in the []
		} else if (tok_temp2.type == "T_RBRACKET" || tok_temp2.type == "T_LBRACKET"){
			//Do nothing
		} else {
			new_list.createnode(tok_temp2);
		}

		if (tok_temp2.type == "T_LBRACKET" ){
			last_T_LBRACKET = true;
		} else {
			last_T_LBRACKET = false;
		}
	}

	new_list.reset_pos();
	tok_temp = new_list.get_one();
	if (strcmp(tok_temp.stringValue, "putinteger") == 0){
		tok_temp2 = new_list.look_ahead_two_no_wrap();

		if (tok_temp2.type == "T_NUMBERVAL"){
			if (isFloat(tok_temp2.stringValue)) {
				error_handler.error(tok_temp2.line, 10);
			} else {
				for (int k = 0; k < strLength(tok_temp2.stringValue); k++){
					myfile2 << "putc(" << "\'" << (tok_temp2.stringValue)[k] << "\'" <<  ", outfile);" << "\n";	
				}
			}			
		} else if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_INTEGER"){
				myfile2 << "fprintf(outfile, \"%" << "d\"," << "MM[";
				myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
				myfile2 << "]";
				outputValType(tok_temp2);
				myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 11);
			}
		} else {
			error_handler.error(tok_temp2.line, 12);
		}
	} else if (strcmp(tok_temp.stringValue, "putchar") == 0){
		tok_temp2 = new_list.look_ahead_two_no_wrap();
		if (tok_temp2.type == "T_CHARVAL"){
			myfile2 << "putc(" << "\'" << (tok_temp2.stringValue) << "\'" <<  ", outfile);" << "\n";				
		} else if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_CHAR"){
				myfile2 << "fprintf(outfile, \"%" << "c\"," << "MM[";
				myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
				myfile2 << "]";
				outputValType(tok_temp2);
				myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 13);
			}
		} else {
			error_handler.error(tok_temp2.line, 14);
		}
	} if (strcmp(tok_temp.stringValue, "putfloat") == 0){
		tok_temp2 = new_list.look_ahead_two_no_wrap();

		if (tok_temp2.type == "T_NUMBERVAL"){
			if (isFloat(tok_temp2.stringValue)) {
				for (int k = 0; k < strLength(tok_temp2.stringValue); k++){
					myfile2 << "putc(" << "\'" << (tok_temp2.stringValue)[k] << "\'" <<  ", outfile);" << "\n";	
				} 
			} else {
				error_handler.error(tok_temp2.line, 15);
			}			
		} else if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_FLOAT"){
				myfile2 << "fprintf(outfile, \"%" << "f\"," << "MM[";
				myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
				myfile2 << "]";
				outputValType(tok_temp2);
				myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 16);
			}
		} else {
			error_handler.error(tok_temp2.line, 17);
		}
	} else if (strcmp(tok_temp.stringValue, "putbool") == 0){
		tok_temp2 = new_list.look_ahead_two_no_wrap();
		if (tok_temp2.type == "T_TRUE" || tok_temp2.type == "T_FALSE"){
			myfile2 << "putc(" << "\'";
			if (tok_temp2.type == "T_TRUE"){
				myfile2 << 1;
			} else if (tok_temp2.type == "T_FALSE"){
				myfile2 << 0;
			}
		 	myfile2 << "\'" <<  ", outfile);" << "\n";				
		} else if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_BOOL"){
				myfile2 << "fprintf(outfile, \"%" << "d\"," << "MM[";
				myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
				myfile2 << "]";
				outputValType(tok_temp2);
				myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 18);
			}
		} else {
			error_handler.error(tok_temp2.line, 19);
		}
	} else if (strcmp(tok_temp.stringValue, "putstring") == 0){
		tok_temp2 = new_list.look_ahead_two_no_wrap();
		if (tok_temp2.type == "T_STRINGVAL"){
			for (int k = 0; k < strLength(tok_temp2.stringValue); k++){
				myfile2 << "putc(" << "\'" << (tok_temp2.stringValue)[k] << "\'" <<  ", outfile);" << "\n";	
			}			
		} else if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_STRING"){
				for (int r = 0; r < 50; r++){
					myfile2 << "fprintf(outfile, \"%" << "c\"," << "MM[";
					myfile2 << r + sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
					myfile2 << "]";
					outputValType(tok_temp2);
					myfile2  << ");" << "\n";
				}
			} else {
				error_handler.error(tok_temp2.line, 20);
			}
		} else {
			error_handler.error(tok_temp2.line, 22);
		}
	} else if (strcmp(tok_temp.stringValue, "getinteger") == 0){	
		tok_temp2 = new_list.look_ahead_two_no_wrap();
		if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_INTEGER"){
					myfile2 << "fscanf(infile, \"%" << "d\"," << "&MM[";
					myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
					myfile2 << "]"; 
					outputValType(tok_temp2);
					myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 23);
			}
		} else {
			error_handler.error(tok_temp2.line, 24);
		}
	} else if (strcmp(tok_temp.stringValue, "getfloat") == 0){	
		tok_temp2 = new_list.look_ahead_two_no_wrap();
		if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_FLOAT"){
					myfile2 << "fscanf(infile, \"%" << "f\"," << "&MM[";
					myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
					myfile2 << "]"; 
					outputValType(tok_temp2);
					myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 25);
			}
		} else {
			error_handler.error(tok_temp2.line, 24);
		}
	} else if (strcmp(tok_temp.stringValue, "getchar") == 0){	
		tok_temp2 = new_list.look_ahead_two_no_wrap();
		if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_CHAR"){
					myfile2 << "fscanf(infile, \"%" << "c\"," << "&MM[";
					myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
					myfile2 << "]"; 
					outputValType(tok_temp2);
					myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 26);
			}
		} else {
			error_handler.error(tok_temp2.line, 24);
		}
	} else if (strcmp(tok_temp.stringValue, "getbool") == 0){	
		tok_temp2 = new_list.look_ahead_two_no_wrap();
		if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_BOOL"){
					myfile2 << "fscanf(infile, \"%" << "d\"," << "&MM[";
					myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
					myfile2 << "].int_val"; 
					
					myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 27);
			}
		} else {
			error_handler.error(tok_temp2.line, 24);
		}
	} else if (strcmp(tok_temp.stringValue, "getstring") == 0){	
		tok_temp2 = new_list.look_ahead_two_no_wrap();
		if (tok_temp2.type == "T_IDENTIFIER"){
			if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_STRING"){
					myfile2 << "fscanf(infile, \"";
					for (int r = 0; r < 50; r++){
						myfile2 << "%" << "c"; 
					} 
					myfile2 << "\"";
					for (int r = 0; r < 50; r++){
						myfile2 << ",&MM[";
						myfile2 << r + sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left;	
						myfile2 << "]"; 
						outputValType(tok_temp2);
					}
					myfile2  << ");" << "\n";
			} else {
				error_handler.error(tok_temp2.line, 26);
			}
		} else {
			error_handler.error(tok_temp2.line, 24);
		}
	}
}

void CodeGen::evalProcStatement(list temp_list2){

}



void CodeGen::generalFor(list temp_list2){
	tokens tok_temp; list assign_statement_list, expression_list;

	temp_list2.reset_pos();
	for (int h = 0; h < temp_list2.get_size(); h++){
		tok_temp = temp_list2.get_one();
		if (tok_temp.type == "T_LPARANTH"){		
			break;
		}
	}

	for (int h = 0; h < temp_list2.get_size(); h++){
		tok_temp = temp_list2.get_one();
		if (tok_temp.type == "T_SEMICOLON"){		
			break;
		} else {
			assign_statement_list.createnode(tok_temp);
		}
	}

	generalAssignStatement(assign_statement_list);

	goto_index_for = 0;
	myfile2 << "\n" << "FOR" << goto_index_for + seq_for + for_count << ":" << "\n" << "\t"; 

	for (int h = 0; h < temp_list2.get_size(); h++){
		tok_temp = temp_list2.get_one();
		if (tok_temp.type == "T_RPARANTH"){		
			break;
		} else {
			expression_list.createnode(tok_temp);
		}
	}

	generalExpression(expression_list);
	goto_index_for = 1;

	myfile2 << "\n" << "if (";
	outputInsideIfParenth(expression_list);
	myfile2 << ") goto FOR" << goto_index_for + seq_for + for_count;	
	myfile2 << ";" << "\n";

	goto_index_for = 2;

	myfile2 << "if (!";
	outputInsideIfParenth(expression_list);
	myfile2 << ") goto FOR" << goto_index_for + seq_for + for_count;	
	myfile2 << ";" << "\n";

	goto_index_for = 1;
	myfile2 << "\n" << "FOR" << goto_index_for + seq_for + for_count << ":" << "\n" << "\t"; 

	//assign_statement_list.display();
	//cout << 666 << endl;
	//expression_list.display();
}

void CodeGen::generalForEnd(){
	goto_index_for = 0;
	myfile2 << "\t" << "goto FOR" << goto_index_for + seq_for + for_count << ";";	

	goto_index_for = 2;
	myfile2 << "\n" << "FOR" << goto_index_for + seq_for + for_count << ": 1;" << "\n" << "\n"; 
}

void CodeGen::generalAssignStatement(list temp_list2){
	list expression_list, destination_list; tokens tok_temp; bool assign_passed = false;

	temp_list2.reset_pos();
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one();

		if (assign_passed){
			expression_list.createnode(tok_temp);
		} else if (tok_temp.type != "T_ASSIGN"){
			destination_list.createnode(tok_temp);
		}

		if (tok_temp.type == "T_ASSIGN"){
			assign_passed = true;
		}
	} 

	generalExpression(expression_list);
	evalDestination(destination_list, expression_list);
}

void CodeGen::evalDestination(list destination_list, list expression_list){
	list new_list; bool last_T_LBRACKET = false; tokens tok_temp2, tok_val_type, tok_temp; bool and_or_not = false; bool relation = false;

 //destination_list.display();

	// Trim the list from [] for single array access
	destination_list.reset_pos();
	for (int j = 0; j < destination_list.get_size(); j++){
		tok_temp2 = destination_list.get_one();
		if (destination_list.look_ahead_no_wrap().type == "T_LBRACKET") {
			tok_temp2.single_array_access = true;
			tok_temp2.index = atoi((destination_list.look_ahead_two_no_wrap()).stringValue);
			new_list.createnode(tok_temp2);
		} else if (last_T_LBRACKET){
			//Do nothing, we don't want the number in the []
		} else if (tok_temp2.type == "T_RBRACKET" || tok_temp2.type == "T_LBRACKET"){
			//Do nothing
		} else {
			new_list.createnode(tok_temp2);
		}

		if (tok_temp2.type == "T_LBRACKET" ){
			last_T_LBRACKET = true;
		} else {
			last_T_LBRACKET = false;
		}
	}


	new_list.reset_pos();
	tok_temp2 = new_list.get_one();
	if ((sym_table.returnValType(tok_temp2.stringValue)).is_array && !tok_temp2.single_array_access){
		expression_list.reset_pos();
		for (int j = 0; j < expression_list.get_size(); j++){
			tok_temp = expression_list.get_one();
	 		if (tok_temp.type == "T_NOT" || tok_temp.type == "T_OR" || tok_temp.type == "T_AND"){
	 			 and_or_not = true;
	 		} else if (isRelation(tok_temp)){
	 			relation = true;
	 		} else if (tok_temp.type == "T_NUMBERVAL" || tok_temp.type == "T_STRINGVAL" || tok_temp.type == "T_CHARVAL" 
	 												  || tok_temp.type == "T_IDENTIFIER" || tok_temp.type == "T_FALSE"
	 												  || tok_temp.type == "T_TRUE"){
	 			tok_val_type = tok_temp;
	 		}
		}

		expression_list.reset_pos();
		tok_temp = expression_list.get_one();
		if ((sym_table.returnValType(tok_temp2.stringValue)).str_val == "V_STRING"){
			myfile2 << "\n"; 
			for (int r = 0; r < 50; r++){
				myfile2 << "\t" << "MM[";
				myfile2 << r + sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left ;
				myfile2 << "]";
				outputValType(tok_temp2);
				myfile2 << "=";


				if (and_or_not){
					myfile2 << "R2[" << r << "].bool_val";
				} else if (relation){
					myfile2 << "R[" << r << "].bool_val";
				} else {
					myfile2 << "R[" << r << "]";
					outputValType(tok_val_type);
				}

				
				myfile2 << ";" << "\n";
			}
		} else {
			if ((sym_table.returnValType(tok_temp2.stringValue)).array_size == (sym_table.returnValType(tok_temp.stringValue)).array_size){

			} else {
				error_handler.error(tok_temp2.line, 22);
			}
		}
	} else {
		myfile2 << "MM[";
		myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - (sym_table.returnValType(tok_temp2.stringValue)).array_left ;
		myfile2 << "]";
		outputValType(tok_temp2);
		myfile2 << "=";

		expression_list.reset_pos();
		for (int j = 0; j < expression_list.get_size(); j++){
			tok_temp = expression_list.get_one();
	 		if (tok_temp.type == "T_NOT" || tok_temp.type == "T_OR" || tok_temp.type == "T_AND"){
	 			 and_or_not = true;
	 		} else if (isRelation(tok_temp)){
	 			relation = true;
	 		} else if (tok_temp.type == "T_NUMBERVAL" || tok_temp.type == "T_STRINGVAL" || tok_temp.type == "T_CHARVAL" 
	 												  || tok_temp.type == "T_IDENTIFIER" || tok_temp.type == "T_FALSE"
	 												  || tok_temp.type == "T_TRUE"){
	 			tok_val_type = tok_temp;
	 		}
		}
		if (and_or_not){
			myfile2 << "R2[0].bool_val";
		} else if (relation){
			myfile2 << "R[0].bool_val";
		} else {
			myfile2 << "R[0]";
			outputValType(tok_val_type);
		}

		
		myfile2 << ";" << "\n" << "\n";
	} 

}

void CodeGen::generalIfElse(list temp_list2){
	tokens tok_temp; list new_list;

	temp_list2.reset_pos();
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one();
		if (tok_temp.type == "T_ELSE"){					
		} else {
			new_list.createnode(tok_temp);		
		}
	}

	goto_index = 2;
	myfile2 << "\t" << "goto IF" << goto_index + if_count + seq_if << ";" << "\n" << "\n";
	myfile2 << "IF" << goto_index - 1 + if_count + seq_if << ":" << "\n";
	end_if_before = false;
}

void CodeGen::generalIf(list temp_list2){
	tokens tok_temp; list temp_list3;


	// Get the Lparanthesis tok
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one();
		if (tok_temp.type == "T_LPARANTH"){		
			break;
		}
	}

	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one();
		if (tok_temp.type == "T_RPARANTH"){		
			break;
		} else {
			temp_list3.createnode(tok_temp);		
		}
	}

	generalExpression(temp_list3);
	goto_index = 0;
	myfile2 << "\n" << "if (";
	outputInsideIfParenth(temp_list3);
	myfile2 << ") goto IF" << goto_index + if_count + seq_if;	
	myfile2 << ";" << "\n";
	//goto_index = goto_index + 1;

	myfile2 << "if (!";
	outputInsideIfParenth(temp_list3);
	myfile2 << ") goto IF" << goto_index + 1 + if_count + seq_if << ";" << "\n" << "\n" ;
	
	myfile2 << "IF" << goto_index + if_count + seq_if << ":" << "\n";
	goto_index = 1;
	end_if_before = false;
	//myfile2 << "IF" << goto_index << ":" << "\n";

	
}

void CodeGen::generalIfEnd(){
	
	if (goto_index == 1 ){
		myfile2 << "IF" << goto_index + if_count + seq_if << ": 1;" << "\n";
		goto_index = 2;
	} //else 
	if (end_if_before){
		//myfile2 << "IF" << goto_index + if_count - 1 << ": 1;" << "\n";
	}

	myfile2 << "IF" << goto_index + if_count + seq_if << ": 1;" << "\n";
	end_if_before = true;
}

void CodeGen::outputInsideIfParenth(list temp_list3){
	tokens tok_temp, tok_val_type; bool and_or_not = false;  bool relation = false;
	temp_list3.reset_pos();
	for (int j = 0; j < temp_list3.get_size(); j++){
		tok_temp = temp_list3.get_one();
 		if (tok_temp.type == "T_NOT" || tok_temp.type == "T_OR" || tok_temp.type == "T_AND"){
 			 and_or_not = true;
 		} else if (isRelation(tok_temp)){
 			relation = true;
 		} else if (tok_temp.type == "T_NUMBERVAL" || tok_temp.type == "T_STRINGVAL" || tok_temp.type == "T_CHARVAL" 
 												  || tok_temp.type == "T_IDENTIFIER"){
 			tok_val_type = tok_temp;
 		}
	}
	if (and_or_not){
		myfile2 << "R2[0].bool_val";
	} else if (relation){
		myfile2 << "R[0].bool_val";
	} else {
		myfile2 << "R[0]";
		outputValType(tok_val_type);
	}
}

void CodeGen::generalExpression(list temp_list2){
	tokens tok_temp2; int count = 1; list new_list; bool last_T_LBRACKET = false; list empty; 
	int num_of_expressions = 0; tokens old_tok_AndOr, new_tok_AndOr; int expression_index = 1;
	bool tok_not_detected = false;
	
	// This for loop will disect the list first, gets rid of single array access
	temp_list2.reset_pos();
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp2 = temp_list2.get_one();
		if (temp_list2.look_ahead_no_wrap().type == "T_LBRACKET") {
			tok_temp2.single_array_access = true;
			tok_temp2.index = atoi((temp_list2.look_ahead_two_no_wrap()).stringValue);
			new_list.createnode(tok_temp2);
		} else if (last_T_LBRACKET){
			//Do nothing, we don't want the number in the []
		} else if (tok_temp2.type == "T_RBRACKET" || tok_temp2.type == "T_LBRACKET"){
			//Do nothing
		} else {
			new_list.createnode(tok_temp2);
		}

		if (tok_temp2.type == "T_LBRACKET" ){
			last_T_LBRACKET = true;
		} else {
			last_T_LBRACKET = false;
		}
	}

	new_list.reset_pos(); list expression_list;
	for (int j = 0; j < new_list.get_size(); j++){
		tok_temp2 = new_list.get_one();
		
		if (tok_temp2.type == "T_NOT"){
			tok_not_detected = true;
		} else if (tok_temp2.type == "T_AND" || tok_temp2.type == "T_OR" || j == new_list.get_size() - 1){
			num_of_expressions = num_of_expressions + 1;
			old_tok_AndOr = new_tok_AndOr;
			new_tok_AndOr = tok_temp2;
 
			if (j == new_list.get_size() - 1){
				expression_list.createnode(tok_temp2);	
			}
		
			evalExpression(expression_list);
			expression_list = empty;
 			
 			if ((count == 1 && num_of_expressions <= 1) && j != new_list.get_size() - 1){
				if (tok_not_detected){
					myfile2 << "R2[0].bool_val=!R[0].bool_val;" << "\n" << "\t";
				} else {
					myfile2 << "R2[0].bool_val=R[0].bool_val;" << "\n" << "\t";	
				}
				
			}	

			if (num_of_expressions > 1){
				
				if (count == 1){
					myfile2 << "R2[0].bool_val=R2[0].bool_val";
					outputAndOr(old_tok_AndOr);
					//if (tok_not_detected){
					//	myfile2 << "!R[0].bool_val;" << "\n" << "\t";
					//} else {
						myfile2 << "R[0].bool_val;" << "\n" << "\t";
					//}
					count = count + 1;
				} else {
					//myfile2 << "R2[1].bool_val=R[0].bool_val;" << "\n" << "\t";

					myfile2 << "R2[0].bool_val=R2[0].bool_val";
					outputAndOr(old_tok_AndOr);
					//if (tok_not_detected){
					//	myfile2 << "!R[0].bool_val;" << "\n" << "\t";
					//} else {
						myfile2 << "R[0].bool_val;" << "\n" << "\t";
					//}
				}
			}

			tok_not_detected = false;
			expression_index = expression_index + 1;
		} else {
			expression_list.createnode(tok_temp2);
		}
	}
}

void CodeGen::outputAndOr(tokens and_or){
	if (and_or.type == "T_AND"){
		myfile2 << "&&";
	} else if(and_or.type == "T_OR"){
		myfile2 << "||";
	} else if(and_or.type == "T_NOT"){
		myfile2 << "!";
	}
}

int CodeGen::evalExpression(list expression_list){
	bool relation = false; list relation_list; list empty; tokens tok_temp2;
	int num_of_relations = 0; int index = 0, count = 1; 
	tokens new_tok_relation, old_tok_relation; int bool_index = 0; int random_counter = 0;

	// Relation checker
	expression_list.reset_pos();
	for (int j = 0; j < expression_list.get_size(); j++){
		tok_temp2 = expression_list.get_one();
		if (isRelation(tok_temp2) || j == expression_list.get_size() - 1){
			
			//if (isRelation(tok_temp2)){
				num_of_relations = num_of_relations + 1;
				old_tok_relation = new_tok_relation;
				new_tok_relation = tok_temp2;
			//} 
			
			if (j == expression_list.get_size() - 1){
				relation_list.createnode(tok_temp2);	
			}
 //relation_list.display();
 //cout << "----" << endl;
 //cout << index << endl;
			index = evalRelation(relation_list, index);
 //cout << index << endl;
			relation_list = empty;

			if (num_of_relations > 1){


 				// If it is a single entity, no arrays
				if (first_tok_val_type.type == "T_NUMBERVAL"
							|| first_tok_val_type.type == "T_TRUE" || first_tok_val_type.type == "T_FALSE"
							|| first_tok_val_type.type == "T_CHARVAL"
							|| (first_tok_val_type.type == "T_IDENTIFIER" && sym_table.returnValType(first_tok_val_type.stringValue).is_array) && first_tok_val_type.single_array_access
							|| (first_tok_val_type.type == "T_IDENTIFIER" && !(sym_table.returnValType(first_tok_val_type.stringValue).is_array))){
					if (count == 1){
						myfile2 << "R[0].bool_val=R[" << second_reg_index << "]";
						outputValType(second_tok_val_type);
						outputRelation(old_tok_relation);
						myfile2 << "R[" << first_reg_index << "]";
						outputValType(first_tok_val_type);
						myfile2 << ";" << "\n";
						count = count + 1;
					} else {
						myfile2 << "R[0].bool_val=R[0].bool_val";
						outputRelation(old_tok_relation);
						myfile2 << "R[" << first_reg_index << "]";
						outputValType(first_tok_val_type);
						myfile2 << ";" << "\n";
					}
									// String
				} else if (first_tok_val_type.type == "T_STRINGVAL"){
					if (count == 1){

 						if (second_tok_val_type.type == "T_STRINGVAL"){
							for (int k = 0; k < 50; k++){
								myfile2 << "R[" << k << "].bool_val=R[" << second_array_index_start + k << "]";
								outputValType(second_tok_val_type); 
								outputRelation(old_tok_relation);
								myfile2 << "R[" << first_array_index_start + k << "]";
								outputValType(first_tok_val_type);
								myfile2 << ";" << "\n" << "\t" << "\t";
							}
							for (int k = 49; k > 0; k--){
								myfile2 << "R[" << k - 1 << "].bool_val=R[" << k << "].bool_val&&"; 
								myfile2 << "R[" << k - 1 << "].bool_val";
								myfile2 << ";" << "\n" << "\t";
							}
							bool_index = bool_index + 50;
						} else if ((second_tok_val_type.type == "T_IDENTIFIER" && sym_table.returnValType(second_tok_val_type.stringValue).is_array) && !second_tok_val_type.single_array_access){
							random_counter = 0;
							for (int k = 0; k < 50; k++){								
								if (k < sym_table.returnValType(second_tok_val_type.stringValue).array_size){									
									myfile2 << "R[" << k << "].bool_val=R[" << second_array_index_start + k << "]";
									outputValType(second_tok_val_type);
									outputRelation(old_tok_relation);
									myfile2 << "R[" << first_array_index_start + k << "]";
									outputValType(first_tok_val_type);
									myfile2 << ";" << "\n" << "\t" << "\t";	
									random_counter = random_counter + 1;
								} else {
									if (strLength(first_tok_val_type.stringValue) != sym_table.returnValType(second_tok_val_type.stringValue).array_size){
										myfile2 << "R[" << k << "].bool_val=R[" << second_array_index_start + k << "]";
										outputValType(second_tok_val_type);
										outputRelation(old_tok_relation);
										myfile2 << "\'" << " " << "\'";
										myfile2 << ";" << "\n" << "\t" << "\t";
										random_counter = random_counter + 1;
									}	
								}
							}
							if (strLength(first_tok_val_type.stringValue) != sym_table.returnValType(second_tok_val_type.stringValue).array_size){
								bool_index = bool_index + 50;
							} else {
								bool_index = bool_index + strLength(first_tok_val_type.stringValue);
							}

							for (int k = random_counter - 1; k > 0; k--){
								myfile2 << "R[" << k - 1 << "].bool_val=R[" << k << "].bool_val&&"; 
								myfile2 << "R[" << k - 1 << "].bool_val";
								myfile2 << ";" << "\n" << "\t";
							}
						}
						count = count + 1;
					} else {
						if (second_tok_val_type.type == "T_STRINGVAL"){
							for (int k = 0; k < 50; k++){
								myfile2 << "R[" << k + bool_index << "].bool_val=R[" << second_array_index_start + k << "]";
								outputValType(second_tok_val_type); 
								outputRelation(old_tok_relation);
								myfile2 << "R[" << first_array_index_start + k << "]";
								outputValType(first_tok_val_type);
								myfile2 << ";" << "\n" << "\t" << "\t";
							}
							for (int k = 49; k > 0; k--){
								myfile2 << "R[" << k - 1 + bool_index << "].bool_val=R[" << k + bool_index << "].bool_val&&"; 
								myfile2 << "R[" << k - 1 + bool_index << "].bool_val";
								myfile2 << ";" << "\n" << "\t";
							}

							myfile2 << "R[0].bool_val=R[0].bool_val&&R[" << bool_index << "].bool_val;";
							myfile2 << ";" << "\n" << "\n" << "\t";
							bool_index = bool_index + 50;
						} else if ((second_tok_val_type.type == "T_IDENTIFIER" && sym_table.returnValType(second_tok_val_type.stringValue).is_array) && !second_tok_val_type.single_array_access){
							random_counter = 0;
							for (int k = 0; k < 50; k++){								
								if (k < sym_table.returnValType(second_tok_val_type.stringValue).array_size){									
									myfile2 << "R[" << k + bool_index << "].bool_val=R[" << second_array_index_start + k << "]";
									outputValType(second_tok_val_type);
									outputRelation(old_tok_relation);
									myfile2 << "R[" << first_array_index_start + k << "]";
									outputValType(first_tok_val_type);
									myfile2 << ";" << "\n" << "\t" << "\t";	
									random_counter = random_counter + 1;
								} else {
									if (strLength(first_tok_val_type.stringValue) != sym_table.returnValType(second_tok_val_type.stringValue).array_size){
										myfile2 << "R[" << k + bool_index << "].bool_val=R[" << second_array_index_start + k << "]";
										outputValType(second_tok_val_type);
										outputRelation(old_tok_relation);
										myfile2 << "\'" << " " << "\'";
										myfile2 << ";" << "\n" << "\t" << "\t";
										random_counter = random_counter + 1;
									}	
								}
							}							

							for (int k = random_counter - 1; k > 0; k--){
								myfile2 << "R[" << k - 1 + bool_index << "].bool_val=R[" << k << "].bool_val&&"; 
								myfile2 << "R[" << k - 1 + bool_index << "].bool_val";
								myfile2 << ";" << "\n" << "\t";
							}

							myfile2 << "R[0].bool_val=R[0].bool_val&&R[" << bool_index << "].bool_val;";
							myfile2 << ";" << "\n" << "\n" << "\t";

							if (strLength(first_tok_val_type.stringValue) != sym_table.returnValType(second_tok_val_type.stringValue).array_size){
								bool_index = bool_index + 50;
							} else {
								bool_index = bool_index + strLength(first_tok_val_type.stringValue);
							}
						}
					}
				// Array
				} else if ((first_tok_val_type.type == "T_IDENTIFIER" && sym_table.returnValType(first_tok_val_type.stringValue).is_array) && !first_tok_val_type.single_array_access){
					if (count == 1){
						if (second_tok_val_type.type == "T_STRINGVAL"){
 							random_counter = 0;
							for (int k = 0; k < 50; k++){								
								if (k < sym_table.returnValType(first_tok_val_type.stringValue).array_size){									
									myfile2 << "R[" << k << "].bool_val=R[" << second_array_index_start + k << "]";
									outputValType(second_tok_val_type);
									outputRelation(old_tok_relation);
									myfile2 << "R[" << first_array_index_start + k << "]";
									outputValType(first_tok_val_type);
									myfile2 << ";" << "\n" << "\t" << "\t";	
									random_counter = random_counter + 1;
								} else {
									if (strLength(second_tok_val_type.stringValue) != sym_table.returnValType(first_tok_val_type.stringValue).array_size){
										myfile2 << "R[" << k << "].bool_val=R[" << second_array_index_start + k << "]";
										outputValType(second_tok_val_type);
										outputRelation(old_tok_relation);
										myfile2 << "\'" << " " << "\'";
										myfile2 << ";" << "\n" << "\t" << "\t";
										random_counter = random_counter + 1;
									}	
								}
							}
							if (strLength(second_tok_val_type.stringValue) != sym_table.returnValType(first_tok_val_type.stringValue).array_size){
								bool_index = bool_index + 50;
							} else {
								bool_index = bool_index + strLength(second_tok_val_type.stringValue);
							}

							for (int k = random_counter - 1; k > 0; k--){
								myfile2 << "R[" << k - 1 << "].bool_val=R[" << k << "].bool_val&&"; 
								myfile2 << "R[" << k - 1 << "].bool_val";
								myfile2 << ";" << "\n" << "\t";
							}
							
						} else if ((second_tok_val_type.type == "T_IDENTIFIER" && sym_table.returnValType(second_tok_val_type.stringValue).is_array) && !second_tok_val_type.single_array_access){
							random_counter = 0;
							for (int k = 0; k < sym_table.returnValType(second_tok_val_type.stringValue).array_size; k++){																								
								myfile2 << "R[" << k << "].bool_val=R[" << second_array_index_start + k << "]";
								outputValType(second_tok_val_type);
								outputRelation(old_tok_relation);
								myfile2 << "R[" << first_array_index_start + k << "]";
								outputValType(first_tok_val_type);
								myfile2 << ";" << "\n" << "\t" << "\t";	
								random_counter = random_counter + 1;							
							}					

							for (int k = random_counter - 1; k > 0; k--){
								myfile2 << "R[" << k - 1 << "].bool_val=R[" << k << "].bool_val&&"; 
								myfile2 << "R[" << k - 1 << "].bool_val";
								myfile2 << ";" << "\n" << "\t";
							}

							bool_index = bool_index + sym_table.returnValType(second_tok_val_type.stringValue).array_size;
						}
						count = count + 1;
					} else {
						if (second_tok_val_type.type == "T_STRINGVAL"){
 							random_counter = 0;
							for (int k = 0; k < 50; k++){								
								if (k < sym_table.returnValType(first_tok_val_type.stringValue).array_size){									
									myfile2 << "R[" << k + bool_index << "].bool_val=R[" << second_array_index_start + k << "]";
									outputValType(second_tok_val_type);
									outputRelation(old_tok_relation);
									myfile2 << "R[" << first_array_index_start + k << "]";
									outputValType(first_tok_val_type);
									myfile2 << ";" << "\n" << "\t" << "\t";	
									random_counter = random_counter + 1;
								} else {
									if (strLength(second_tok_val_type.stringValue) != sym_table.returnValType(first_tok_val_type.stringValue).array_size){
										myfile2 << "R[" << k + bool_index << "].bool_val=R[" << second_array_index_start + k << "]";
										outputValType(second_tok_val_type);
										outputRelation(old_tok_relation);
										myfile2 << "\'" << " " << "\'";
										myfile2 << ";" << "\n" << "\t" << "\t";
										random_counter = random_counter + 1;
									}	
								}
							}

							for (int k = random_counter - 1; k > 0; k--){
								myfile2 << "R[" << k - 1 + bool_index<< "].bool_val=R[" << k + bool_index << "].bool_val&&"; 
								myfile2 << "R[" << k - 1 + bool_index << "].bool_val";
								myfile2 << ";" << "\n" << "\t";
							}

							myfile2 << "R[0].bool_val=R[0].bool_val&&R[" << bool_index << "].bool_val;";
							myfile2 << ";" << "\n" << "\n" << "\t";
							
							if (strLength(second_tok_val_type.stringValue) != sym_table.returnValType(first_tok_val_type.stringValue).array_size){
								bool_index = bool_index + 50;
							} else {
								bool_index = bool_index + strLength(second_tok_val_type.stringValue);
							}
						} else if ((second_tok_val_type.type == "T_IDENTIFIER" && sym_table.returnValType(second_tok_val_type.stringValue).is_array) && !second_tok_val_type.single_array_access){
							random_counter = 0;
							for (int k = 0; k < sym_table.returnValType(second_tok_val_type.stringValue).array_size; k++){																								
								myfile2 << "R[" << k + bool_index << "].bool_val=R[" << second_array_index_start + k << "]";
								outputValType(second_tok_val_type);
								outputRelation(old_tok_relation);
								myfile2 << "R[" << first_array_index_start + k << "]";
								outputValType(first_tok_val_type);
								myfile2 << ";" << "\n" << "\t" << "\t";	
								random_counter = random_counter + 1;							
							}					
							for (int k = random_counter - 1; k > 0; k--){
								myfile2 << "R[" << k - 1 + bool_index << "].bool_val=R[" << k + bool_index << "].bool_val&&"; 
								myfile2 << "R[" << k - 1 + bool_index << "].bool_val";
								myfile2 << ";" << "\n" << "\t";
							}

							myfile2 << "R[0].bool_val=R[0].bool_val&&R[" << bool_index << "].bool_val;";
							myfile2 << ";" << "\n" << "\n" << "\t";

							bool_index = bool_index + sym_table.returnValType(second_tok_val_type.stringValue).array_size;
						}
						count = count + 1;
					}
				}
			} 
		} else {
			relation_list.createnode(tok_temp2);
		}
	}
	return 0;
}

void CodeGen::outputRelation(tokens relation){
	if (relation.type == "T_EQUALTO") { myfile2 << "==";
	} else if (relation.type == "T_NOTEQUALTO") { myfile2 << "!=";
	} else if (relation.type == "T_LESSTHAN") { myfile2 << "<";
	} else if (relation.type == "T_LESSTHANEQUAL") { myfile2 << "<=";
 	} else if (relation.type == "T_GREATERTHAN") { myfile2 << ">";
 	} else if (relation.type == "T_GREATERTHANEQUAL") { myfile2 << ">="; }
}

int CodeGen::evalRelation(list relation_list, int prority_index){
	int count = 1; int index = 0; tokens tok_temp2; int left, right;
	int index_return = 0;
	relation_list.reset_pos();

 //relation_list.display();
 //cout << "====" << endl;

	for (int j = 0; j < relation_list.get_size(); j++){
		tok_temp2 = relation_list.get_one();
		if (count == 1){
			second_tok_val_type = first_tok_val_type;
			first_tok_val_type = tok_temp2;
			if (tok_temp2.type == "T_IDENTIFIER"){
	
				if (sym_table.returnValType(tok_temp2.stringValue).is_array){
					left = (sym_table.returnValType(tok_temp2.stringValue)).array_left;
					right = (sym_table.returnValType(tok_temp2.stringValue)).array_right;
	//cout << left << " " << right << endl;
					if (tok_temp2.single_array_access){
						second_reg_index = first_reg_index;
						first_reg_index = index + prority_index;
						myfile2 << "R[" << index + prority_index<< "]";
						outputValType(tok_temp2);
						myfile2 << "=";
						myfile2 << "MM[";

						myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + tok_temp2.index - left;
						myfile2 << "]";
						outputValType(tok_temp2);
						myfile2 << ";" << "\n" << "\t";
					} else {
						second_array_index_start = first_array_index_start;
						first_array_index_start = prority_index;
						for (int k = 0; k < (sym_table.returnValType(tok_temp2.stringValue)).array_size; k++){
							myfile2 << "R[" << k + prority_index << "]";
							outputValType(tok_temp2);
							myfile2 << "=";
							myfile2 << "MM[";
							myfile2 << sym_table.getMMIndex(tok_temp2.stringValue) + k  ;
							myfile2 << "]";
							outputValType(tok_temp2);
							myfile2 << ";" << "\n" << "\t";
						}
					}
				} else {
					second_reg_index = first_reg_index;
					first_reg_index = index + prority_index;	
					myfile2 << "R[" << (index + prority_index) << "]";
					outputValType(tok_temp2);
					myfile2 << "=";
					myfile2 << "MM[";
					myfile2 << sym_table.getMMIndex(tok_temp2.stringValue);
					myfile2 << "]";
					outputValType(tok_temp2);
					myfile2 << ";" << "\n" << "\t";
				}
			} else if(tok_temp2.type == "T_STRINGVAL"){
				second_array_index_start = first_array_index_start;
				first_array_index_start = prority_index;		
				for (int k = 0; k < strLength(tok_temp2.stringValue); k++){
					myfile2 << "R[" << k + prority_index<< "]";
					outputValType(tok_temp2);
					myfile2 << "=";
					myfile2 << "\'";
					myfile2 << tok_temp2.stringValue[k];
					myfile2 << "\'"; 
					myfile2 << ";" << "\n" << "\t";
				}
				for (int k = strLength(tok_temp2.stringValue); k < 50; k++){
					myfile2 << "R[" << k + prority_index<< "]";
					outputValType(tok_temp2);
					myfile2 << "=";
					myfile2 << "\'" << " ";
					myfile2 << "\'"; 
					myfile2 << ";" << "\n" << "\t";
				}
				
			} else {
				second_reg_index = first_reg_index;
				first_reg_index = index + prority_index;
				myfile2 << "R[" << index + prority_index << "]";
				outputValType(tok_temp2);
				myfile2 << "=";
			}

			count = count + 1;
		} else {
			if (tok_temp2.type != "T_TRUE" ){
			if (tok_temp2.type != "T_FALSE" ){
			if (tok_temp2.type != "T_CHARVAL" ){
            if (tok_temp2.type != "T_STRINGVAL" ){
            if (tok_temp2.type != "T_NUMBERVAL" ){
            if (tok_temp2.type != "T_IDENTIFIER" ){
        	if (tok_temp2.type != "T_SEMICOLON" ){
				myfile2 << "R[" << index + prority_index << "]";
				outputValType(relation_list.look_ahead());
				myfile2 << "=R[" << index + prority_index << "]";
				outputValType(relation_list.look_ahead());
			}}}}}}}
			count = count + 1;
		}

		outputMainNew(relation_list, tok_temp2, count, index);

		if (tok_temp2.type == "T_IDENTIFIER" && sym_table.returnValType(tok_temp2.stringValue).is_array 
											 && !(tok_temp2.single_array_access) && count > 2){
			if (index < sym_table.returnValType(tok_temp2.stringValue).array_size - 1){
				relation_list.goBackOne();
				relation_list.goBackOne();
				j = j - 2;
			}
			index = index + 1;
			if (index >= (sym_table.returnValType(tok_temp2.stringValue).array_size) ){
				index = 0;
			}

		} else if(tok_temp2.type == "T_STRINGVAL" && count > 2){
			if (index  < 49 ){
				relation_list.goBackOne();
				relation_list.goBackOne();
				j = j - 2;
			}
			index = index + 1;
	//cout << index << endl;
			if (index > 49 ){
				index = 0;
			}
		}

		// Index evaluation
		if (tok_temp2.type == "T_STRINGVAL" 
						|| (tok_temp2.type == "T_IDENTIFIER" && !(tok_temp2.single_array_access) && sym_table.returnValType(tok_temp2.stringValue).is_array)){
 
			if (tok_temp2.type == "T_IDENTIFIER" && index_return < sym_table.returnValType(tok_temp2.stringValue).array_size ){
				index_return = sym_table.returnValType(tok_temp2.stringValue).array_size + prority_index;
			} else if (tok_temp2.type == "T_STRINGVAL" && index_return < 50){

				index_return = 50 + prority_index;
			}
		} else if (tok_temp2.type == "T_IDENTIFIER" && !(sym_table.returnValType(tok_temp2.stringValue).is_array) ) {
			if (index_return < sym_table.returnValType(tok_temp2.stringValue).array_size){
				index_return = sym_table.returnValType(tok_temp2.stringValue).array_size + prority_index;
 
			}
		} else if (tok_temp2.type == "T_NUMBERVAL" || tok_temp2.type == "T_CHARVAL") {
			if (index_return < sym_table.returnValType(tok_temp2.stringValue).array_size){
				index_return = sym_table.returnValType(tok_temp2.stringValue).array_size + prority_index;
			}
		} else if ( (tok_temp2.type == "T_IDENTIFIER" && (tok_temp2.single_array_access) && sym_table.returnValType(tok_temp2.stringValue).is_array)){
			if (index_return < 1){
				index_return = 1 + prority_index;
			}
		} else if (tok_temp2.type == "T_TRUE" || tok_temp2.type == "T_FALSE"){
			if (index_return < 1){
				index_return = 1 + prority_index;
			}
		}
	}

	

	return index_return;
}

list CodeGen::outputMainNew(list temp_list2,tokens tok_temp, int count, int index){
	if (tok_temp.type == "T_IDENTIFIER" && count > 2){		
		myfile2 << "MM[";
		myfile2 << sym_table.getMMIndex(tok_temp.stringValue) + index;
		myfile2 << "]";

		//temp_list2 = arrayOutput(temp_list2, tok_temp, reg_index);
		outputValType(tok_temp);
		myfile2 << ";" << "\n" << "\t";
	} else if (tok_temp.type == "T_LPARANTH"){
	} else if (tok_temp.type == "T_RPARANTH"){		
	} else if (tok_temp.type == "T_LBRACKET"){
	} else if (tok_temp.type == "T_RBRACKET"){
	} else if (tok_temp.type == "T_ADD"){	  		myfile2 << "+";
	} else if (tok_temp.type == "T_MINUS"){	  		myfile2 << "-";
	} else if (tok_temp.type == "T_MULT"){	  		myfile2 << "*";
	} else if (tok_temp.type == "T_DIVIDE"){  		myfile2 << "/";
	} else if (tok_temp.type == "T_AND"){     		//myfile2 << "&&";
	} else if (tok_temp.type == "T_OR"){      		//myfile2 << "||";
	} else if (tok_temp.type == "T_NOT"){     		//myfile2 << "!";
	} else if (tok_temp.type == "T_ASSIGN"){  		myfile2 << "=";
	} else if (tok_temp.type == "T_LESSTHAN"){		//myfile2 << "<";
	} else if (tok_temp.type == "T_LESSTHANEQUAL"){	//myfile2 << "<=";
	} else if (tok_temp.type == "T_GREATERTHAN"){	//myfile2 << ">";
	} else if (tok_temp.type == "T_GREATERTHANEQUAL"){//myfile2 << ">=";
	} else if (tok_temp.type == "T_NOTEQUALTO"){	//myfile2 << "!=";
	} else if (tok_temp.type == "T_EQUALTO"){		//myfile2 << "==";
	} else if (tok_temp.type == "T_TRUE"){			myfile2 << "true" ; myfile2 << ";" << "\n" << "\t";
	} else if (tok_temp.type == "T_FALSE"){			myfile2 << "false"; myfile2 << ";" << "\n" << "\t"; 
	} else if (tok_temp.type == "T_CHARVAL"){		myfile2 << "\'" << tok_temp.stringValue << "\'";
		myfile2 << ";" << "\n" << "\t";
	} else if (tok_temp.type == "T_NUMBERVAL"){		myfile2 << tok_temp.stringValue;
		myfile2 << ";" << "\n" << "\t";
	} else if (tok_temp.type == "T_STRINGVAL"  && count > 2){	
		if (strLength(tok_temp.stringValue) <= index){
			myfile2 << "\'" << " " << "\'" ;
			myfile2 << ";" << "\n" << "\t";
		} else {
			myfile2 << "\'" << tok_temp.stringValue[index] << "\'" ;
			myfile2 << ";" << "\n" << "\t";
		}		
	} 
		
	return temp_list2;
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

void CodeGen::generalStatements(list temp_list2, tokens tok_val_type){
	int reg_index; bool string_detected = false; bool go_back = false; bool str_ident = false;
	tokens tok_temp2; int bs1, bs2, bs3;
	tokens tok_temp;  int count = 1; int count2 = 1;	

	count = 1; count2 = 1; int str_array_index = 0;
	for (int j = 0; j < temp_list2.get_size(); j++){
		tok_temp = temp_list2.get_one(); 
		//count = count + 1;
		if (count == 1){
			//if (tok_temp.type == "T_IDENTIFIER"){
				if (tok_temp.type == "T_STRINGVAL"){
					reg_index = 0;
					string_detected = true;
					for (int f = 0; f < strLength(tok_temp.stringValue); f++){
						
						myfile2 << "R[" << f << "]";
						outputValType(tok_temp);
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
						outputValType(tok_temp);
						myfile2 << "=";

						myfile2 << "MM[";
						myfile2 << sym_table.getMMIndex(tok_temp.stringValue) + f;
						myfile2 << "]";
						outputValType(tok_temp);
						myfile2 << ";" << "\n" << "\t";
						
						reg_index++;
					}
				} else {
					myfile2 << "R[0]";

					outputValType(tok_temp);
					myfile2 << "=";
				}
				count = count + 1;
			//}
		} else {
			//If the next token is a STRING VALUE
			if (temp_list2.look_ahead_no_wrap().type == "T_STRINGVAL"){
				myfile2  << "R[" << str_array_index << "]";
				outputValType(temp_list2.look_ahead());
				myfile2 << "=R[" << str_array_index << "]";
				outputValType(temp_list2.look_ahead());
				go_back = true;

			} else if (temp_list2.look_ahead_no_wrap().type == "T_IDENTIFIER" && !(temp_list2.look_ahead_no_wrap().single_array_access) && (sym_table.returnValType(temp_list2.look_ahead_no_wrap().stringValue)).is_array){
				myfile2  << "R[" << str_array_index << "]";
				outputValType(temp_list2.look_ahead());
				myfile2 << "=R[" << str_array_index << "]";
				outputValType(temp_list2.look_ahead());
				go_back = true;
				str_ident = true;

			} 
			else if (tok_temp.type != "T_CHARVAL" ){
            if (tok_temp.type != "T_STRINGVAL" ){
            if (tok_temp.type != "T_NUMBERVAL" ){
            if (tok_temp.type != "T_SEMICOLON" ){
            if (tok_temp.type != "T_IDENTIFIER" ){

                    myfile2  << "R[0]";
                    outputValType((temp_list2.look_ahead()));
                    myfile2 << "=R[0]";
                    outputValType((temp_list2.look_ahead()));
                    count = count + 1;
            }}}}}
		}
		temp_list2 = outputMain(temp_list2, tok_temp, tok_temp, reg_index, str_array_index, go_back);
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



list CodeGen::outputMain(list temp_list2, tokens tok_temp, tokens tok_val_type, int reg_index, int str_array_index, bool go_back){
	if (tok_temp.type == "T_IDENTIFIER" ){
		
		temp_list2 = arrayOutput(temp_list2, tok_temp, reg_index);

		outputValType(tok_temp);
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
				outputValType(tok_temp);
				
				myfile2 << "=R[" << f << "]";
				outputValType(tok_temp);
				myfile2 << ";" << "\n";
				if (f == reg_index - 1){}
				else {myfile2 << "\t";}
			}
		} else {
			outputValType((temp_list2.look_back()));
			myfile2 << "=R[0]";
			outputValType((temp_list2.look_back()));
			myfile2 << ";" << "\n";
			
		}
		
	}
	return temp_list2;
}

bool CodeGen::checkOp(tokens tok_temp){
	if (tok_temp.type == "T_ADD"){	  		return true;
	} else if (tok_temp.type == "T_MINUS"){	  		return true;
	} else if (tok_temp.type == "T_MULT"){	  		return true;
	} else if (tok_temp.type == "T_DIVIDE"){  		return true;
	} else if (tok_temp.type == "T_AND"){     		return true;
	} else if (tok_temp.type == "T_OR"){      		return true;
	} else if (tok_temp.type == "T_NOT"){     		return true;
	} else if (tok_temp.type == "T_ASSIGN"){  		return true;
	} else if (tok_temp.type == "T_LESSTHAN"){		return true;
	} else if (tok_temp.type == "T_LESSTHANEQUAL"){	return true;
	} else if (tok_temp.type == "T_GREATERTHAN"){	return true;
	} else if (tok_temp.type == "T_GREATERTHANEQUAL"){return true;
	} else if (tok_temp.type == "T_NOTEQUALTO"){	return true;
	} else if (tok_temp.type == "T_EQUALTO"){		return true;
	} else { 										return false;}
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





void CodeGen::display(){
	for (int i = 0; i < code_gen_order.size(); i++){
		cout << "\t" << code_gen_order[i].getCG() << endl;
		code_gen_order[i].display();
	}
}

void CodeGen::set_flags(tokens tok){
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

	if (tok.type == "T_INTEGER"){ prev_TT_int = true; } else { prev_TT_int = false;	}
	if (tok.type == "T_STRING"){ prev_TT_str = true; } else { prev_TT_str = false;	}
	if (tok.type == "T_BOOL"){ prev_TT_bool = true; } else { prev_TT_bool = false;	}
	if (tok.type == "T_FLOAT"){ prev_TT_float = true; } else { prev_TT_float = false;	}
	if (tok.type == "T_CHAR"){ prev_TT_char = true; } else { prev_TT_char = false;	}
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

bool CodeGen::isRelation(tokens tok_temp2){
	if (tok_temp2.type == "T_LESSTHAN" || tok_temp2.type == "T_LESSTHANEQUAL" 
									   || tok_temp2.type == "T_GREATERTHAN" || tok_temp2.type == "T_GREATERTHANEQUAL"
									   || tok_temp2.type == "T_NOTEQUALTO" || tok_temp2.type == "T_EQUALTO"){
		return true;
	} else {
		return false;
	}
}