#pragma once
#include "headerer.h"

struct col_details {
	char col_name[20];	// 属性名
	int type; // 1代表double 2代表varchar
	int size;	// 建表时的限制
	//int flag;	// 1 代表是主键 2 代表是非主键 3 代表是外码
	//char name[20]; // 如果是外码 这里应该为表名 
};
struct table {
	col_details col[30]; //各属性名字、类型、限制
	char name[20]; // 表名
	char main_key[20]; //主码名字
	int count;  // number of attributes/columns 属性数量
	int rec_count;	//当前表中的记录数
	int last_index; // 待删除
};
struct sin_record {
	double ary[30];
	char var[30][100];
	int flag = 0;  // default 0 0代表删除以外的状态 1代表被标记为删除
};


void read_tabe_meta_my(string tablename, table& tab);
void read_meta(string tablename, table& tab);
void update_meta(string tablename, table& tab);
int find_keyname_position_my(string keyname, table& tab);
vector<string> getFiles(string cate_dir);

int get_main_key_position_my(string tablename, table& tab);
class record_wrapper {
	// deal with single record
public:
	record_wrapper(string tablename) {
		// init
		di = 0; vi = 0; rcount = 0;
		int i = 0, j = 0; tname = tablename;
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 30; j++) {
				rel[i][j] = 0;
			}
		} // set all to 0

		// based on met to build relationship
		string metpath = "table\\" + tablename + "\\met";
		ifstream mp(metpath, ios::binary);
		table read_temp;
		mp.read((char*)&read_temp, sizeof(struct table));
		mp.close();
		rcount = read_temp.rec_count;
		noa = read_temp.count;

		ifstream fp;
		string filepath = "table\\" + tablename + "\\file.data";
		fp.open(filepath, ios::binary);
		// 1000 records?
		fp.read((char*)rec, sizeof(rec));
		fp.close();
		for (i = 0; i < read_temp.count; i++) {
			string str(read_temp.col[i].col_name);
			type_ary[i] = read_temp.col[i].type;
			rel2[str] = i; // map string to int
			if (read_temp.col[i].type == 1) {
				rel[0][i] = di;
				di++;
			} // double
			else if (read_temp.col[i].type == 2) {
				rel[1][i] = vi;
				vi++;
			} // varchar
		}
	}
	void show(int pos, string keyname) {
		int i2 = rel2[keyname]; // get index2 from rel2
		int i1 = 0; // default double
		if (type_ary[i2] == 2) {
			i1 = 1; // varchar, change i1 to 1
		}
		int exi = rel[i1][i2];	// exact index of the array
		if (i1 == 0) {
			cout << rec[pos].ary[exi] << endl;
		}
		else if (i1 == 1) {
			cout << rec[pos].var[exi] << endl;
		}
	}
	int get(int pos, string keyname, string &key1, double &key2) {
		// depends on return value
		int i2 = rel2[keyname];
		int i1 = 0;
		if (type_ary[i2] == 2) {	//varchar 2
			i1 = 1;
		}
		int exi = rel[i1][i2];
		if (i1 == 0) {
			key2 = rec[pos].ary[exi];	// double
		}
		else if (i1 == 1) {
			key1 = string(rec[pos].var[exi]); // varchar
		}
		return type_ary[i2];
	}
	void get_s(int pos, string keyname, string &key1) {
		int i2 = rel2[keyname];
		int i1 = 1;
		int exi = rel[i1][i2];
		key1 = string(rec[pos].var[exi]);
	}
	void get_d(int pos, string keyname, double &key2) {
		int i2 = rel2[keyname];
		int i1 = 0;	//double
		int exi = rel[i1][i2];
		key2 = rec[pos].ary[exi];
	}
	bool search_tuples_exist(string keyname, string key) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) {
			if (m == rcount) {
				break;
			}
			if (rec[i].flag == 1) {
				continue;
			}
			m++;
			string cmp;
			get_s(i, keyname, cmp);
			if (cmp == key) {
				return 1;	// 一旦找到就返回1
			}
		}
		return 0;	// 找不到存在相等的记录
	}
	bool search_tuples_exist(string keyname, double key) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) {
			if (m == rcount) {
				break;
			}
			if (rec[i].flag == 1) {
				continue;
			}
			m++;
			double cmp;
			get_d(i, keyname, cmp);
			if (cmp == key) {
				return 1;
			}
		}
		return 0;
	}
	bool search_tuples(string keyname, string key, int ary[], int &n) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) {
			if (m == rcount) {
				break;
			}
			if (rec[i].flag == 1) {
				continue;
			}
			m++;
			string cmp;
			get_s(i, keyname, cmp);
			if (cmp == key) {
				ary[j] = i;
				j++;
			}
		}
		n = j;
		if (n == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	bool search_tuples(string keyname, double key, int ary[], int &n) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) {
			if (m == rcount) {
				break;
			}
			if (rec[i].flag == 1) {
				continue;
			}
			m++;
			double cmp;
			get_d(i, keyname, cmp);
			if (cmp == key) {
				ary[j] = i;
				j++;
			}
		}
		n = j;
		if (n == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	bool search_tuples_inequal(string keyname, string key, int ary[], int &n) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) {
			if (m == rcount) {
				break;
			}
			if (rec[i].flag == 1) {
				continue;
			}
			m++;
			string cmp;
			get_s(i, keyname, cmp);
			if (cmp == key) {
				ary[j] = i;
				j++;
			}
		}
		n = j;
		if (n == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	bool search_tuples_inequal(string keyname, double key, int ary[], int &n) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) {
			if (m == rcount) {
				break;
			}
			if (rec[i].flag == 1) {
				continue;
			}
			m++;
			double cmp;
			get_d(i, keyname, cmp);
			if (cmp != key) {
				ary[j] = i;
				j++;
			}
		}
		n = j;
		if (n == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}

	void search_smaller_tuples(string keyname, double key, int ary[], int& n) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) {
			if (m == rcount) {
				break;
			}
			if (rec[i].flag == 1) {
				continue;
			}
			m++;
			double cmp;
			get_d(i, keyname, cmp);
			if (cmp < key) {
				ary[j] = i;
				j++; // move further
			}
		}
		n = j;
	}
	void search_bigger_tuples(string keyname, double key, int ary[], int &n) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) {
			if (m == rcount) {
				break;
			}
			if (rec[i].flag == 1) {
				continue;
			}
			m++;
			double cmp;
			get_d(i, keyname, cmp);
			if (cmp > key) {
				ary[j] = i;
				j++;
			}
		}
		n = j;
	}
	void search_bigger_and_smaller_tuples(string keyname, double key1, double key2, int ary[], int &n) {
		int i = 0, j = 0;
		int m = 0;
		for (i = 0; i < 1000; i++) { 
			if (m == rcount) {
				break; // 
			}
			if (rec[i].flag == 1) {
				continue;
			}//被删除的记录跳过
			m++; // 正常的记录数量 m + 1
			double cmp; get_d(i, keyname, cmp);
			if (key1 < cmp && cmp < key2) {
				ary[j] = i;
				j++;
			}
		}
		n = j;
	}
	void insert(double acry[], int d, string asry[], int s) {
		int k = 0; // insert at an emtpy place
		int i = 0, j = 0;
		for (i = 0; i < 1000; i++) {
			if (rec[i].flag == 1) {
				k = i;	// find one delete slot to insert
				break;
			}
		}
		if (k == 0) {
			k = rcount;
		}
		for (i = 0; i < d; i++) {
			rec[k].ary[i] = acry[i]; // copy double array
		}
		for (i = 0; i < s; i++) {
			strcpy(rec[k].var[i], asry[i].c_str());	// copy varchar array
		}
		rec[k].flag = 0; // not empty
		rcount = rcount + 1;
		table c;
		read_meta(tname, c);
		c.rec_count = rcount;
		update_meta(tname, c);
		save();
	}
	int insert_p(double acry[], int d, string asry[], int s) {
		int k = 0; // insert at count
		int i = 0, j = 0;
		for (i = 0; i < 1000; i++) {
			if (rec[i].flag == 1) {
				k = i;	// find one delete slot to insert
				break;
			}
		}
		if (k == 0) {
			k = rcount;
		}

		for (i = 0; i < d; i++) {
			rec[k].ary[i] = acry[i]; // copy double array
		}
		for (i = 0; i < s; i++) {
			strcpy(rec[k].var[i], asry[i].c_str());	// copy varchar array
		}
		rec[k].flag = 0; // not empty
		rcount = rcount + 1;
		table c;
		read_meta(tname, c);
		c.rec_count = rcount;
		update_meta(tname, c);
		save();
		return k;
	}
	void delete_tuple(int ary[], int &n) {
		int i = 0, j = 0;
		for (i = 0; i < n; i++) {
			int x = ary[i];
			rec[x].flag = 1;
		}
		rcount = rcount - n;
		table c;
		read_meta(tname, c);
		c.rec_count = rcount;
		update_meta(tname, c);
		save(); // save delete flag into file
	}
	void change_tuple(string keyname, string key_before, string key_now, int &n) {
		int i = 0, j = 0;
		j = n;
		int i2 = rel2[keyname];
		int i1 = 1;
		int exi = rel[i1][i2];
		strcpy(rec[j].var[exi], key_now.c_str());
		save();
	}
	void change_tuple(string keyname, double key_before, double key_now, int &n) {
		int i = 0, j = 0;
		j = n;
		int i2 = rel2[keyname];
		int i1 = 0;
		int exi = rel[i1][i2];
		rec[j].ary[exi] = key_now;
		save();
	}
	void save() {
		ofstream fp;
		string filepath = "table\\" + tname + "\\file.data";
		fp.open(filepath, ios::binary);
		// 1000 records?
		fp.write((char*)rec, sizeof(rec));
		fp.close();
	}
	string tname;
	int rcount;  // number of records
	int noa;	// number of attributes
	int type_ary[30];
	sin_record rec[1000]; // 1000 records limit
	int rel[2][30]; // double or varchar && index => exact index in record 
	int di; // double类型用了几个
	int vi; // varchar类型用了几个
	map<string, int> rel2;  // change keyname into exact index

};