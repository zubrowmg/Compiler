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
	prog_start = false; 
	prog_begin = false; 
	prog_end = false; 
	proc_start = false; 
	proc_begin = false; 
	//proc_end = false;

	prev_TT_prog = false; 
	prev_TT_proc = false; 
	prev_TT_ident = false;
	prev_TT_is = false;
	prev_TT_begin = false;
	prev_TT_end = false;
	prev_TT_semi = false;

	prog_declare = false;
}

void CodeGen::init(tokens tok, Symbol sym)
{
	list empty; 

	//-------- Initialized Program Space Variables --------//
	if (prog_declare){
		temp_list.setCG("prog_declare");
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

void CodeGen::printCode()
{
	ofstream myfile;
	myfile.open ("CodeGen.c");

	tokens tok_temp;

	for (int i = 0; i < code_gen_order.size(); i++){
		code_gen_order[i].reset_pos();
		if (code_gen_order[i].getCG() == "prog_declare"){ 
			
			for (int j = 0; j < code_gen_order[i].get_size(); j++){
				tok_temp = code_gen_order[i].get_one(); 
				if (tok_temp.type == "T_CHAR"){
					myfile << "char";
				} else if (tok_temp.type == "T_INTEGER"){
					myfile << "int";
				} 


	//add Integers and strings and shit


				else if (tok_temp.type == "T_IDENTIFIER"){
					myfile << " " << tok_temp.stringValue;
				} else if (tok_temp.type == "T_SEMICOLON"){
					myfile << ";" << "\n";
				}
			}
		}
	}

	
  	//myfile << "Writing this to a file.\n";
  	myfile.close();
}

void CodeGen::display()
{
	for (int i = 0; i < code_gen_order.size(); i++){
		code_gen_order[i].display();
	}
}

void CodeGen::set_flags(tokens tok)
{
	//cout << tok.type << endl;

	if (tok.type == "T_PROGRAM" && prev_TT_end && prog_begin ) { proc_begin = false; prog_end = true;}
	if (tok.type == "T_BEGIN" && prog_start && !proc_begin) { prog_start = false; prog_begin = true; prog_declare = false;}

	if (tok.type == "T_PROCEDURE" && prev_TT_end && proc_begin) { proc_begin = false; prog_declare = true; }
	if (tok.type == "T_END" ) { prev_TT_end = true; } else { prev_TT_end = false; }
	if (tok.type == "T_BEGIN" && prog_start && proc_start) { proc_begin = true; proc_start = false;}

	if (tok.type == "T_IDENTIFIER" && prev_TT_proc && prog_start){ proc_start = true; prog_declare = false; }  
	if (tok.type == "T_PROCEDURE"){ prev_TT_proc = true; } else { prev_TT_proc = false; }

	if (tok.type == "T_IS" && prev_TT_ident){ prog_start = true; prog_declare = true;}
	if (tok.type == "T_IDENTIFIER" && prev_TT_prog){ prev_TT_ident = true; } else { prev_TT_ident = false; }
	if (tok.type == "T_PROGRAM"){ prev_TT_prog = true; } else { prev_TT_prog = false; }
}