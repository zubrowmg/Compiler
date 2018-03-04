#ifndef PROCEDURENODE_H
#define PROCEDURENODE_H

#include <map>

struct symbolNode{
    int line_num; std::string value; std::string type;
};


class ProcedureNode
{
    private:
        std::map <std::string, std::string> table;
        ProcedureNode *next;
        std::string name;

    public:
        ProcedureNode();
        void insertValue(std::string key);
        void setName(std::string input);
        std::string getName();
        void setNext(ProcedureNode *input);
        void printTable();
        ProcedureNode *getNext();
        bool newCheck(std::string input);
        bool check(std::string input);
        void error(std::string input);


};

#endif

#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <map>

class Symbol
{
    private:
        std::map <std::string, std::string> global;
        ProcedureNode *pos;
        ProcedureNode *tail;
        ProcedureNode *head;
        int size;
        std::vector<std::string> order;
        int order_size; std::string current_proc;
        bool prev2_TT_glob, prev_TT_glob, prev_TT_proc, prev_TT_end, prev_TT_int;
        bool prev_TT_prog, prev_TT_flt, prev_TT_str, prev_TT_bool, prev_TT_char;  
    public:
        Symbol();
        void insertGlobal(std::string key);
        void insertValue(std::string key);

        void newProc(std::string input);
        void endProc();

        void printGlobal();
        void printTable();
        void printAll();

        void resetPos();
        void nextPos();

        bool procCheck(std::string input);
        bool newCheck(std::string input);
        bool newCheckGlobal(std::string input);
        bool check(std::string input);
        
        void error(std::string input, int i);

        void init(std::string token, std::string value);
        
        bool find();
};

#endif

