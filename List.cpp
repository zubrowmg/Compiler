#include <iostream>
#include <iomanip>

#include "ParseTree.h"
#include "List.h"
#include "Symbol.h"


using namespace std;

/*------------------------------------------------------
	List Class
------------------------------------------------------*/
int list::createnode(tokens value)
{
	node *temp = new node;
	temp->token = value;
	temp->next = NULL;
	if(head == NULL) {
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else {	
		tail->next = temp;
		temp->previous = tail;
		tail = temp;
	}
	size = size + 1;
	return 0;
}

list::list()
{
	size = 0;
	head = NULL;
	tail = NULL;
	pos = NULL;
	code_gen = "";
	if_goto_num = 0;
}	

int list::display()
{
	node *temp = new node;
	temp = head;    		
	while(temp != NULL) {
		cout << temp->token.type << "  "<< '[' << temp->token.stringValue << ']';
		//cout << " " << temp->token.if_goto_label_num;
		cout << " " << temp->token.index;
		cout << endl;
		temp = temp->next;
	}
	return 0;
} 

void list::deleteNode(tokens tok){

}

void list::setTable(std::string input){
	proc = input;
}

std::string list::getTable(){
	return proc;
}

int list::get_size(){return size;}
int list::reset_pos(){ pos = head; return 0;}

void list::modifyLast(tokens tok){
//cout << "*" << tok.stringValue << "*"  << "  " << "*"  << (((pos->previous)->previous)->token).stringValue << "*"  << endl;
	//if ((tok.stringValue) == ((((pos->previous)->previous)->token).stringValue)){
		
//cout << "HI";
		(((pos->previous)->previous)->token).single_array_access = true;

	//}
}

tokens list::get_one() 
{	
	node *temp = new node;	   		
	temp = pos;
	//cout << pos->token.type << "  "<< '[' << pos->token.stringValue << ']' << endl;
	if (pos->next == NULL){
		pos = head;
	} else {
		pos = pos->next;
	}
	return temp->token;
} 

tokens list::look_ahead() 
{	
	return pos->token;
} 

tokens list::look_back() 
{	
	if (pos == head){
		return (tail->previous)->token;
	} else if (pos == head->next){
		return tail->token;
	} else {
		return ((pos->previous)->previous)->token;
	}
} 


tokens list::look_ahead_no_wrap() 
{	
	tokens empty;
	if (pos == head){
		return empty;
	} else {
		return pos->token;
	}
}

tokens list::look_ahead_two_no_wrap() 
{	
	tokens empty;
	if (pos == head){
		return empty;
	} else if (pos == tail){
		return empty;
	} else {
		return (pos->next)->token;
	}
}

void list::goBackOne(){
	if (pos->previous == NULL){
		pos = tail;
	} else {
		pos = pos->previous;
	}
}

void list::setCG(std::string input)
{
	code_gen = input;
}

std::string list::getCG()
{
	return code_gen;
}