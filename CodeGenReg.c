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
FILE *outfile;
outfile = fopen("output.txt","w");

R[0].int_val=8;
	MM[100].int_val=R[0].int_val;

R[0].char_val='l';
	R[1].char_val='o';
	R[2].char_val='l';
	R[3].char_val=' ';
	R[4].char_val=' ';
	R[5].char_val=' ';
	R[6].char_val=' ';
	R[7].char_val=' ';
	R[8].char_val=' ';
	R[9].char_val=' ';
	R[10].char_val=' ';
	R[11].char_val=' ';
	R[12].char_val=' ';
	R[13].char_val=' ';
	R[14].char_val=' ';
	R[15].char_val=' ';
	R[16].char_val=' ';
	R[17].char_val=' ';
	R[18].char_val=' ';
	R[19].char_val=' ';
	R[20].char_val=' ';
	R[21].char_val=' ';
	R[22].char_val=' ';
	R[23].char_val=' ';
	R[24].char_val=' ';
	R[25].char_val=' ';
	R[26].char_val=' ';
	R[27].char_val=' ';
	R[28].char_val=' ';
	R[29].char_val=' ';
	R[30].char_val=' ';
	R[31].char_val=' ';
	R[32].char_val=' ';
	R[33].char_val=' ';
	R[34].char_val=' ';
	R[35].char_val=' ';
	R[36].char_val=' ';
	R[37].char_val=' ';
	R[38].char_val=' ';
	R[39].char_val=' ';
	R[40].char_val=' ';
	R[41].char_val=' ';
	R[42].char_val=' ';
	R[43].char_val=' ';
	R[44].char_val=' ';
	R[45].char_val=' ';
	R[46].char_val=' ';
	R[47].char_val=' ';
	R[48].char_val=' ';
	R[49].char_val=' ';
	
	MM[111].char_val=R[0].char_val;
	MM[112].char_val=R[1].char_val;
	MM[113].char_val=R[2].char_val;
	MM[114].char_val=R[3].char_val;
	MM[115].char_val=R[4].char_val;
	MM[116].char_val=R[5].char_val;
	MM[117].char_val=R[6].char_val;
	MM[118].char_val=R[7].char_val;
	MM[119].char_val=R[8].char_val;
	MM[120].char_val=R[9].char_val;
	MM[121].char_val=R[10].char_val;
	MM[122].char_val=R[11].char_val;
	MM[123].char_val=R[12].char_val;
	MM[124].char_val=R[13].char_val;
	MM[125].char_val=R[14].char_val;
	MM[126].char_val=R[15].char_val;
	MM[127].char_val=R[16].char_val;
	MM[128].char_val=R[17].char_val;
	MM[129].char_val=R[18].char_val;
	MM[130].char_val=R[19].char_val;
	MM[131].char_val=R[20].char_val;
	MM[132].char_val=R[21].char_val;
	MM[133].char_val=R[22].char_val;
	MM[134].char_val=R[23].char_val;
	MM[135].char_val=R[24].char_val;
	MM[136].char_val=R[25].char_val;
	MM[137].char_val=R[26].char_val;
	MM[138].char_val=R[27].char_val;
	MM[139].char_val=R[28].char_val;
	MM[140].char_val=R[29].char_val;
	MM[141].char_val=R[30].char_val;
	MM[142].char_val=R[31].char_val;
	MM[143].char_val=R[32].char_val;
	MM[144].char_val=R[33].char_val;
	MM[145].char_val=R[34].char_val;
	MM[146].char_val=R[35].char_val;
	MM[147].char_val=R[36].char_val;
	MM[148].char_val=R[37].char_val;
	MM[149].char_val=R[38].char_val;
	MM[150].char_val=R[39].char_val;
	MM[151].char_val=R[40].char_val;
	MM[152].char_val=R[41].char_val;
	MM[153].char_val=R[42].char_val;
	MM[154].char_val=R[43].char_val;
	MM[155].char_val=R[44].char_val;
	MM[156].char_val=R[45].char_val;
	MM[157].char_val=R[46].char_val;
	MM[158].char_val=R[47].char_val;
	MM[159].char_val=R[48].char_val;
	MM[160].char_val=R[49].char_val;
putc('h', outfile);
putc('e', outfile);
putc('l', outfile);
putc('l', outfile);
putc('i', outfile);
fprintf(outfile, "%c",MM[111].char_val);
fprintf(outfile, "%c",MM[112].char_val);
fprintf(outfile, "%c",MM[113].char_val);
fprintf(outfile, "%c",MM[114].char_val);
fprintf(outfile, "%c",MM[115].char_val);
fprintf(outfile, "%c",MM[116].char_val);
fprintf(outfile, "%c",MM[117].char_val);
fprintf(outfile, "%c",MM[118].char_val);
fprintf(outfile, "%c",MM[119].char_val);
fprintf(outfile, "%c",MM[120].char_val);
fprintf(outfile, "%c",MM[121].char_val);
fprintf(outfile, "%c",MM[122].char_val);
fprintf(outfile, "%c",MM[123].char_val);
fprintf(outfile, "%c",MM[124].char_val);
fprintf(outfile, "%c",MM[125].char_val);
fprintf(outfile, "%c",MM[126].char_val);
fprintf(outfile, "%c",MM[127].char_val);
fprintf(outfile, "%c",MM[128].char_val);
fprintf(outfile, "%c",MM[129].char_val);
fprintf(outfile, "%c",MM[130].char_val);
fprintf(outfile, "%c",MM[131].char_val);
fprintf(outfile, "%c",MM[132].char_val);
fprintf(outfile, "%c",MM[133].char_val);
fprintf(outfile, "%c",MM[134].char_val);
fprintf(outfile, "%c",MM[135].char_val);
fprintf(outfile, "%c",MM[136].char_val);
fprintf(outfile, "%c",MM[137].char_val);
fprintf(outfile, "%c",MM[138].char_val);
fprintf(outfile, "%c",MM[139].char_val);
fprintf(outfile, "%c",MM[140].char_val);
fprintf(outfile, "%c",MM[141].char_val);
fprintf(outfile, "%c",MM[142].char_val);
fprintf(outfile, "%c",MM[143].char_val);
fprintf(outfile, "%c",MM[144].char_val);
fprintf(outfile, "%c",MM[145].char_val);
fprintf(outfile, "%c",MM[146].char_val);
fprintf(outfile, "%c",MM[147].char_val);
fprintf(outfile, "%c",MM[148].char_val);
fprintf(outfile, "%c",MM[149].char_val);
fprintf(outfile, "%c",MM[150].char_val);
fprintf(outfile, "%c",MM[151].char_val);
fprintf(outfile, "%c",MM[152].char_val);
fprintf(outfile, "%c",MM[153].char_val);
fprintf(outfile, "%c",MM[154].char_val);
fprintf(outfile, "%c",MM[155].char_val);
fprintf(outfile, "%c",MM[156].char_val);
fprintf(outfile, "%c",MM[157].char_val);
fprintf(outfile, "%c",MM[158].char_val);
fprintf(outfile, "%c",MM[159].char_val);
fprintf(outfile, "%c",MM[160].char_val);

fclose(outfile);
}