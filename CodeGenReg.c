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
	R[1].char_val='y';
	R[0].char_val=R[0].char_val+MM[0].char_val;
	R[1].char_val=R[1].char_val+MM[1].char_val;
	R[2].char_val=R[2].char_val+MM[2].char_val;
	R[3].char_val=R[3].char_val+MM[3].char_val;
	R[4].char_val=R[4].char_val+MM[4].char_val;
	R[5].char_val=R[5].char_val+MM[5].char_val;
	R[6].char_val=R[6].char_val+MM[6].char_val;
	R[7].char_val=R[7].char_val+MM[7].char_val;
	R[8].char_val=R[8].char_val+MM[8].char_val;
	R[9].char_val=R[9].char_val+MM[9].char_val;
	R[10].char_val=R[10].char_val+MM[10].char_val;
	R[11].char_val=R[11].char_val+MM[11].char_val;
	R[12].char_val=R[12].char_val+MM[12].char_val;
	R[13].char_val=R[13].char_val+MM[13].char_val;
	R[14].char_val=R[14].char_val+MM[14].char_val;
	R[15].char_val=R[15].char_val+MM[15].char_val;
	R[16].char_val=R[16].char_val+MM[16].char_val;
	R[17].char_val=R[17].char_val+MM[17].char_val;
	R[18].char_val=R[18].char_val+MM[18].char_val;
	R[19].char_val=R[19].char_val+MM[19].char_val;
	R[20].char_val=R[20].char_val+MM[20].char_val;
	R[21].char_val=R[21].char_val+MM[21].char_val;
	R[22].char_val=R[22].char_val+MM[22].char_val;
	R[23].char_val=R[23].char_val+MM[23].char_val;
	R[24].char_val=R[24].char_val+MM[24].char_val;
	R[25].char_val=R[25].char_val+MM[25].char_val;
	R[26].char_val=R[26].char_val+MM[26].char_val;
	R[27].char_val=R[27].char_val+MM[27].char_val;
	R[28].char_val=R[28].char_val+MM[28].char_val;
	R[29].char_val=R[29].char_val+MM[29].char_val;
	R[30].char_val=R[30].char_val+MM[30].char_val;
	R[31].char_val=R[31].char_val+MM[31].char_val;
	R[32].char_val=R[32].char_val+MM[32].char_val;
	R[33].char_val=R[33].char_val+MM[33].char_val;
	R[34].char_val=R[34].char_val+MM[34].char_val;
	R[35].char_val=R[35].char_val+MM[35].char_val;
	R[36].char_val=R[36].char_val+MM[36].char_val;
	R[37].char_val=R[37].char_val+MM[37].char_val;
	R[38].char_val=R[38].char_val+MM[38].char_val;
	R[39].char_val=R[39].char_val+MM[39].char_val;
	R[40].char_val=R[40].char_val+MM[40].char_val;
	R[41].char_val=R[41].char_val+MM[41].char_val;
	R[42].char_val=R[42].char_val+MM[42].char_val;
	R[43].char_val=R[43].char_val+MM[43].char_val;
	R[44].char_val=R[44].char_val+MM[44].char_val;
	R[45].char_val=R[45].char_val+MM[45].char_val;
	R[46].char_val=R[46].char_val+MM[46].char_val;
	R[47].char_val=R[47].char_val+MM[47].char_val;
	R[48].char_val=R[48].char_val+MM[48].char_val;
	R[49].char_val=R[49].char_val+MM[49].char_val;
	R[0].char_val=R[0].char_val+MM[50].char_val;
	R[1].char_val=R[1].char_val+MM[51].char_val;
	R[2].char_val=R[2].char_val+MM[52].char_val;
	R[3].char_val=R[3].char_val+MM[53].char_val;
	R[4].char_val=R[4].char_val+MM[54].char_val;
	R[5].char_val=R[5].char_val+MM[55].char_val;
	R[6].char_val=R[6].char_val+MM[56].char_val;
	R[7].char_val=R[7].char_val+MM[57].char_val;
	R[8].char_val=R[8].char_val+MM[58].char_val;
	R[9].char_val=R[9].char_val+MM[59].char_val;
	R[10].char_val=R[10].char_val+MM[60].char_val;
	R[11].char_val=R[11].char_val+MM[61].char_val;
	R[12].char_val=R[12].char_val+MM[62].char_val;
	R[13].char_val=R[13].char_val+MM[63].char_val;
	R[14].char_val=R[14].char_val+MM[64].char_val;
	R[15].char_val=R[15].char_val+MM[65].char_val;
	R[16].char_val=R[16].char_val+MM[66].char_val;
	R[17].char_val=R[17].char_val+MM[67].char_val;
	R[18].char_val=R[18].char_val+MM[68].char_val;
	R[19].char_val=R[19].char_val+MM[69].char_val;
	R[20].char_val=R[20].char_val+MM[70].char_val;
	R[21].char_val=R[21].char_val+MM[71].char_val;
	R[22].char_val=R[22].char_val+MM[72].char_val;
	R[23].char_val=R[23].char_val+MM[73].char_val;
	R[24].char_val=R[24].char_val+MM[74].char_val;
	R[25].char_val=R[25].char_val+MM[75].char_val;
	R[26].char_val=R[26].char_val+MM[76].char_val;
	R[27].char_val=R[27].char_val+MM[77].char_val;
	R[28].char_val=R[28].char_val+MM[78].char_val;
	R[29].char_val=R[29].char_val+MM[79].char_val;
	R[30].char_val=R[30].char_val+MM[80].char_val;
	R[31].char_val=R[31].char_val+MM[81].char_val;
	R[32].char_val=R[32].char_val+MM[82].char_val;
	R[33].char_val=R[33].char_val+MM[83].char_val;
	R[34].char_val=R[34].char_val+MM[84].char_val;
	R[35].char_val=R[35].char_val+MM[85].char_val;
	R[36].char_val=R[36].char_val+MM[86].char_val;
	R[37].char_val=R[37].char_val+MM[87].char_val;
	R[38].char_val=R[38].char_val+MM[88].char_val;
	R[39].char_val=R[39].char_val+MM[89].char_val;
	R[40].char_val=R[40].char_val+MM[90].char_val;
	R[41].char_val=R[41].char_val+MM[91].char_val;
	R[42].char_val=R[42].char_val+MM[92].char_val;
	R[43].char_val=R[43].char_val+MM[93].char_val;
	R[44].char_val=R[44].char_val+MM[94].char_val;
	R[45].char_val=R[45].char_val+MM[95].char_val;
	R[46].char_val=R[46].char_val+MM[96].char_val;
	R[47].char_val=R[47].char_val+MM[97].char_val;
	R[48].char_val=R[48].char_val+MM[98].char_val;
	R[49].char_val=R[49].char_val+MM[99].char_val;
	
if () {
	R[0].int_val=MM[105].int_val;
	R[0].int_val=R[0].int_val+1;
	MM[105]=R[0];
}
}