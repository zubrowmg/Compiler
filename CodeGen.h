#ifndef CODEGEN_H
#define CODEGEN_H

#include <iostream>
#include <iomanip>
#include <fstream>

#include "Symbol.h"
#include "ParseTree.h"
#include "List.h"

using namespace std;

struct expNode
{
	int new_position;
};

class CodeGen{
	private:
    	bool prog_start, prog_begin, prog_end, proc_start, proc_begin, proc_end;
    	bool prev_TT_prog, prev_TT_ident, prev_TT_is, prev_TT_begin, prev_TT_end;
    	bool prev_TT_proc, prev_TT_semi, prog_declare, prev_TT_ident2, prev_TT_LPAR;
        bool prev_TT_int, prev_TT_bool, prev_TT_str, prev_TT_char, prev_TT_float;
    	std::vector<list> code_gen_order;
    	list temp_list;
    	ofstream myfile; ofstream myfile2;
    	int MM_Index;
		std::vector<int> MM;    
		Symbol sym_table;
		//std::std::vector<list> exp_rewrite;	
		//int assign_option;
		int current_array_index;
        //bool last_TT_Then;

    public:
    	CodeGen();
    	void init(tokens tok, Symbol sym);	
    	void set_flags(tokens tok);
    	void printCode();
    	void display();
    	void output(list input); 
    	void output2(list input);

    	void outputValType(tokens tok_temp);
    	list arrayOutput(list input_list, tokens tok_temp, int reg_index);
    	list outputMain(list temp_list2, tokens tok_temp, tokens tok_val_type, int reg_index, int str_array_index, bool go_back);
        list outputMainNew(list temp_list2, tokens tok_temp, int count, int index);

    	int strLength(char str[256]);
        bool isFloat(char str[256]);
        bool isRelation(tokens tok);
        bool checkOp(tokens tok_input);
    	void SinArAChecker(list input);
        
        void generalStatements(list input, tokens tok_input);
        void generalIf(list input);
        void generalEnd();
        void generalExpression(list temp_list2);
        void evalExpression(list expression_list);
        int evalRelation(list expression_list, int index);
};

#endif