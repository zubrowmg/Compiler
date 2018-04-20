#ifndef ERROR_H
#define ERROR_H



class Error{
	private:
    	int total_errors;
    public:
    	Error();
    	void error(int line, int type);	
    	void error(int line, int type, std::string var);
    	void error(int line, int type, std::string TC_1, std::string TC_2);	
    	void error(int line, int type, int arg_num, std::string var);
    	int getTotalErrors();
    	void incrementError();
};

#endif
