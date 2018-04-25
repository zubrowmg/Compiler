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
int R3[10000];
int RP[10000];

int main(){
FILE *outfile; outfile = fopen("output.txt","w");
FILE *infile; infile = fopen("input.txt","r");

goto main_prog;



main_prog: 1;

R[0].int_val=10;
	MM[0].int_val=R[0].int_val;

R[0].int_val=MM[0].int_val;
	R[0].int_val=R[0].int_val*10;
	MM[0].int_val=R[0].int_val;

fprintf(outfile, "%d",MM[0].int_val);

fclose(outfile);
fclose(infile);
}