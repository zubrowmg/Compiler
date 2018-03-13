#include <iostream>
#include <iomanip>
#include <vector>

#include "ParseTree.h"
#include "List.h"
#include "Symbol.h"


using namespace std;

/*------------------------------------------------------
	ParseNode Class
------------------------------------------------------*/
ParseNode::ParseNode(void)
{
	left = NULL;
	right = NULL;
	middle = NULL;
	root = NULL;
	option = 0;
	done = false;
	leaf = true;
	token = "";
	prime = false;
	ident = "";

}

ParseNode *ParseNode::getLeft(){return left;}
ParseNode *ParseNode::getRight(){return right;}
ParseNode *ParseNode::getMiddle(){return middle;}
ParseNode *ParseNode::getRoot(){return root;}
std::string ParseNode::getToken(){return token;}
int ParseNode::getOption(){return option;}
bool ParseNode::getNodeStatus(){return done;}
bool ParseNode::getLeaf(){return leaf;}
bool ParseNode::getPrime(){return prime;}

void ParseNode::setPrime(bool input){prime = input;}
void ParseNode::setLeft(ParseNode *input){left = input;}
void ParseNode::setMiddle(ParseNode *input){middle = input;}
void ParseNode::setRight(ParseNode *input){right = input;}
void ParseNode::setRoot(ParseNode *input){root = input;}
void ParseNode::setToken(std::string token_input){token = token_input;}
void ParseNode::setToken(std::string token_input, std::string input_ident){token = token_input; ident = input_ident;}
void ParseNode::setOption(int input){option = input;}
void ParseNode::setNodeStatus(bool input){done = input;}
void ParseNode::setLeaf(bool input){leaf = input;}
std::string ParseNode::getIdent(){return ident;}

/*------------------------------------------------------
	ParseTree Class
------------------------------------------------------*/
ParseTree::ParseTree(void)
{
	ParseNode *temp = new ParseNode;
	temp_node = NULL;
	treeroot = NULL;
	pos = NULL;
	new_node = NULL;
	null_node = NULL;
	temp->setToken("<program>"); // program
	treeroot = temp;
	temp = NULL;
	is_legit = true;
	get_new_pos = true;

	expression_flag = false;
	expression_done = false;
	expression_count = 0;
	assignment_flag = false;
	assignment_done = false;
	assignment_count = 0;
	prog_body_flag = false;
	name3_flag = false;
}

bool ParseTree::getLegit(){return is_legit;}
ParseNode *ParseTree::getTreeRoot(){return treeroot;}
ParseNode *ParseTree::getPos(){return pos;}
void ParseTree::setNewNode(std::string type, std::string ident){ ParseNode *temp = new ParseNode; new_node = temp; new_node->setToken(type, ident);}
void ParseTree::clearPos(){ParseNode *temp = new ParseNode; pos = temp; get_new_pos = true;}
void ParseTree::clearNewNode(){ParseNode *temp = new ParseNode; new_node = temp;}
void ParseTree::clearTempNode(){ParseNode *temp = new ParseNode; temp_node = temp;}
void ParseTree::printTree(){ printTree(treeroot);}
bool ParseTree::getExpressFlag(){ return expression_flag;}
bool ParseTree::getAssignmentFlag(){ return assignment_flag;}
bool ParseTree::getProgBodyFlag(){ return prog_body_flag;}
void ParseTree::setSym(Symbol input_sym){ sym = input_sym; }

ParseNode *ParseTree::printTree(ParseNode *node)
{
	if (!(node->getLeft() == NULL)){printTree(node->getLeft());}
	if (!(node->getMiddle() == NULL)){printTree(node->getMiddle());}
	if (!(node->getRight() == NULL)){printTree(node->getRight());}
	if (node->getLeaf()){cout << node->getToken() << endl;}
	
	return node->getRoot();	
}

ParseNode *ParseTree::findStart(ParseNode *start)
{	
	if (start->getToken() == "<expression>" ){
 		expression_flag = true; expression_count++;
 	}
 	if (start->getToken() == "<statement1>" ){
 		assignment_flag = true; assignment_count++;
 	}

	if (!(start->getLeft() == NULL)){findStart(start->getLeft());}
	if (!(start->getMiddle() == NULL)){findStart(start->getMiddle());}
	if (!(start->getRight() == NULL)){findStart(start->getRight());}	
	if (start->getNodeStatus() == false){
		if (get_new_pos){
			pos = start;
			get_new_pos = false;	
		}
	}
	
	if (start->getToken() == "<expression>" ){expression_count--;}
 	if (start->getToken() == "<expression>" && get_new_pos && expression_count == 0){
 		expression_flag = false;
 	}

 	if (start->getToken() == "<statement1>" ){assignment_count--;}
 	if (start->getToken() == "<statement1>" && get_new_pos && assignment_count == 0){
 		assignment_flag = false;
 	}

	return start->getRoot();			
}

void ParseTree::re_findStart()
{
	pos->setNodeStatus(true);
	clearPos();
	findStart(treeroot);
	createnode_3();
}

void ParseTree::deleteAll(ParseNode *node)
{
	if (!(node->getLeft() == NULL)){deleteAll(node->getLeft());}
	if (!(node->getMiddle() == NULL)){deleteAll(node->getMiddle());}
	if (!(node->getRight() == NULL)){deleteAll(node->getRight());}

	node->setRoot(NULL);

	delete node; node = NULL;	
}

void ParseTree::backUp(){
	pos = pos->getRoot();

	if (pos->getLeft() != NULL){ deleteAll(pos->getLeft()); pos->setLeft(NULL);}
	if (pos->getMiddle() != NULL){ deleteAll(pos->getMiddle()); pos->setMiddle(NULL);} 
	if (pos->getRight() != NULL){ deleteAll(pos->getRight()); pos->setRight(NULL);}

	createnode_3();
}

void ParseTree::createnode_3(){
/*--------------------------------------------------  
	<program>
--------------------------------------------------*/	
	if (pos->getToken() == "<program>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){													
			temp_node->setToken("<program_header>"); newTempNode();
			temp_node->setToken("<program_body>"); newTempNode();
			temp_node->setToken("<T_PERIOD>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else {

			is_legit = false;
			//backUp();
		}
	}
/*--------------------------------------------------  
	<program_header>
--------------------------------------------------*/	
	else if (pos->getToken() == "<program_header>"){ pos->setOption((pos->getOption()) + 1);
		if (pos->getOption() == 1){															
			temp_node->setToken("<T_PROGRAM>"); newTempNode();
			temp_node->setToken("<identifier>"); newTempNode();
			temp_node->setToken("<T_IS>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<program_body>
--------------------------------------------------*/	
	else if (pos->getToken() == "<program_body>"){ pos->setOption((pos->getOption()) + 1);
prog_body_flag = true;
		if (pos->getOption() == 1){															
			temp_node->setToken("<program_body1>"); newTempNode();
			temp_node->setToken("<program_body2>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<program_body1>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){															
			temp_node->setToken("<program_body3>"); newTempNode();
			temp_node->setToken("<T_BEGIN>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<program_body3>"){ pos->setOption((pos->getOption()) + 1);
		if (pos->getOption() == 1){															
			temp_node->setToken("<declaration>"); newTempNode();
			temp_node->setToken("<T_SEMICOLON>"); newTempNode();
			temp_node->setToken("<program_body3>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else if (pos->getOption() == 2){															
			temp_node->setToken("<NULL>"); newTempNode();
			
			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<program_body2>"){ pos->setOption((pos->getOption()) + 1);
		
		if (pos->getOption() == 1){															
			temp_node->setToken("<program_body4>"); newTempNode();
			temp_node->setToken("<T_END>"); newTempNode();
			temp_node->setToken("<T_PROGRAM>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<program_body4>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){															
			temp_node->setToken("<statement>"); newTempNode();
			temp_node->setToken("<T_SEMICOLON>"); newTempNode();
			temp_node->setToken("<program_body4>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else if (pos->getOption() == 2){															
			temp_node->setToken("<NULL>"); newTempNode();
			
			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<declaration>
--------------------------------------------------*/	
	else if (pos->getToken() == "<declaration>"){ pos->setOption((pos->getOption()) + 1);
		if (pos->getOption() == 1){															
			temp_node->setToken("<T_GLOBAL>"); newTempNode();
			temp_node->setToken("<declaration1>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else if (pos->getOption() == 2){															
			temp_node->setToken("<declaration1>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<declaration1>"){ pos->setOption((pos->getOption()) + 1);
		if (pos->getOption() == 1){															
			temp_node->setToken("<procedure_declaration>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else if (pos->getOption() == 2){															
			temp_node->setToken("<variable_declaration>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		}else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<procedure_declaration>
--------------------------------------------------*/	
	else if (pos->getToken() == "<procedure_declaration>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){									
			temp_node->setToken("<procedure_header>"); newTempNode();
			temp_node->setToken("<procedure_body>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else {
			backUp();
		}
	} 
/*--------------------------------------------------  
	<procedure_header>
--------------------------------------------------*/
	else if (pos->getToken() == "<procedure_header>"){ pos->setOption((pos->getOption()) + 1);		
		if (pos->getOption() == 1){						
			temp_node->setToken("<T_PROCEDURE>"); newTempNode();		
			temp_node->setToken("<identifier>"); newTempNode();
			temp_node->setToken("<procedure_header1>"); newTempNode();
			
			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<procedure_header1>"){ pos->setOption((pos->getOption()) + 1);		
		if (pos->getOption() == 1){						
			temp_node->setToken("<T_LPARANTH>"); newTempNode();			
			temp_node->setToken("<procedure_header2>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		}else {
			backUp();
		}
	}  else if (pos->getToken() == "<procedure_header2>"){ pos->setOption((pos->getOption()) + 1);		

		if (pos->getOption() == 1){						
			temp_node->setToken("<paramenter_list>"); newTempNode();		
			temp_node->setToken("<T_RPARANTH>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 2){	

			temp_node->setToken("<T_RPARANTH>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		}else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<paramenter_list>
--------------------------------------------------*/
	else if (pos->getToken() == "<paramenter_list>"){ pos->setOption((pos->getOption()) + 1);		

		if (pos->getOption() == 1){						
			temp_node->setToken("<paramenter>");			
			newTempNode();
			
			temp_node->setPrime(true);
			temp_node->setToken("<paramenter_list'>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<paramenter_list'>"){ pos->setOption((pos->getOption()) + 1);			
		if (pos->getOption() == 1){								
			temp_node->setToken("<T_COMMA>"); newTempNode();
			temp_node->setToken("<paramenter>"); newTempNode();	
			temp_node->setToken("<paramenter_list'>");
			temp_node->setPrime(true); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		}else if (pos->getOption() == 2){										
			temp_node->setToken("<NULL>"); newTempNode();
		
			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<paramenter>
--------------------------------------------------*/
	else if (pos->getToken() == "<paramenter>"){ pos->setOption((pos->getOption()) + 1);		
		if (pos->getOption() == 1){						
			temp_node->setToken("<variable_declaration>"); newTempNode();				
			temp_node->setToken("<paramenter1>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<paramenter1>"){ pos->setOption((pos->getOption()) + 1);		
		if (pos->getOption() == 1){											
			temp_node->setToken("<T_IN>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 2){						
			temp_node->setToken("<T_OUT>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 3){										
			temp_node->setToken("<T_INOUT>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		}else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<procedure_body>
--------------------------------------------------*/	
	else if (pos->getToken() == "<procedure_body>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){															
			temp_node->setToken("<procedure_body1>"); newTempNode();

			temp_node->setToken("<procedure_body2>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<procedure_body1>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){															
			temp_node->setToken("<procedure_body3>"); newTempNode();

			temp_node->setToken("<T_BEGIN>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<procedure_body3>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){															
			temp_node->setToken("<declaration>"); newTempNode();

			temp_node->setToken("<T_SEMICOLON>"); newTempNode();

			temp_node->setToken("<procedure_body3>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else if (pos->getOption() == 2){															
			temp_node->setToken("<NULL>"); newTempNode();
			
			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<procedure_body2>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){															
			temp_node->setToken("<procedure_body4>"); newTempNode();

			temp_node->setToken("<T_END>"); newTempNode();

			temp_node->setToken("<T_PROCEDURE>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<procedure_body4>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){															
			temp_node->setToken("<statement>"); newTempNode();

			temp_node->setToken("<T_SEMICOLON>"); newTempNode();

			temp_node->setToken("<procedure_body4>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else if (pos->getOption() == 2){															
			temp_node->setToken("<NULL>"); newTempNode();
			
			pos = pos->getLeft();
			createnode_3();			
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<variable_declaration>
--------------------------------------------------*/
	else if (pos->getToken() == "<variable_declaration>"){ pos->setOption((pos->getOption()) + 1);		

		if (pos->getOption() == 1){						
			temp_node->setToken("<type_mark>"); newTempNode();
					
			temp_node->setToken("<identifier>"); newTempNode();

			temp_node->setToken("<variable_declaration1>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<variable_declaration1>"){ pos->setOption((pos->getOption()) + 1);		

		if (pos->getOption() == 1){						
			temp_node->setToken("<T_LBRACKET>"); newTempNode();	

			temp_node->setToken("<variable_declaration2>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 2){						
			temp_node->setToken("<NULL>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<variable_declaration2>"){ pos->setOption((pos->getOption()) + 1);		

		if (pos->getOption() == 1){						
			temp_node->setToken("<variable_declaration3>"); newTempNode();
					
			temp_node->setToken("<T_RBRACKET>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<variable_declaration3>"){ pos->setOption((pos->getOption()) + 1);		

		if (pos->getOption() == 1){						
			temp_node->setToken("<lower_bound>"); newTempNode();
			
			temp_node->setToken("<T_COLON>"); newTempNode();

			temp_node->setToken("<upper_bound>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	}		
/*--------------------------------------------------  
	<type_mark>
--------------------------------------------------*/	
	else if (pos->getToken() == "<type_mark>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){										
			temp_node->setToken("<T_INTEGER>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else if (pos->getOption() == 2){						
			temp_node->setToken("<T_FLOAT>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else if (pos->getOption() == 3){						
			temp_node->setToken("<T_STRING>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else if (pos->getOption() == 4){						
			temp_node->setToken("<T_BOOL>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else if (pos->getOption() == 5){						
			temp_node->setToken("<T_CHAR>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<lower_bound>
--------------------------------------------------*/
	 else if (pos->getToken() == "<lower_bound>"){ pos->setOption((pos->getOption()) + 1);

		if (pos->getOption() == 1){													
			temp_node->setToken("<number>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<upper_bound>
--------------------------------------------------*/
	else if (pos->getToken() == "<upper_bound>"){ pos->setOption((pos->getOption()) + 1);	

		if (pos->getOption() == 1){												
			temp_node->setToken("<number>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else {
			backUp();
		}
	} 
/*--------------------------------------------------  
	<statement>
--------------------------------------------------*/
	else if (pos->getToken() == "<statement>"){ pos->setOption((pos->getOption()) + 1);	
	
		if (pos->getOption() == 1){													
			temp_node->setToken("<if_statement>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else if (pos->getOption() == 2){													
			temp_node->setToken("<loop_statement>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else if (pos->getOption() == 3){												
			temp_node->setToken("<return_statement>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else if (pos->getOption() == 4){
			temp_node->setToken("<statement1>"); newTempNode();													

			pos = pos->getLeft();
			createnode_3();		
		}  else {
			backUp();
		}
	} else if (pos->getToken() == "<statement1>"){ pos->setOption((pos->getOption()) + 1);	
		
		if (pos->getOption() == 1){	
			if ( !(sym.procCheck(new_node->getIdent())) ){
				assignment_flag = true;
			}
			
			temp_node->setToken("<identifier>"); newTempNode();												
			temp_node->setToken("<statement2>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else {
			//assignment_flag = false;
			backUp();
		}
	} else if (pos->getToken() == "<statement2>"){ pos->setOption((pos->getOption()) + 1);	
	
		if (pos->getOption() == 1){	
			//assignment_flag = true;
			temp_node->setToken("<assignment_statement>"); newTempNode();						

			pos = pos->getLeft();
			createnode_3();		
		} else if (pos->getOption() == 2){	
			assignment_flag = false;
			temp_node->setToken("<procedure_call>"); newTempNode();						

			pos = pos->getLeft();
			createnode_3();		
		} else {
			backUp();
		}
	} /*else if (pos->getToken() == "<statement3>"){ pos->setOption((pos->getOption()) + 1);	
	
		if (pos->getOption() == 1){	
			temp_node->setToken("<identifier>"); newTempNode();	
			temp_node->setToken("<assignment_statement>"); newTempNode();						

			pos = pos->getLeft();
			createnode_3();		
		} else {
			backUp();
		}
	} */
/*--------------------------------------------------  
	<procedure_call>
--------------------------------------------------*/
	else if (pos->getToken() == "<procedure_call>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){													
			//temp_node->setToken("<identifier>"); newTempNode();
			temp_node->setToken("<procedure_call1>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else {
			backUp();
		} 
	} else if (pos->getToken() == "<procedure_call1>"){ pos->setOption((pos->getOption()) + 1);
					
		if (pos->getOption() == 1){																
			temp_node->setToken("<T_LPARANTH>"); newTempNode();

			temp_node->setToken("<procedure_call2>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<procedure_call2>"){ pos->setOption((pos->getOption()) + 1);				

		if (pos->getOption() == 1){													
			temp_node->setToken("<T_RPARANTH>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else if (pos->getOption() == 2){													
			temp_node->setToken("<argument_list>"); newTempNode();

			temp_node->setToken("<T_RPARANTH>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<assignment_statement>
--------------------------------------------------*/
 	else if (pos->getToken() == "<assignment_statement>"){ pos->setOption((pos->getOption()) + 1);			
			

		if (pos->getOption() == 1){						
			temp_node->setToken("<destination>"); newTempNode();

			temp_node->setToken("<assignment_statement1>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			
			backUp();
		}
	} else if (pos->getToken() == "<assignment_statement1>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){										
				temp_node->setToken("<T_ASSIGN>"); newTempNode();

				temp_node->setToken("<expression>"); newTempNode();

				pos = pos->getLeft();
				createnode_3();				
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<destination>
--------------------------------------------------*/
	 else if (pos->getToken() == "<destination>"){ pos->setOption((pos->getOption()) + 1);					
		if (pos->getOption() == 1){						
			//temp_node->setToken("<identifier>"); newTempNode();
			temp_node->setToken("<destination1>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<destination1>"){ pos->setOption((pos->getOption()) + 1);					
		if (pos->getOption() == 1){																				
			temp_node->setToken("<T_LBRACKET>"); newTempNode();
			temp_node->setToken("<expression>"); newTempNode();

			temp_node->setToken("<T_RBRACKET>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else if (pos->getOption() == 2){							
			temp_node->setToken("<NULL>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		}else {
			backUp();
		}
	} /*else if (pos->getToken() == "<destination2>"){ pos->setOption((pos->getOption()) + 1);					
		if (pos->getOption() == 2){													
			temp_node->setToken("<expression>"); newTempNode();

			temp_node->setToken("<T_RBRACKET>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else {
			backUp();
		}
	}*/
/*--------------------------------------------------  
	<if_statement>
--------------------------------------------------*/
	 else if (pos->getToken() == "<if_statement>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){						
			temp_node->setToken("<if_statement1>"); newTempNode();
			temp_node->setToken("<if_statement2>"); newTempNode();
			temp_node->setToken("<if_statement3>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<if_statement1>"){ pos->setOption((pos->getOption()) + 1);					

		if (pos->getOption() == 1){																				
			temp_node->setToken("<T_IF>"); newTempNode();
			temp_node->setToken("<if_statement4>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<if_statement4>"){ pos->setOption((pos->getOption()) + 1);					

		if (pos->getOption() == 1){																			
			temp_node->setToken("<T_LPARANTH>"); newTempNode();
			temp_node->setToken("<expression>"); newTempNode();
			temp_node->setToken("<T_RPARANTH>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<if_statement2>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){																				
			temp_node->setToken("<T_THEN>"); newTempNode();
			temp_node->setToken("<if_statement5>"); newTempNode();
			temp_node->setToken("<if_statement6>"); newTempNode();
	
			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<if_statement5>"){ pos->setOption((pos->getOption()) + 1);	
					
		if (pos->getOption() == 1){																				
			temp_node->setToken("<statement>"); newTempNode();
			temp_node->setToken("<T_SEMICOLON>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<if_statement6>"){ pos->setOption((pos->getOption()) + 1);	
			
		if (pos->getOption() == 1){													

			temp_node->setToken("<if_statement5>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else if (pos->getOption() == 2){													

			temp_node->setToken("<NULL>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	}  else if (pos->getToken() == "<if_statement3>"){ pos->setOption((pos->getOption()) + 1);	
			
		if (pos->getOption() == 1){																				
			temp_node->setToken("<if_statement7>"); newTempNode();
			temp_node->setToken("<T_END>"); newTempNode();
			temp_node->setToken("<T_IF>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<if_statement7>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){													
			temp_node->setToken("<T_ELSE>"); newTempNode();
			temp_node->setToken("<if_statement5>"); newTempNode();
			temp_node->setToken("<if_statement6>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else if (pos->getOption() == 2){													
			temp_node->setToken("<NULL>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		}  else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<loop_statement>
--------------------------------------------------*/
	else if (pos->getToken() == "<loop_statement>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){						
			temp_node->setToken("<T_FOR>"); newTempNode();
			temp_node->setToken("<loop_statement1>"); newTempNode();
			temp_node->setToken("<loop_statement2>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {

			backUp();
		}
	} else if (pos->getToken() == "<loop_statement1>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){						
			temp_node->setToken("<T_LPARANTH>"); newTempNode();
			temp_node->setToken("<loop_statement3>"); newTempNode();
			temp_node->setToken("<loop_statement4>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<loop_statement3>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){						
			temp_node->setToken("<statement3>"); newTempNode();
			//temp_node->setToken("<statement>"); newTempNode();
			temp_node->setToken("<T_SEMICOLON>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<loop_statement4>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){						
			temp_node->setToken("<expression>"); newTempNode();
			temp_node->setToken("<T_RPARANTH>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<loop_statement2>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){						
			temp_node->setToken("<loop_statement5>"); newTempNode();
			temp_node->setToken("<T_END>"); newTempNode();
			temp_node->setToken("<T_FOR>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<loop_statement5>"){ pos->setOption((pos->getOption()) + 1);	
				
		if (pos->getOption() == 1){						
			temp_node->setToken("<statement>"); newTempNode();
			temp_node->setToken("<T_SEMICOLON>"); newTempNode();
			temp_node->setToken("<loop_statement5>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();				
		} else if (pos->getOption() == 2){						
			temp_node->setToken("<NULL>"); newTempNode();
			
			pos = pos->getLeft();
			createnode_3();				
		} else {
			backUp();
		}
	} 
/*--------------------------------------------------  
	<return_statement>
--------------------------------------------------*/
	else if (pos->getToken() == "<return_statement>"){ pos->setOption((pos->getOption()) + 1);			
		
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_RETURN"){				
				newNewNode();
			} else {
				createnode_3();
			}	
		} else {
			backUp();
		}
	} 
/*--------------------------------------------------  
	<identifier>
--------------------------------------------------*/
	else if (pos->getToken() == "<identifier>"){ pos->setOption((pos->getOption()) + 1);			
		
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_IDENTIFIER"){				
				newNewNode();
			} else {
				createnode_3();
			}	
		} else {
			backUp();
		}
	} 
/*--------------------------------------------------  
	<expression>
--------------------------------------------------*/
	else if (pos->getToken() == "<expression>"){ pos->setOption((pos->getOption()) + 1);			

		//expression_flag = true;

		if (pos->getOption() == 1){	
			temp_node->setToken("<T_NOT>");	newTempNode();
			temp_node->setToken("<arithOp>");newTempNode();
			
			temp_node->setPrime(true);
			temp_node->setToken("<expression'>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 2){	
			temp_node->setToken("<arithOp>");newTempNode();
			
			temp_node->setPrime(true);
			temp_node->setToken("<expression'>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {			
			backUp();
		}
	} else if (pos->getToken() == "<expression'>"){ pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){	
			temp_node->setToken("<T_AND>");	newTempNode();
			temp_node->setToken("<arithOp>");newTempNode();
			
			temp_node->setPrime(true);
			temp_node->setToken("<expression'>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 2){	
			temp_node->setToken("<T_OR>");newTempNode();
			temp_node->setToken("<arithOp>");newTempNode();
			
			temp_node->setPrime(true);
			temp_node->setToken("<expression'>");newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 3){	
			temp_node->setToken("<NULL>");newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {	
			//expression_flag = false;		
			backUp();
		}
	} 
/*--------------------------------------------------  
	<artithOp>
--------------------------------------------------*/
	 else if (pos->getToken() == "<arithOp>"){ pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){						
			temp_node->setToken("<relation>");			
			newTempNode();
			
			temp_node->setPrime(true);
			temp_node->setToken("<arithOp'>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			
			backUp();
		}
	} else if (pos->getToken() == "<arithOp'>"){ pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){								
				
			temp_node->setToken("<T_ADD>");	newTempNode();
		
			temp_node->setToken("<relation>"); newTempNode();	

			temp_node->setToken("<arithOp'>");
			temp_node->setPrime(true); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 2){								
			
			temp_node->setToken("<T_MINUS>"); newTempNode();
		
			temp_node->setToken("<relation>"); newTempNode();	

			temp_node->setToken("<arithOp'>");
			temp_node->setPrime(true); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		}  else if (pos->getOption() == 3){
		
			temp_node->setToken("<NULL>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
		
	}
/*--------------------------------------------------  
	<relation>
--------------------------------------------------*/
	else if (pos->getToken() == "<relation>"){ pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){						
			temp_node->setToken("<term>"); newTempNode();				
				
			temp_node->setPrime(true); temp_node->setToken("<relation'>");	newTempNode();			
			
			pos = pos->getLeft();
			createnode_3();								
		} else {			
			backUp();
		}
	} else if (pos->getToken() == "<relation'>"){ pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){									
							
			temp_node->setToken("<T_EQUALTO>");	newTempNode();
		
			temp_node->setToken("<term>");	newTempNode();	

			temp_node->setPrime(true); temp_node->setToken("<relation'>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 2){
				
			temp_node->setToken("<T_NOTEQUALTO>");	newTempNode();
		
			temp_node->setToken("<term>");	newTempNode();	

			temp_node->setPrime(true); temp_node->setToken("<relation'>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 3){
			
			temp_node->setToken("<T_LESSTHAN>"); newTempNode();
		
			temp_node->setToken("<term>");	newTempNode();	

			temp_node->setPrime(true); temp_node->setToken("<relation'>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 4){
			
			temp_node->setToken("<T_LESSTHANEQUAL>"); newTempNode();
		
			temp_node->setToken("<term>");	newTempNode();	

			temp_node->setPrime(true); temp_node->setToken("<relation'>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 5){
				
			temp_node->setToken("<T_GREATERTHAN>");	newTempNode();
		
			temp_node->setToken("<term>");	newTempNode();	

			temp_node->setPrime(true); temp_node->setToken("<relation'>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 6){
			
			temp_node->setToken("<T_GREATERTHANEQUAL>"); newTempNode();
		
			temp_node->setToken("<term>");	newTempNode();	

			temp_node->setPrime(true); temp_node->setToken("<relation'>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 7){
			
			temp_node->setToken("<NULL>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}		
	}
/*--------------------------------------------------  
	<term>
--------------------------------------------------*/	
	 else if (pos->getToken() == "<term>"){	pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){						
			temp_node->setToken("<factor>"); newTempNode();	

			temp_node->setPrime(true);
			temp_node->setToken("<term'>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();								
		} else {			
			backUp();
		}
	} else if (pos->getToken() == "<term'>"){ pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){												
			temp_node->setToken("<T_MULT>"); newTempNode();
		
			temp_node->setToken("<factor>"); newTempNode();	

			temp_node->setPrime(true);
			temp_node->setToken("<term'>"); newTempNode();	

			pos = pos->getLeft();
	     	createnode_3();			
		} else if (pos->getOption() == 2){							
			temp_node->setToken("<T_DIVIDE>");				
			newTempNode();
		
			temp_node->setToken("<factor>");				
			newTempNode();	

			temp_node->setPrime(true);
			temp_node->setToken("<term'>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();
		}  else if (pos->getOption() == 3){							
			temp_node->setToken("<NULL>");				
			newTempNode();
		
			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<factor>
--------------------------------------------------*/
	 else if (pos->getToken() == "<factor>"){
		pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){						
			temp_node->setToken("<T_TRUE>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else if (pos->getOption() == 2){		
			temp_node->setToken("<T_FALSE>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else if (pos->getOption() == 3){

			temp_node->setToken("<char>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else if (pos->getOption() == 4){			
			temp_node->setToken("<string>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else if (pos->getOption() == 5){
			temp_node->setToken("<factor2>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();			
		} else if (pos->getOption() == 6){							
			temp_node->setToken("<T_LPARANTH>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();					
		} else {	
			backUp();
		}
	} else if (pos->getToken() == "<factor2>"){ pos->setOption((pos->getOption()) + 1);	
			
		if (pos->getOption() == 1){						
			temp_node->setToken("<T_MINUS>"); newTempNode();

			temp_node->setToken("<factor3>"); newTempNode();
			
			pos = pos->getLeft();
			createnode_3();		
		} else if (pos->getOption() == 2){						
			temp_node->setToken("<factor3>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();		
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<factor3>"){
		pos->setOption((pos->getOption()) + 1);	
			
		if (pos->getOption() == 1){						
			temp_node->setToken("<name>");				
			newTempNode();
			
			pos = pos->getLeft();
			createnode_3();	
		} else if (pos->getOption() == 2){	
			temp_node->setToken("<number>");				
			newTempNode();
				
			pos = pos->getLeft();
			createnode_3();	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<factor4>"){
		pos->setOption((pos->getOption()) + 1);			
	
		if (pos->getOption() == 1){						
			temp_node->setToken("<expression>");			
			newTempNode();
			
			temp_node->setToken("<T_RPARANTH>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	} 
/*--------------------------------------------------  
	<name>
--------------------------------------------------*/
 else if (pos->getToken() == "<name>"){
		pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){						

			temp_node->setToken("<name1>");				
			newTempNode();

			temp_node->setToken("<name2>");				
			newTempNode();	

			pos = pos->getLeft();
			createnode_3();	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<name1>"){
		pos->setOption((pos->getOption()) + 1);			
		if (pos->getOption() == 1){						
			
			temp_node->setToken("<identifier>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<name2>"){pos->setOption((pos->getOption()) + 1);			
		if (pos->getOption() == 1){									
			temp_node->setToken("<T_LBRACKET>"); newTempNode();
			temp_node->setToken("<name3>");	newTempNode();
//cout << 90;
			pos = pos->getLeft();
			createnode_3();					
		} else if (pos->getOption() == 2 ){								
			temp_node->setToken("<NULL>"); newTempNode();
//cout << 95;
			pos = pos->getLeft();
			createnode_3();					
		} else {
			name3_flag = false;
			backUp();
		}
	} else if (pos->getToken() == "<name3>"){ pos->setOption((pos->getOption()) + 1);			//hhhhh
name3_flag = true;
		if (pos->getOption() == 1){						
			temp_node->setToken("<expression>"); newTempNode();			
			temp_node->setToken("<T_RBRACKET>"); newTempNode();
//cout << 100;
			pos = pos->getLeft();
			createnode_3();
		} else {
			is_legit = false;
			backUp();
		}
	} 
/*--------------------------------------------------  
	<argument_list>
--------------------------------------------------*/
	else if (pos->getToken() == "<argument_list>"){pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){						
			temp_node->setToken("<expression>");			
			newTempNode();
			
			temp_node->setPrime(true);
			temp_node->setToken("<argument_list'>");				
			newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<argument_list'>"){ pos->setOption((pos->getOption()) + 1);			

		if (pos->getOption() == 1){								
						
			temp_node->setToken("<T_COMMA>"); newTempNode();
		
			temp_node->setToken("<expression>"); newTempNode();	

			temp_node->setToken("<argument_list'>");
			temp_node->setPrime(true); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else if (pos->getOption() == 2){								
						
			temp_node->setToken("<NULL>"); newTempNode();

			pos = pos->getLeft();
			createnode_3();
		} else {
			backUp();
		}
	}
/*--------------------------------------------------  
	<char>
--------------------------------------------------*/
	else if (pos->getToken() == "<char>"){pos->setOption((pos->getOption()) + 1);			
		if (pos->getOption() == 1){						

			if (new_node->getToken() == "T_CHARVAL"){				
				newNewNode();
			} else {createnode_3();}	
		} else {	
			backUp();
		}
	}
/*--------------------------------------------------  
	<string>
--------------------------------------------------*/
	 else if (pos->getToken() == "<string>"){pos->setOption((pos->getOption()) + 1);		
		if (pos->getOption() == 1){					
			if (new_node->getToken() == "T_STRINGVAL"){				
				newNewNode(); 
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} 
/*--------------------------------------------------  
	<number>
--------------------------------------------------*/
	else if (pos->getToken() == "<number>"){ pos->setOption((pos->getOption()) + 1);			
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_NUMBERVAL"){				
				newNewNode(); 
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} 
/*--------------------------------------------------  
	<tokens>
--------------------------------------------------*/
	else if (pos->getToken() == "<T_PROGRAM>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_PROGRAM"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_IS>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_IS"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_BEGIN>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_BEGIN"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_END>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_END"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_GLOBAL>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_GLOBAL"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_PROCEDURE>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_PROCEDURE"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_PERIOD>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_PERIOD"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_IN>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_IN"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_OUT>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_OUT"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_INOUT>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_INOUT"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	}else if (pos->getToken() == "<T_LPARANTH>"){ pos->setOption((pos->getOption()) + 1);					
		if (pos->getOption() == 1){									
			if (new_node->getToken() == "T_LPARANTH"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_LBRACKET>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_LBRACKET"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_RPARANTH>"){ pos->setOption((pos->getOption()) + 1);					
		if (pos->getOption() == 1){									
			if (new_node->getToken() == "T_RPARANTH"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_RBRACKET>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_RBRACKET"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_SEMICOLON>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_SEMICOLON"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_INTEGER>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_INTEGER"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_FLOAT>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_FLOAT"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_STRING>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_STRING"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_BOOL>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_BOOL"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_CHAR>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_CHAR"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_IF>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_IF"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_THEN>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_THEN"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_ELSE>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_ELSE"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_FOR>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_FOR"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_RETURN>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_RETURN"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_AND>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_AND"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_OR>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_OR"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_ADD>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_ADD"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_MINUS>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_MINUS"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_NOT>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_NOT"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_LESSTHAN>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_LESSTHAN"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_LESSTHANEQUAL>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_LESSTHANEQUAL"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_GREATERTHAN>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_GREATERTHAN"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_GREATERTHANEQUAL>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_GREATERTHANEQUAL"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_EQUALTO>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_EQUALTO"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_NOTEQUALTO>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_NOTEQUALTO"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_ASSIGN>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_ASSIGN"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_MULT>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_MULT"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_DIVIDE>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_DIVIDE"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_TRUE>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_TRUE"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_FALSE>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_FALSE"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<T_COLON>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_COLON"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	}  else if (pos->getToken() == "<T_COMMA>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
			if (new_node->getToken() == "T_COMMA"){				
				newNewNode();
			} else {createnode_3();}	
		} else {
			backUp();
		}
	} else if (pos->getToken() == "<NULL>"){ pos->setOption((pos->getOption()) + 1);	
		if (pos->getOption() == 1){						
							
			pos->setNodeStatus(true);
			
			re_findStart();	
		} else {
			backUp();
		}
	}
  
}

void ParseTree::newTempNode(){
	pos->setNodeStatus(true);						
	pos->setLeaf(false);							

	if (pos->getLeft() == NULL){
		pos->setLeft(temp_node);					
	} else if (pos->getMiddle() == NULL){
		pos->setMiddle(temp_node);					
	} else if (pos->getRight() == NULL){
		pos->setRight(temp_node);					
	}
	
	temp_node->setRoot(pos);					
	clearTempNode();
}



void ParseTree::newNewNode(){	
	pos->setNodeStatus(true);						
	pos->setLeaf(false);							

	if (pos->getLeft() == NULL){
		pos->setLeft(new_node);					
	} else if (pos->getMiddle() == NULL){
		pos->setMiddle(new_node);					
	} else if (pos->getRight() == NULL){
		pos->setRight(new_node);					
	}
			
	new_node->setRoot(pos);								//Set new node root  to pos
	new_node->setNodeStatus(true);
	clearNewNode();
}

void ParseTree::createnode_2(std::string input){
	ParseNode *temp = new ParseNode;

	temp_node = temp;

	findStart(treeroot);
	
	createnode_3();

	if (input == "T_ENDFILE"){
		checkStatus(treeroot);

	}
	
	clearPos();
	return;
}

ParseNode *ParseTree::checkStatus(ParseNode *node){
	if (!(node->getLeft() == NULL)){ checkStatus(node->getLeft());}
	if (!(node->getMiddle() == NULL)){ checkStatus(node->getMiddle());}
	if (!(node->getRight() == NULL)){ checkStatus(node->getRight());}

	if (node->getNodeStatus() == false){
		is_legit = false;	
	}

	return node->getRoot();
}

ParseNode *ParseTree::travel(ParseNode *node)
{
	if (!(node->getLeft() == NULL)){travel(node->getLeft());}
	if (!(node->getMiddle() == NULL)){travel(node->getMiddle());}
	if (!(node->getRight() == NULL)){travel(node->getRight());}

	cout << node->getToken() << " | " << node->getNodeStatus() << endl;

	return node->getRoot();
}

/*void ParseTree::display()
{
	ParseNode *temp = new ParseNode;

	if (treeroot == NULL){
		cout << "Nothing in Parse Tree" << endl;
	} else {
		temp = treeroot;
		//cout << temp->getToken() << endl;
		
		if (temp->getLeft() != NULL){
			temp = treeroot->getLeft();
			cout << temp->getToken() << endl;
			temp = treeroot;
		} else if (temp->getRight() != NULL){
			temp = treeroot->getRight();
			cout << temp->getToken() << endl;
			temp = treeroot;
		} else if (temp->getMiddle() != NULL){
			temp = treeroot->getMiddle();
			cout << temp->getToken() << endl;
			temp = treeroot;
		}		
	}	
}

void ParseTree::newNullNode(){
	pos->setNodeStatus(true);						//The pos node is fufilled
	pos->setLeaf(false);							//The pos is not a leaf

	if (pos->getLeft() == NULL){
		pos->setLeft(null_node);					
	} else if (pos->getMiddle() == NULL){
		pos->setMiddle(null_node);					
	} else if (pos->getRight() == NULL){
		pos->setRight(null_node);					
	}
	
	null_node->setToken("<null>");
	null_node->setRoot(pos);
	null_node->setNodeStatus(true);					
	clearNullNode();
}

void ParseTree::clearNullNode(){ParseNode *temp = new ParseNode; null_node = temp;}
void ParseTree::specialBackUp(std::string input){
	ParseNode *temp = new ParseNode;
	pos = pos->getRoot();
	if (pos->getLeft() != NULL){ temp = pos->getLeft(); delete temp; pos->setLeft(NULL); temp = NULL;}
	if (pos->getMiddle() != NULL){ temp = pos->getMiddle(); delete temp; pos->setMiddle(NULL); temp = NULL;} 
	if (pos->getRight() != NULL){ temp = pos->getRight(); delete temp; pos->setRight(NULL); temp = NULL;}

	if (pos->getToken() == input){
		pos = pos->getRoot();
		if ((pos->getLeft())->getToken() == input){ 
			temp = pos->getLeft();
			delete temp; pos->setLeft(NULL); temp = NULL;			
		} else if ((pos->getMiddle())->getToken() == input){ 
			temp = pos->getMiddle();
			delete temp; pos->setMiddle(NULL); temp = NULL;
		} else if ((pos->getRight())->getToken() == input){ 
			temp = pos->getRight();
			delete temp; pos->setRight(NULL); temp = NULL;
		}
	}
	createnode_3();
}

void ParseTree::factorbackUp(){
	ParseNode *temp = new ParseNode;
	pos = pos->getRoot();
	if (pos->getLeft() != NULL){ temp = pos->getLeft(); delete temp; pos->setLeft(NULL); temp = NULL;} 
	if (pos->getMiddle() != NULL){ temp = pos->getMiddle(); delete temp; pos->setMiddle(NULL); temp = NULL;} 
	if (pos->getRight() != NULL){ temp = pos->getRight(); delete temp; pos->setRight(NULL); temp = NULL;}
	
	if (pos->getToken() == "<term'>" ){
		pos = pos->getRoot();
		if ((pos->getLeft())->getToken() == "<term'>"){ 
			temp = pos->getLeft();
			delete temp; pos->setLeft(NULL); temp = NULL;			
		} else if ((pos->getMiddle())->getToken() == "<term'>"){ 
			temp = pos->getMiddle();
			delete temp; pos->setMiddle(NULL); temp = NULL;
		} else if ((pos->getRight())->getToken() == "<term'>"){ 
			temp = pos->getRight();
			delete temp; pos->setRight(NULL); temp = NULL;
		}
	}
	createnode_3();
}

ParseNode *ParseTree::setExpressFlag(ParseNode *start)
{	
	if (start->getToken() == "<expression>" && get_new_pos){
 		expression_flag = false; expression_count++;
 	}

	if (!(start->getLeft() == NULL)){setExpressFlag(start->getLeft());}
	if (!(start->getMiddle() == NULL)){setExpressFlag(start->getMiddle());}
	if (!(start->getRight() == NULL)){setExpressFlag(start->getRight());}	
	if (start->getNodeStatus() == false){
		if (get_new_pos){
			pos = start;
			get_new_pos = false;	
		}
	}
	

 	

	return start->getRoot();			
}
*/
