#ifndef ERROR_H
#define ERROR_H


class Error{
	private:
    	
    public:
    	void error(int line, int type);	
    	void error(int line, int type, std::string var);	
};

#endif
