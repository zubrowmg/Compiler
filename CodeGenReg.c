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
union val R[10000];
union val R2[10000];

int main(){
R[0].int_val=10;
	MM[110].int_val=R[0].int_val;

R[0].int_val=10;
	MM[97].int_val=R[0].int_val;

R[0].int_val=MM[110].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[110].int_val=R[0].int_val;

}