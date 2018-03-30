#include <string.h>
#include <stdio.h>
#include <stdbool.h>

union val{
	int int_val;
	bool bool_val;
	char char_val;
	float float_val;
};

union val MM[10000];
union val R[100];

int main(){
R[0].int_val=3;
	R[0].int_val=R[0].int_val*5;
	MM[52].int_val=R[0].int_val;
R[0].int_val=MM[52].int_val;
	MM[58].int_val=R[0].int_val;
R[0].int_val=4;
	MM[58].int_val=R[0].int_val;
R[0].int_val=MM[58].int_val;
	R[0].int_val=R[0].int_val+MM[58].int_val;
	MM[58].int_val=R[0].int_val;
}