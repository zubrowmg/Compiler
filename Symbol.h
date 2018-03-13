#ifndef PROCEDURENODE_H
#define PROCEDURENODE_H

#include <map>

struct symbolNode{
    int line_num; std::string type; std::string proc;
    bool is_array; int array_left, array_right, array_size;

    std::string str_val; 
    int int_val;
    symbolNode()
    {
        is_array = false; array_size = 1; array_left = 0; array_right = 0;
    }
};

struct TCNode{
    std::string ident, type;  
    bool array_single_access;
    TCNode()
    {
        //array_single_access  = false;
    }
};

class ProcedureNode
{
    private:
        std::map <std::string, symbolNode> table;
        ProcedureNode *next;
        std::string name;

    public:
        ProcedureNode();
        void insertValue(std::string key, symbolNode sym);
        void setName(std::string input);
        std::string getName();
        void setNext(ProcedureNode *input);
        void printTable();
        ProcedureNode *getNext();
        bool newCheck(std::string input);
        bool check(std::string input);
        void error(std::string input);

        symbolNode returnValType(std::string ident);
        void modify(std::string ident);
        void modify(std::string ident, std::string num, char c);
};

#endif

#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <map>
#include <string>

#include "Error.h"

class Symbol
{
    private:
        std::map <std::string, symbolNode> global; 
        ProcedureNode *pos;
        ProcedureNode *tail;
        ProcedureNode *head;
        int size;
        std::vector<std::string> order;
        int order_size; std::string current_proc;
        bool prev2_TT_glob, prev_TT_glob, prev_TT_proc, prev_TT_end, prev_TT_int;
        bool prev_TT_prog, prev_TT_flt, prev_TT_str, prev_TT_bool, prev_TT_char;

        bool prev2_TT_int;
        bool prev2_TT_flt, prev2_TT_str, prev2_TT_bool, prev2_TT_char;

        bool prev_TT_SEMICOLON, prev_TT_LB, prev_TT_IDENT, prev2_TT_LB;  
        std::vector<TCNode> type_check; std::vector<TCNode> type_check_AS;
        std::string last_ident;
        Error sym_error_handler;
        int left_bound;

    public:
        Symbol();
        void insertGlobal(std::string key, symbolNode sym); 
        void insertValue(std::string key, symbolNode sym);

        void newProc(std::string input);
        void endProc();

        void printGlobal(); 
        void printTable();
        void printAll();

        void resetPos();
        void nextPos();

        bool procCheck(std::string input);
        bool newCheck(std::string input, symbolNode sym);
        bool newCheckGlobal(std::string input, symbolNode sym); 
        bool check(std::string input, symbolNode sym);
        
        void error(std::string input, int i);

        void init(std::string token, std::string value, symbolNode sym); 
        
        bool find();
        symbolNode returnValType(std::string ident);
        void clearTC();
        bool insertTC(std::string ident, std::string TT);

        void modify(std::string ident);
        void modify(std::string ident, std::string num, char c);

        void clearTC_AS();
        bool insertTC_AS(std::string ident, std::string TT);

        bool modifyTC(std::string ident, std::string TT);
        bool modifyTC_AS(std::string ident, std::string TT);
        //bool MC(std::string ident, std::string TT);
        bool MC();
        bool MC_AS();
};

#endif

