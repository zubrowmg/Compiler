#ifndef CODEGEN_H
#define CODEGEN_H

#include <iostream>
#include <iomanip>
#include <fstream>

#include "Symbol.h"
#include "ParseTree.h"
#include "List.h"

using namespace std;

class CodeGen{
	private:
    	bool prog_start, prog_begin, prog_end, proc_start, proc_begin, proc_end;
    	bool prev_TT_prog, prev_TT_ident, prev_TT_is, prev_TT_begin, prev_TT_end;
    	bool prev_TT_proc, prev_TT_semi, prog_declare, prev_TT_ident2, prev_TT_LPAR;
    	std::vector<list> code_gen_order;
    	list temp_list;
    	ofstream myfile; ofstream myfile2;
    	int MM_Index;
		std::vector<int> MM;    
		Symbol sym_table;	

    public:
    	CodeGen();
    	void init(tokens tok, Symbol sym);	
    	void set_flags(tokens tok);
    	void printCode();
    	void display();
    	void output(list input); 
    	void output2(list input);
};

#endif