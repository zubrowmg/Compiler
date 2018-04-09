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
R[0].int_val=1;
	R[1].int_val=10;
	
if () {
	R[0].int_val=MM[105].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[105]=R[0];
}
}