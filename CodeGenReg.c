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

R[0].int_val=0;
	MM[1].int_val=R[0].int_val;

R[0]=-5;
	MM[0].int_val=R[0].int_val;


FOR3:
	R[0].int_val=MM[0].int_val;
	R[1].int_val=4;
	R[0].bool_val=R[0].int_val<R[1].int_val;

if (R[0].bool_val) goto FOR4;
if (!R[0].bool_val) goto FOR5;

FOR4:
	R[0].int_val=MM[1].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[1].int_val=R[0].int_val;

R[0].int_val=MM[0].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[0].int_val=R[0].int_val;

	goto FOR3;
FOR5: 1;

putc('e', outfile);
R[0].int_val=10;
	MM[0].int_val=R[0].int_val;


FOR6:
	R[0].int_val=MM[0].int_val;
	R[1].int_val=20;
	R[0].bool_val=R[0].int_val<R[1].int_val;

if (R[0].bool_val) goto FOR7;
if (!R[0].bool_val) goto FOR8;

FOR7:
	fprintf(outfile, "%f",MM[37].float_val);
R[0].int_val=MM[0].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[0].int_val=R[0].int_val;

	goto FOR6;
FOR8: 1;


fclose(outfile);
fclose(infile);
}