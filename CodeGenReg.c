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

if_proc: 1;
goto if_proc_main;

if_proc_main: 1;
fprintf(outfile, "%d",MM[0].int_val);


putc(' ', outfile);


R[0].int_val=MM[0].int_val;
	R[0].int_val=R[0].int_val-10;
	MM[0].int_val=R[0].int_val;



fprintf(outfile, "%d",MM[0].int_val);


putc(' ', outfile);



RP[0]=RP[0]+1;
R3[0]=RP[0]==1;
if (R3[0]) goto return_if_proc0;

for_proc: 1;
goto for_proc_main;

for_proc_main: 1;



RP[1]=RP[1]+1;



main_prog: 1;

R[0].int_val=10;
	MM[1].int_val=R[0].int_val;

R[0].int_val=MM[1].int_val;
	R[0].int_val=R[0].int_val*10;
	MM[1].int_val=R[0].int_val;

fprintf(outfile, "%d",MM[1].int_val);
putc(' ', outfile);
R[0].int_val=MM[1].int_val;
	MM[0].int_val=R[0].int_val;

goto if_proc;
return_if_proc0: 1;

fprintf(outfile, "%d",MM[1].int_val);
putc(' ', outfile);

fclose(outfile);
fclose(infile);
}