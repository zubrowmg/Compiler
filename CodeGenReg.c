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
FILE *outfile; outfile = fopen("output.txt","w");
FILE *infile; infile = fopen("input.txt","r");

goto main_prog;

if_proc: 1;

R[0].char_val='a';
	MM[0].char_val=R[0].char_val;



ff_proc: 1;



main_prog: 1;

goto if_proc;
return_if_proc0: 1;

goto if_proc;
return_if_proc1: 1;

goto if_proc;
return_if_proc2: 1;

R[0].char_val='a';
	MM[0].char_val=R[0].char_val;

R[0].char_val=MM[0].char_val;
	MM[1].bool_val=R[0].bool_val;

R[0].char_val=MM[0].char_val;
	MM[3].int_val=R[0].int_val;

R[0].int_val=1;
	
if (R[0].int_val) goto IF3;
if (!R[0].int_val) goto IF4;

IF3:
R[0].char_val=MM[0].char_val;
	MM[4].int_val=R[0].int_val;

IF4: 1;
IF5: 1;
R[0].bool_val=MM[1].bool_val;
	MM[0].char_val=R[0].char_val;

R[0].int_val=24;
	MM[0].char_val=R[0].char_val;

fprintf(outfile, "%d",MM[4].int_val);

fclose(outfile);
fclose(infile);
}