#ifndef CODEGEN_H
#define CODEGEN_H

#include "Symbol.h"
#include "ParseTree.h"
#include "List.h"

class CodeGen{
	private:
    	bool prog_start, prog_begin, prog_end, proc_start, proc_begin, proc_end;
    	bool prev_TT_prog, prev_TT_ident, prev_TT_is, prev_TT_begin, prev_TT_end;
    	bool prev_TT_proc, prev_TT_semi, prog_declare;
    	std::vector<list> code_gen_order;
    	list temp_list;

    public:
    	CodeGen();
    	void init(tokens tok, Symbol sym);	
    	void set_flags(tokens tok);
    	void printCode();
    	void display();
};

#endif