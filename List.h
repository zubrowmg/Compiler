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
    	int if_goto_num;
    	
    public:
    	list();
    	int get_size();
    	int createnode(tokens value);    	
		int display(); 
		int reset_pos();
		tokens get_one();
		tokens look_ahead();
		tokens look_back();
		tokens look_ahead_no_wrap();
		tokens look_ahead_two_no_wrap();
		void goBackOne();

		void setCG(std::string input);
		std::string getCG();

		void modifyLast(tokens tok);

		void deleteNode(tokens tok);
		//void setPos();
		
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



