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
R[0].char_val='h';
	R[1].char_val='i';
	R[0].char_val=R[0].char_val+'y';
	R[1].char_val=R[1].char_val+'o';
	R[0].char_val=R[0].char_val+'j';
	R[1].char_val=R[1].char_val+'k';
	MM[0].char_val=R[0].char_val;
	MM[1].char_val=R[1].char_val;
}