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

int main(){
R[0].int_val=1;
	MM[111].int_val=R[0].int_val;
R[0].int_val=2;
	MM[112].int_val=R[0].int_val;
R[0].int_val=3;
	MM[113].int_val=R[0].int_val;
R[0].int_val=MM[111].int_val;
	R[1].int_val=MM[112].int_val;
	R[2].int_val=MM[113].int_val;
	R[3].int_val=MM[111].int_val;
	R[4].int_val=MM[112].int_val;
	R[5].int_val=MM[113].int_val;
	R[0].bool_val=R[0].int_val==R[3].int_val;
		R[1].bool_val=R[1].int_val==R[4].int_val;
		R[2].bool_val=R[2].int_val==R[5].int_val;
		R[1].bool_val=R[2].bool_val&&R[1].bool_val;
	R[0].bool_val=R[1].bool_val&&R[0].bool_val;
	R[6].int_val=MM[111].int_val;
	R[7].int_val=MM[112].int_val;
	R[8].int_val=MM[113].int_val;
	R[3].bool_val=R[3].int_val==R[6].int_val;
		R[4].bool_val=R[4].int_val==R[7].int_val;
		R[5].bool_val=R[5].int_val==R[8].int_val;
		R[4].bool_val=R[5].bool_val&&R[4].bool_val;
	R[3].bool_val=R[4].bool_val&&R[3].bool_val;
	R[0].bool_val=R[0].bool_val&&R[3].bool_val;;

	
if () {
	R[0].int_val=MM[110].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[101].int_val=R[0].int_val;
}
}