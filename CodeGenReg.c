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

fib: 1;
goto fib_main;

fib_main: 1;
R[0].bool_val=false;
	
if (R[0].bool_val) goto IF3;
if (!R[0].bool_val) goto IF4;

IF3:



RP[0]=RP[0]+1;



IF4: 1;
IF5: 1;




putc('z', outfile);



RP[0]=RP[0]+1;



main_prog: 1;

goto value;
return_value0: 1;

goto value;
return_value0: 1;


fclose(outfile);
fclose(infile);
}