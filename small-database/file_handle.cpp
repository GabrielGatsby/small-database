#include "file_handle.h"


void read_tabe_meta_my(string tablename, table& tab) {
	string metpath = "table\\" + tablename + "\\met";
	ifstream fp(metpath, ios::binary);
	fp.read((char*)&tab, sizeof(struct table));
	fp.close();
}
void read_meta(string tablename, table &tab) {
	string metpath = "table\\" + tablename + "\\met";
	ifstream fp(metpath, ios::binary);
	fp.read((char*)&tab, sizeof(struct table));
	fp.close();
}
void update_meta(string tablename, table &tab) {
	string metpath = "table\\" + tablename + "\\met";
	ofstream fp(metpath, ios::binary);
	fp.write((char*)&tab, sizeof(struct table));
	fp.close();
}
int find_keyname_position_my(string keyname, table & tab)
{
	for (int i = 0; i < tab.count; i++) {
		string str = tab.col[i].col_name;
		if (!keyname.compare(str))
			return i;
	}
}

vector<string> getFiles(string cate_dir)
{
	vector<string> files;
	_finddata_t file;
	long lf;
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1) {
		cout << cate_dir << " not found!!!" << endl;
	}
	else {
		while (_findnext(lf, &file) == 0) {
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
	sort(files.begin(), files.end());
	return files;
}

int get_main_key_position_my(string tablename, table & tab)
{
	int i;
	for (i = 0; i < tab.count; i++) {
		if (!strcmp(tab.col[i].col_name,tab.main_key)) {
			return i;
		}
	}
}
