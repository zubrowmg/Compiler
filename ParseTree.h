#ifndef ParseNode_H
#define ParseNode_H

struct tokens{
	std::string type;
	
	char stringValue[256]; 			// holds lexeme value if string/identifier
 	int intValue; 					// holds lexeme value if integer
 	double doubleValue; 			// holds lexeme value if double

 	int line;
	char assigned_value[256];

    bool single_array_access;	 	 
    tokens(){
        single_array_access = false;
    }
};

class ParseNode
{
    private:
        ParseNode *left, *right, *middle, *root;
        std::string token, ident; int option; bool done, leaf, prime;

        
    public:
        ParseNode(void);
        
        ParseNode *getLeft();
        ParseNode *getMiddle();
        ParseNode *getRight();
        ParseNode *getRoot();
        std::string getToken();
        int getOption();
        bool getNodeStatus();
        bool getLeaf();
        bool getPrime();

        void setPrime(bool input);
        void setLeft(ParseNode *input);
        void setMiddle(ParseNode *input);
        void setRight(ParseNode *input);
        void setRoot(ParseNode *input);
        void setToken(std::string input);
        void setToken(std::string input, std::string input2);
        void setOption(int input);
        void setNodeStatus(bool input);
        void setLeaf(bool input);

        std::string getIdent();
};

#endif


#ifndef ParseTree_H
#define ParseTree_H

#include <vector>
#include "Symbol.h"
class ParseTree 
{
    private:

    	ParseNode *temp_node;
    	ParseNode *new_node;
        ParseNode *null_node;
  		ParseNode *pos;
        ParseNode *treeroot;
        bool is_legit;
        bool get_new_pos;

        bool expression_flag;
        bool expression_done;
        int expression_count;
        bool assignment_flag;
        bool assignment_done;
        int assignment_count;
        bool prog_body_flag, name3_flag;

        Symbol sym;

    public:
    	ParseTree(void);
    	void createnode(std::string input);
    	void createnode_2(std::string input);
    	void createnode_3();
    	
    	void re_findStart();
    	ParseNode *findStart(ParseNode *input);
    	
    	ParseNode *printTree(ParseNode *input);
    	void printTree();

    	ParseNode *getTreeRoot();
    	ParseNode *getPos();
    	ParseNode *checkStatus(ParseNode *input);
    	void setNewNode(std::string type8, std::string ident);
        void clearNewNode();
    	void deleteAll(ParseNode *input);
    	void backUp(); 
    	
    	bool getLegit();
    	void clearPos();
    	void clearTempNode();
    	
        
    	void newTempNode();
    	void newNewNode();
        

        ParseNode *travel(ParseNode *input);

        bool getExpressFlag();
        bool getAssignmentFlag();
        bool getProgBodyFlag();

        void setSym(Symbol input_sym);
};

#endif

