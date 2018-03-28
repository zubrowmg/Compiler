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

		if (tok.type == "T_SEMICOLON"){
			code_gen_order.push_back(temp_list);
			temp_list = empty;
		}
	}


	

	if (prog_end && tok.type == "T_ENDFILE"){
		printCode();
	}
	set_flags(tok);
}

void CodeGen::output(list temp_list){
	tokens tok_temp;

	for (int j = 0; j < temp_list.get_size(); j++){
		tok_temp = temp_list.get_one(); 
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
//cout << MM_Index << endl;
			MM_Index = sym_table.setMMIndex(tok_temp.stringValue, MM_Index);
		} else if (tok_temp.type == "T_SEMICOLON"){
			myfile << ";" << "\n";
		}
	}

}

void CodeGen::output2(list temp_list){
	tokens tok_temp;

	myfile2 << "\t";

	for (int j = 0; j < temp_list.get_size(); j++){
		tok_temp = temp_list.get_one(); 
		
		if (tok_temp.type == "T_IDENTIFIER"){
			myfile2 << "MM[" << sym_table.getMMIndex(tok_temp.stringValue) << "]";

			if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_INTEGER"){
				myfile2 << ".int_val";
			} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_BOOL"){
				myfile2 << ".bool_val";
			} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_CHAR"){
				myfile2 << ".char_val";
			} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_STRING"){
				myfile2 << ".string_val";
			} else if ((sym_table.returnValType(tok_temp.stringValue)).str_val == "V_FLOAT"){
				myfile2 << ".float_val";
			}
			
//cout << tok_temp.stringValue << sym_table.getMMIndex(tok_temp.stringValue) << endl;
		}

		else if (tok_temp.type == "T_LPARANTH"){
			//myfile << "(";
		} else if (tok_temp.type == "T_RPARANTH"){
			//myfile << ")";
		} else if (tok_temp.type == "T_LBRACKET"){
			//myfile << "[";
		} else if (tok_temp.type == "T_RBRACKET"){
			//myfile << "]";
		} else if (tok_temp.type == "T_ADD"){
			myfile2 << "+";
		} else if (tok_temp.type == "T_ASSIGN"){
			myfile2 << "=";
		} else if (tok_temp.type == "T_NUMBERVAL"){
			myfile2 << tok_temp.stringValue;
		}

		else if (tok_temp.type == "T_IDENTIFIER"){
			//myfile << " " << tok_temp.stringValue;
		} else if (tok_temp.type == "T_SEMICOLON"){
			myfile2 << ";" << "\n";
		}
	}
	
}
void CodeGen::printCode()
{

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
		<< "\t" << "char string_val[20];" << "\n"
		<< "\t" << "float float_val;" << "\n"
	 << "};" << "\n" << "\n";
	myfile2 << "union val MM[100];" << "\n";

	
	myfile << "\n" << "int main(){" << "\n";
	myfile2 << "\n" << "int main(){" << "\n";
	
	for (int i = 0; i < code_gen_order.size(); i++){
		code_gen_order[i].reset_pos();
		if (code_gen_order[i].getCG() == "prog_begin"){ 
			//output(code_gen_order[i]);	
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