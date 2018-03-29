#include <string.h>
#include <stdio.h>
#include <stdbool.h>

union val{
	int int_val;
	bool bool_val;
	char char_val;
	char string_val[20];
	float float_val;
};

union val MM[100];
union val R[100];

int main(){
R[0].int_val=2;
	MM[1].int_val=R[0].int_val;
R[0].int_val=MM[1].int_val;
	R[0].int_val=R[0].int_val+MM[1].int_val;
	MM[1].int_val=R[0].int_val;
printf("%d", MM[1].int_val);
}
