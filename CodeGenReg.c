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

sf_proc: 1;
goto sf_proc_main;

sf_proc_main: 1;
putc('1', outfile);



RP[1]=RP[1]+1;
R3[0]=RP[1]==1;
if (R3[0]) goto return_sf_proc0;

if_proc_main: 1;
goto sf_proc;
return_sf_proc0: 1;



putc('2', outfile);



RP[0]=RP[0]+1;
R3[0]=RP[0]==1;
if (R3[0]) goto return_if_proc0;

ff_proc: 1;
goto ff_proc_main;

ff_proc_main: 1;
R[0].int_val=1;
	MM[13].int_val=R[0].int_val;



putc('3', outfile);



RP[2]=RP[2]+1;
R3[0]=RP[2]==1;
if (R3[0]) goto return_ff_proc0;



main_prog: 1;

goto if_proc;
return_if_proc0: 1;

R[0].int_val=1;
	MM[9].int_val=R[0].int_val;

R[0].int_val=MM[9].int_val;
	MM[4].int_val=R[0].int_val;

R[0].char_val=MM[6].char_val;
	MM[5].char_val=R[0].char_val;

goto ff_proc;
return_ff_proc0: 1;


fclose(outfile);
fclose(infile);
}