#include "record.h"


#include "file_handle.h"
// provide clear interface for others to deal with table/records/met


void read_record_my(string t_name, int rec_ad, Record &rec, table &tab) {    // table_name table1? rec_addr file0 '0'
	record_wrapper a(t_name);
	strcpy(rec.table_name, t_name.c_str());
	rec.rec_addr = rec_ad;
	rec.count = tab.count;
	int j = 0; int k = 0;
	for (j = 0; j < rec.count; j++) {
		double c; char d[50 + 1];
		k = rec_ad;
		string keyname(tab.col[j].col_name);
		if (tab.col[j].type == 1) {
			double key2;
			a.get_d(k, keyname, key2);
			rec.data[j] = (double*)malloc(sizeof(double));
			*((double*)rec.data[j]) = key2;
		}
		else if (tab.col[j].type == 2) {
			string key1;
			a.get_s(k, keyname, key1);
			rec.data[j] = malloc(sizeof(char) * (50 + 1));
			strcpy((char*)rec.data[j], key1.c_str());
		}
	} // read from file%d.dat
}
void read_record_my_by_director_name(string t_name, string file_name, Record & rec, table & tab)
{
	int pos = file_name[4] - 48; // '0' -48 extract the fild%d.dat d
	read_record_my(t_name, pos, rec, tab);
}
