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

R[0].int_val=1;
	MM[101].int_val=R[0].int_val;

R[0].int_val=0;
	MM[110].int_val=R[0].int_val;


FOR3:
	R[0].int_val=MM[110].int_val;
	R[1].int_val=10;
	R[0].bool_val=R[0].int_val<R[1].int_val;

if (R[0].bool_val) goto FOR4;
if (!R[0].bool_val) goto FOR5;

FOR4:
	R[0].int_val=MM[110].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[110].int_val=R[0].int_val;

R[0].int_val=MM[101].int_val;
	R[0].int_val=R[0].int_val*2;
	MM[101].int_val=R[0].int_val;

	goto FOR3;
FOR5: 1;

R[0].int_val=0;
	MM[110].int_val=R[0].int_val;


FOR6:
	R[0].int_val=MM[110].int_val;
	R[1].int_val=10;
	R[0].bool_val=R[0].int_val<R[1].int_val;

if (R[0].bool_val) goto FOR7;
if (!R[0].bool_val) goto FOR8;

FOR7:
	R[0].int_val=MM[110].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[110].int_val=R[0].int_val;

R[0].int_val=MM[101].int_val;
	R[0].int_val=R[0].int_val*2;
	MM[101].int_val=R[0].int_val;

	goto FOR6;
FOR8: 1;

}