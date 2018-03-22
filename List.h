#ifndef LIST_H
#define LIST_H

/*--------------------------------  
	Node stuct for the linked list
--------------------------------*/
struct node{	
	node *previous;
	node *next;
	tokens token;
	std::string str;
};

class list{
	private:
    	node *head, *tail, *pos;
    	int size;
    	std::string code_gen;
    public:
    	list();
    	int get_size();
    	int createnode(tokens value);    	
		int display(); 
		int reset_pos();
		tokens get_one();

		void setCG(std::string input);
		std::string getCG();
		
};

#endif

#ifndef CHARTNODE_H
#define CHARTNODE_H

class ChartNode{
	private:
    	ChartNode *previous, *next; list *read;
    	std::string value;
    public:
    	
};

#endif



