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
}	

int list::display()
{
	node *temp = new node;
	temp = head;    		
	while(temp != NULL) {
		cout << temp->token.type << "  "<< '[' << temp->token.stringValue << ']' << endl;
		temp = temp->next;
	}
	return 0;
} 



int list::get_size(){return size;}
int list::reset_pos(){ pos = head; return 0;}

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

void list::setCG(std::string input)
{
	code_gen = input;
}

std::string list::getCG()
{
	return code_gen;
}