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

print_string: 1;
goto print_string_main;

print_string_main: 1;
putc('e', outfile);
putc('n', outfile);
putc('t', outfile);
putc('e', outfile);
putc('r', outfile);
putc(' ', outfile);
putc('a', outfile);
putc(' ', outfile);
putc('s', outfile);
putc('t', outfile);
putc('r', outfile);
putc('i', outfile);
putc('n', outfile);


fscanf(infile, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",&MM[1].char_val,&MM[2].char_val,&MM[3].char_val,&MM[4].char_val,&MM[5].char_val,&MM[6].char_val,&MM[7].char_val,&MM[8].char_val,&MM[9].char_val,&MM[10].char_val,&MM[11].char_val,&MM[12].char_val,&MM[13].char_val,&MM[14].char_val,&MM[15].char_val,&MM[16].char_val,&MM[17].char_val,&MM[18].char_val,&MM[19].char_val,&MM[20].char_val,&MM[21].char_val,&MM[22].char_val,&MM[23].char_val,&MM[24].char_val,&MM[25].char_val,&MM[26].char_val,&MM[27].char_val,&MM[28].char_val,&MM[29].char_val,&MM[30].char_val,&MM[31].char_val,&MM[32].char_val,&MM[33].char_val,&MM[34].char_val,&MM[35].char_val,&MM[36].char_val,&MM[37].char_val,&MM[38].char_val,&MM[39].char_val,&MM[40].char_val,&MM[41].char_val,&MM[42].char_val,&MM[43].char_val,&MM[44].char_val,&MM[45].char_val,&MM[46].char_val,&MM[47].char_val,&MM[48].char_val,&MM[49].char_val,&MM[50].char_val);


R[0].int_val=MM[0].int_val;
	R[1].int_val=3;
	R[0].bool_val=R[0].int_val<R[1].int_val;

if (R[0].bool_val) goto IF3;
if (!R[0].bool_val) goto IF4;

IF3:


R[0].int_val=MM[0].int_val;
	MM[0].int_val=R[0].int_val;

goto print_string;
return_print_string0: 1;



IF4: 1;
IF5: 1;




fprintf(outfile, "%c",MM[1].char_val);
fprintf(outfile, "%c",MM[2].char_val);
fprintf(outfile, "%c",MM[3].char_val);
fprintf(outfile, "%c",MM[4].char_val);
fprintf(outfile, "%c",MM[5].char_val);
fprintf(outfile, "%c",MM[6].char_val);
fprintf(outfile, "%c",MM[7].char_val);
fprintf(outfile, "%c",MM[8].char_val);
fprintf(outfile, "%c",MM[9].char_val);
fprintf(outfile, "%c",MM[10].char_val);
fprintf(outfile, "%c",MM[11].char_val);
fprintf(outfile, "%c",MM[12].char_val);
fprintf(outfile, "%c",MM[13].char_val);
fprintf(outfile, "%c",MM[14].char_val);
fprintf(outfile, "%c",MM[15].char_val);
fprintf(outfile, "%c",MM[16].char_val);
fprintf(outfile, "%c",MM[17].char_val);
fprintf(outfile, "%c",MM[18].char_val);
fprintf(outfile, "%c",MM[19].char_val);
fprintf(outfile, "%c",MM[20].char_val);
fprintf(outfile, "%c",MM[21].char_val);
fprintf(outfile, "%c",MM[22].char_val);
fprintf(outfile, "%c",MM[23].char_val);
fprintf(outfile, "%c",MM[24].char_val);
fprintf(outfile, "%c",MM[25].char_val);
fprintf(outfile, "%c",MM[26].char_val);
fprintf(outfile, "%c",MM[27].char_val);
fprintf(outfile, "%c",MM[28].char_val);
fprintf(outfile, "%c",MM[29].char_val);
fprintf(outfile, "%c",MM[30].char_val);
fprintf(outfile, "%c",MM[31].char_val);
fprintf(outfile, "%c",MM[32].char_val);
fprintf(outfile, "%c",MM[33].char_val);
fprintf(outfile, "%c",MM[34].char_val);
fprintf(outfile, "%c",MM[35].char_val);
fprintf(outfile, "%c",MM[36].char_val);
fprintf(outfile, "%c",MM[37].char_val);
fprintf(outfile, "%c",MM[38].char_val);
fprintf(outfile, "%c",MM[39].char_val);
fprintf(outfile, "%c",MM[40].char_val);
fprintf(outfile, "%c",MM[41].char_val);
fprintf(outfile, "%c",MM[42].char_val);
fprintf(outfile, "%c",MM[43].char_val);
fprintf(outfile, "%c",MM[44].char_val);
fprintf(outfile, "%c",MM[45].char_val);
fprintf(outfile, "%c",MM[46].char_val);
fprintf(outfile, "%c",MM[47].char_val);
fprintf(outfile, "%c",MM[48].char_val);
fprintf(outfile, "%c",MM[49].char_val);
fprintf(outfile, "%c",MM[50].char_val);



RP[0]=RP[0]+1;
R3[0]=RP[0]==1;
if (R3[0]) goto return_print_string0;
R3[0]=RP[0]==2;
if (R3[0]) goto return_print_string1;



main_prog: 1;

goto print_string;
return_print_string1: 1;


fclose(outfile);
fclose(infile);
}