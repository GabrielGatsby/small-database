#include "INF.h"


//�������б���
void give_you_all_tables_inf(vector<string>& tables) {
	fstream f;
	f.open("table\\table_list", ios::in | ios::binary);
	string str;
	while (f >> str)
		tables.push_back(str);
	f.close();
	return;
}
void create_meta(string tablename, string btree_pk, vector<string>col_name, vector<int>type, vector<int>size, table table_meta) {

}
//���ұ��Ƿ����
bool search_table_exist_inf(string tablename) {
	fstream f;
	f.open("table\\table_list", ios::in | ios::binary);
	string str;
	while (f >> str) {
		if (!str.compare(tablename)) {
			f.close();
			return true;
		}
	}
	f.close();
	return false;
}

//������
void create_table_inf(string tablename, string btree_pk, vector<string>col_nam, vector<int>typ, vector<int>siz) {
	//��tableǰ�����ж�table�Ƿ����

	//д��table_list
	fstream f;
	f.open("table\\table_list", ios::out | ios::binary | ios::app);
	f << tablename;
	f << '\n';
	f.close();

	char *str;

	//����table�ļ���
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s", tablename.c_str());
	_mkdir(str);
	free(str);

	//������������ֵ�

	table temp;
	strcpy(temp.main_key,btree_pk.c_str());
	temp.count = col_nam.size();
	temp.rec_count = 0;
	strcpy(temp.name, tablename.c_str());
	//temp.last_index = 0;
	int i = 0, j = 0;
	for (i = 0; i < col_nam.size(); i++) {
		strcpy(temp.col[i].col_name, col_nam[i].c_str());
		temp.col[i].type = typ[i];
		temp.col[i].size = siz[i];
	}
	string metpath = "table\\" + tablename + "\\met";
	ofstream mp(metpath, ios::binary);
	mp.write((char*)&temp, sizeof(temp));
	mp.close();

	

	//���������ݴ洢�ļ���
	ofstream fp;
	string filepath = "table\\" + tablename + "\\file.data";
	fp.open(filepath, ios::binary);
	struct sin_record rec[1000];
	fp.write((char*)rec, sizeof(rec));
	fp.close();

	//���������ļ���
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index", tablename.c_str());
	_mkdir(str);
	free(str);

	//��������b+���ļ���	����Ҫ�����ﴴ��
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s", tablename.c_str(), btree_pk.c_str());
	_mkdir(str);
	free(str);

	return;
}

//ɾ����
void drop_table_inf(string tablename) {//ɾtableǰ�����ж�table�Ƿ����
	//ɾ����
	char *str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "rd /s/q table\\%s", tablename.c_str());
	system(str);
	free(str);

	//ɾ��table_list�еı���
	fstream f;
	f.open("table\\table_list", ios::in);
	vector<string> a;
	string s;
	while (f >> s) {
		if (s.compare(tablename))//�ѷ�ɾ����table�ȴ��
			a.push_back(s);
	}
	f.close();
	f.open("table\\table_list", ios::out | ios::trunc);

	for (int i = 0; i < a.size(); i++) {
		f << a[i];
		f << '\n';
	}
	f.close();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

//�����ļ��л��ļ��Ƿ����
bool something_exist(char* path){
	int flag = _access(path, 0);
	if (flag == 0)
		return 1;
	if (flag == -1)
		return 0;
}

//�����ļ����·����ļ����ļ���
void get_all_something_under(char * path, vector<string>& something)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;
	// \\* ����Ҫ�������е�����,��ĳ�\\*.jpg��ʾ����jpg�����ļ�
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) {
		return;
	}
	do
	{
		if (strcmp(fileInfo.name, ".") && strcmp(fileInfo.name, "..")) {
			something.push_back(fileInfo.name);
		}
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}


FILEP get_raddress_from_filename(string filename)
{
	filename.erase(filename.begin(), filename.begin() + FILE_FIRST_LENGTH);
	filename.erase(filename.end() - FILE_LAST_LENGTH, filename.end());
	int a = atoi(filename.c_str());
	return a;
}

//����������bptree��
void create_index_inf1(string tablename, string keyname)//�����ж�table�Ƿ����,�����ж�index�Ƿ����
{
	//�����ļ���
	char* str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s", tablename.c_str(), keyname.c_str());
	_mkdir(str);
	free(str);

	table table_meta;
	read_tabe_meta_my(tablename, table_meta);
	int key_positon = find_keyname_position_my(keyname, table_meta);//�ҵ������������������ĸ�λ�ã���-1��

	if (table_meta.col[key_positon].type == 1) {//���ؼ�����double��
		BPlusTree btree;//����
		btree.ReadBPlusTree(tablename, keyname);

		vector<FILEP> points_to;
		search_tuples_all_inf(tablename, points_to);

		for (int i = 0; i < points_to.size(); i++) {

			TRecord trecord;//������record

			trecord.Raddress = points_to[i];//�����¼��ַ

			Record rec;
			read_record_my(tablename, points_to[i], rec, table_meta);//��ȡ��¼
			trecord.key =*(double*)rec.data[key_positon];//����ؼ���
			btree.Insert_BPlus_Tree(trecord);
		}
	}
	else {//���ؼ�����neochar��ʱ
		BPlusTreeC btree;//����
		btree.ReadBPlusTree(tablename, keyname);

		vector<FILEP> points_to;
		search_tuples_all_inf(tablename, points_to);

		for (int i = 0; i < points_to.size(); i++) {

			TRecordC trecord;//������record
			
			trecord.Raddress = points_to[i];//�����¼��ַ

			Record rec;
			read_record_my(tablename, points_to[i], rec, table_meta);//��ȡ��¼
			trecord.key = (char*)rec.data[key_positon];//����ؼ���
			btree.Insert_BPlus_Tree(trecord);
		}
	}
	
}

//ɾ��������bptree)
void delete_index_inf1(string tablename, string keyname)//ǧ����ɾ���������������������ȴ������ֵ�ȷ�ϡ�
{
	char* str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	if (search_index_exist_inf(tablename, keyname)) {//��������
		sprintf(str, "rd /s/q table\\%s\\index\\bptree_%s", tablename.c_str(), keyname.c_str());
		system(str);
	}
	free(str);
}

//��ѯ���ڵ�����
void search_all_indexs_inf(string tablename, vector<string>& all_indexs)
{
	char *str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index", tablename.c_str());
	get_all_something_under(str, all_indexs);
	free(str);
}

//��ѯĿ�����������Ƿ����
bool search_index_exist_inf(string tablename, string keyname)
{
	char *str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s", tablename.c_str(),keyname.c_str());
	bool exist = something_exist(str);
	free(str);
	return exist;
}

bool create_index_inf(string tablename, string keyname)
{
	if (search_index_exist_inf(tablename, keyname)) {
		//����������ˣ�ɶ������
		return false;
	}
	else {
		create_index_inf1(tablename, keyname);
		return true;
	}
}

bool delete_index_inf(string tablename, string keyname)
{
	table table_meta;
	read_tabe_meta_my(tablename, table_meta);

	if (table_meta.main_key == keyname) {//�޷�ɾ����������
		return false;
	}
	else if(!search_index_exist_inf(tablename, keyname)){//û��������Ե�����
		return false;
	}
	else {//����ɾ����������
		delete_index_inf1(tablename, keyname);
		return true;
	}
}

void read_table_meta_inf(string tablename, table & tab)
{
	read_tabe_meta_my(tablename, tab);
}

void read_record_inf(string tablename, FILEP point_to, Record & record)
{
	table table_meta;
	read_tabe_meta_my(tablename, table_meta);
	read_record_my(tablename, point_to, record, table_meta);
}

void read_records_inf(string tablename, vector<FILEP> points_to, vector < Record>&records) {
	table temp;
	read_meta(tablename, temp);
	record_wrapper a(tablename);
	for (int i = 0; i < points_to.size(); i++) {
		Record rec;
		strcpy(rec.table_name, tablename.c_str());
		rec.rec_addr = points_to[i];
		rec.count = temp.count;
		int j = 0; int k = 0;
		for (j = 0; j < rec.count; j++) {
			double c; char d[50 + 1];
			k = points_to[i];
			string keyname(temp.col[j].col_name);
			if (temp.col[j].type == 1) {
				double key2;
				a.get_d(k, keyname, key2);
				rec.data[j] = (double*)malloc(sizeof(double));
				*((double*)rec.data[j]) = key2;
			}
			else if (temp.col[j].type == 2) {
				string key1;
				a.get_s(k, keyname, key1);
				rec.data[j] = malloc(sizeof(char) * (50 + 1));
				strcpy((char*)rec.data[j], key1.c_str());
			}
		} // read from file%d.dat
		records.push_back(rec);
	}
}


int exist_attr_in_table(string tablename, string attr_name) {
	table temp;
	read_meta(tablename, temp);
	for (int i = 0; i < temp.count; i++) {
		if (strcmp(attr_name.c_str(), temp.col[i].col_name) == 0) {
			return temp.col[i].type;
		}
	}
	return -1;
}

bool search_tuples_exist_inf(string tablename, string keyname, string key)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		return search_tuples_exist_inf1(tablename, keyname, key);
	}
	else {//����������
		return search_tuples_exist_noindex(tablename, keyname, key);
	}
}

bool search_tuples_exist_inf(string tablename, string keyname, double key)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		return search_tuples_exist_inf1(tablename, keyname, key);
	}
	else {//����������
		return search_tuples_exist_noindex(tablename, keyname, key);
	}
}

bool search_tuples_inf(string tablename, string keyname, string key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		return search_tuples_inf2(tablename, keyname, key, points_to);
	}
	else {//����������
		return search_tuples_noindex(tablename, keyname, key, points_to);
	}
}

bool search_tuples_inf(string tablename, string keyname, double key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		return search_tuples_inf2(tablename, keyname, key, points_to);
	}
	else {//����������
		return search_tuples_noindex(tablename, keyname, key, points_to);
	}
}

bool search_tuples_not_equal_inf(string tablename, string keyname, double key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		return search_tuples_not_equal_inf2(tablename, keyname, key, points_to);
	}
	else {//����������
		return search_tuples_not_equal_noindex(tablename, keyname, key, points_to);
	}
}

bool search_tuples_not_equal_inf(string tablename, string keyname, string key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		return search_tuples_not_equal_inf2(tablename, keyname, key, points_to);
	}
	else {//����������
		return search_tuples_not_equal_noindex(tablename, keyname, key, points_to);
	}
}

bool search_tuples_all_inf(string tablename, vector<FILEP>& points_to)
{
	table table_meta;
	read_meta(tablename, table_meta);
	int type = exist_attr_in_table(tablename, table_meta.main_key);
	return search_tuples_all_inf2(tablename, table_meta.main_key, type,points_to);
}


void search_smaller_tuples_inf(string tablename, string keyname, double key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		search_smaller_tuples_inf2(tablename, keyname, key, points_to);
	}
	else {//����������
		search_smaller_tuples_noindex(tablename, keyname, key, points_to);
	}
}

void search_bigger_tuples_inf(string tablename, string keyname, double key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		search_bigger_tuples_inf2(tablename, keyname, key, points_to);
	}
	else {//����������
		search_bigger_tuples_noindex(tablename, keyname, key, points_to);
	}
}

void search_bigger_and_smaller_tuples_inf(string tablename, string keyname, double key1, double key2, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree��������
		search_bigger_and_smaller_tuples_inf2(tablename, keyname, key1, key2, points_to);
	}
	else {//����������
		search_bigger_and_smaller_tuples_noindex(tablename, keyname, key1, key2, points_to);
	}
}

string get_main_key(string tablename)
{
	table table_meta;
	read_table_meta_inf(tablename, table_meta);
	return table_meta.main_key;
}

vector<string> get_all_attr(string tablename) {
	vector<string> attrs;
	table temp;
	read_meta(tablename, temp);
	for (int i = 0; i < temp.count; i++) {
		string str(temp.col[i].col_name);
		attrs.push_back(str);
	}
	return attrs;
}

FILEP insert_tuple_(string tablename, Record record)//�����¼������
{
	record_wrapper a(tablename);

	table temp;
	read_tabe_meta_my(tablename, temp);
	int j = 0, m = 0, n = 0;
	double ary[30];
	string sary[30] = { "","","" };
	for (j = 0; j < temp.count; j++) {
		double x; char y[100];
		if (temp.col[j].type == 1) {
			x = *(double*)record.data[j];
			ary[m] = x;
			m++;
		}
		else if (temp.col[j].type == 2) {
			strcpy(y, (char*)record.data[j]);
			sary[n] = string(y);
			n++;
		}
	}

	int address = a.insert_p(ary, m, sary, n);
	//update the dictionary
	return FILEP(address);

}



void delete_tuples_inf(string tablename, vector<FILEP>& points_to)
{
	delete_tuples_inf1(tablename, points_to);//ɾ�������ڵļ�¼��ַ

	delete_tuples_from_dicks(tablename, points_to);//ɾ����¼
}

bool insert_tuple_inf(string tablename, Record & record)
{
	//��Ҫ�鿴�Ƿ������ظ�
	table table_meta;
	read_tabe_meta_my(tablename, table_meta);
	int i = find_keyname_position_my(table_meta.main_key,table_meta);//��¼����λ��
	
	bool is_exist = 0;//Ĭ������û���ظ�
	if (table_meta.col[i].type == 1) {//����doubleʱ
		is_exist = search_tuples_exist_inf(tablename, table_meta.col[i].col_name, *(double*)record.data[i]);
	}
	if (table_meta.col[i].type == 2) {//����neocharʱ
		is_exist = search_tuples_exist_inf(tablename, table_meta.col[i].col_name, (char*)record.data[i]);
	}

	if (is_exist) {
		return false;
	}

	else {
		FILEP address = insert_tuple_(tablename, record);//�����¼�����ؼ�¼��ַ
		insert_tuple_inf1(tablename, address);
	}
}


//void change_tuples_inf(string tablename, string keyname, string key_before, string key_now, vector<FILEP> points_to)
//{
//	change_tuples_inf1(tablename, keyname, key_before, key_now, points_to);//���������ڵļ�¼��ַ��Ӧ������ֵ
//
//	change_tuples_from_dicks(tablename, keyname, key_before, key_now, points_to);//���¼�¼
//
//}
//
//void change_tuples_inf(string tablename, string keyname, double key_before, double key_now, vector<FILEP> points_to)
//{
//	change_tuples_inf1(tablename, keyname, key_before, key_now, points_to);//���������ڵļ�¼��ַ��Ӧ������ֵ
//
//	change_tuples_from_dicks(tablename, keyname, key_before, key_now, points_to);//���¼�¼
//}

void change_tuple_inf(string tablename, string keyname, string key_before, string key_now, FILEP point_to)
{
	table table_meta;
	read_table_meta_inf(tablename, table_meta);
	if (!keyname.compare(table_meta.main_key)) {//Ҫ�޸ĵ�����������
		vector<FILEP> points_to_pk;
		search_tuples_inf(tablename, keyname, key_now, points_to_pk);
		if (points_to_pk.size()) {//��������Ҫ�޸ĳ�Ϊ������
			return;
		}
	}
	change_tuple_inf1(tablename, keyname, key_before, key_now, point_to);
	change_tuple_from_disk(tablename, keyname, key_before, key_now, point_to);

}

void change_tuple_inf(string tablename, string keyname, double key_before, double key_now, FILEP point_to)
{
	table table_meta;
	read_table_meta_inf(tablename, table_meta);
	if (!keyname.compare(table_meta.main_key)) {//Ҫ�޸ĵ�����������
		vector<FILEP> points_to_pk;
		search_tuples_inf(tablename, keyname, key_now, points_to_pk);
		if (points_to_pk.size()) {//��������Ҫ�޸ĳ�Ϊ������
			return;
		}
	}
	change_tuple_inf1(tablename, keyname, key_before, key_now, point_to);
	change_tuple_from_disk(tablename, keyname, key_before, key_now, point_to);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ȡ�����¼��ַ�������ļ��еļ�¼��ַ
void read_inid(string tablename, string keyname, TSearchResult tresult, vector<FILEP> &points_to) {
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), tresult.Raddress);
	ifstream in_file(str, ios::in | ios::binary);
	free(str);

	FILEP _point = 0;
	while (1) {
		in_file.read((char*)&_point, sizeof(_point));
		if (in_file.eof()) break;//������ĩβʱ�ͽ���
		points_to.push_back(_point);

	}

	in_file.close();
}

//��TSearchResult�ж���¼��ַ����ַ������
void from_result_add_to_points(string tablename, string keyname, TSearchResult tresult, vector<FILEP> &points_to) {
	if (tresult.exist) {
		if (tresult.ismul) {//���ؼ���ָ������¼��ַ������
			read_inid(tablename, keyname, tresult, points_to);
		}
		else {//���ؼ���ָ�򵥸���¼��ַ
			FILEP _point;
			_point = tresult.Raddress;
			points_to.push_back(_point);
		}
	}
}

//��TSearchResult�����ж���¼��ַ����ַ������
void from_results_add_to_points(string tablename, string keyname, vector<TSearchResult> &results, vector<FILEP> &points_to) {
	for (int i = 0; i < results.size(); i++) {
		from_result_add_to_points(tablename, keyname, results[i], points_to);
	}
}

//�жϵ�ǰ�ؼ��ֵ�Ԫ���Ƿ����
bool search_tuples_exist_inf1(string tablename, string keyname, string key) {
	TSearchResult tresult;
	TRecordC trecord;
	trecord.key = key;
	BPlusTreeC btree;
	btree.ReadBPlusTree(tablename, keyname);
	btree.Search_BPlus_Tree(trecord, tresult);
	if (tresult.exist)
		return true;
	return false;
}
bool search_tuples_exist_inf1(string tablename, string keyname, double key) {
	TSearchResult tresult;
	TRecord trecord;
	trecord.key = key;
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);
	btree.Search_BPlus_Tree(trecord, tresult);
	if (tresult.exist)
		return true;
	return false;
}

//���ҵ�ǰ�ؼ���Ԫ�飨����TSearchResult��
bool search_tuples_inf1(string tablename, string keyname, string key, TSearchResult &tresult) {//����ǰ�����ж�table�Ƿ����
	BPlusTreeC btree;
	btree.ReadBPlusTree(tablename, keyname);
	TRecordC trecord;
	trecord.key = key;
	btree.Search_BPlus_Tree(trecord, tresult);//�Ѳ�ѯ������ص�tresult��
	return tresult.exist;//���عؼ����Ƿ����
}
bool search_tuples_inf1(string tablename, string keyname, double key, TSearchResult &tresult) {//����ǰ�����ж�table�Ƿ����
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);
	TRecord trecord;
	trecord.key = key;
	btree.Search_BPlus_Tree(trecord, tresult);//�Ѳ�ѯ������ص�tresult��
	return tresult.exist;//���عؼ����Ƿ����
}

bool search_tuples_not_equal_inf1(string tablename, string keyname, string key, vector<TSearchResult> &results)
{
	BPlusTreeC btree;
	btree.ReadBPlusTree(tablename, keyname);

	BPlusNodeC node;
	btree.ReadBPlusNode(btree.get_root(), node);

	//��λ������ߵ�Ҷ�ڵ�
	while (!node.isleaf)
	{
		btree.ReadBPlusNode(node.pointers[0].second, node);
	}

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//�ض�������
		for (int i = 0; i < node.nkey; i++) {
			tresult.ismul = node.pointers[i].first;
			tresult.Raddress = node.pointers[i].second;
			results.push_back(tresult);
		}

		if (node.pointers[MAX_KEY].second == 0)//û����һ��Ҷ�ڵ���,break
			break;

		btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
	}
	return results.size();
}

bool search_tuples_not_equal_inf2(string tablename, string keyname, string key, vector<FILEP>& points_to)
{
	vector<TSearchResult> results;
	search_tuples_not_equal_inf1(tablename, keyname, key, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
	return points_to.size();
}

bool search_tuples_not_equal_inf1(string tablename, string keyname, double key, vector<TSearchResult>& results)
{
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);

	BPlusNode node;
	btree.ReadBPlusNode(btree.get_root(), node);

	//��λ������ߵ�Ҷ�ڵ�
	while (!node.isleaf)
	{
		btree.ReadBPlusNode(node.pointers[0].second, node);
	}

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//�ض�������
		for (int i = 0; i < node.nkey; i++) {
			tresult.ismul = node.pointers[i].first;
			tresult.Raddress = node.pointers[i].second;
			results.push_back(tresult);
		}

		if (node.pointers[MAX_KEY].second == 0)//û����һ��Ҷ�ڵ���,break
			break;

		btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
	}
	return results.size();
}

bool search_tuples_not_equal_inf2(string tablename, string keyname, double key, vector<FILEP>& points_to)
{
	vector<TSearchResult> results;
	search_tuples_not_equal_inf1(tablename, keyname, key, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
	return points_to.size();
}

bool seatch_tuples_all_inf1(string tablename, string keyname, int type, vector<TSearchResult> &tresults)
{
	if (type == 1) {
		BPlusTree btree;
		btree.ReadBPlusTree(tablename, keyname);

		BPlusNode node;
		btree.ReadBPlusNode(btree.get_root(), node);

		//��λ������ߵ�Ҷ�ڵ�
		while (!node.isleaf)
		{
			btree.ReadBPlusNode(node.pointers[0].second, node);
		}

		while (1)
		{
			TSearchResult tresult;
			tresult.exist = true;//�ض�������
			for (int i = 0; i < node.nkey; i++) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				tresults.push_back(tresult);
			}

			if (node.pointers[MAX_KEY].second == 0)//û����һ��Ҷ�ڵ���,break
				break;

			btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
		}
	}
	if (type == 2) {
		BPlusTreeC btree;
		btree.ReadBPlusTree(tablename, keyname);

		BPlusNodeC node;
		btree.ReadBPlusNode(btree.get_root(), node);

		//��λ������ߵ�Ҷ�ڵ�
		while (!node.isleaf)
		{
			btree.ReadBPlusNode(node.pointers[0].second, node);
		}

		while (1)
		{
			TSearchResult tresult;
			tresult.exist = true;//�ض�������
			for (int i = 0; i < node.nkey; i++) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				tresults.push_back(tresult);
			}

			if (node.pointers[MAX_KEY].second == 0)//û����һ��Ҷ�ڵ���,break
				break;

			btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
		}
	}
	return tresults.size();
}

bool search_tuples_all_inf2(string tablename, string keyname, int type, vector<FILEP>& points_to)
{
	vector<TSearchResult> results;
	seatch_tuples_all_inf1(tablename, keyname, type, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
	return points_to.size();
}





//���ҵ�ǰ�ؼ���Ԫ�飨����vector<FILEP>)
bool search_tuples_inf2(string tablename, string keyname, string key, vector<FILEP> &points_to) {//����ǰ�����ж�table�Ƿ����
	TSearchResult tresult;
	search_tuples_inf1(tablename, keyname, key, tresult);
	if (tresult.exist) {
		from_result_add_to_points(tablename, keyname, tresult, points_to);
	}
	return tresult.exist;//���عؼ����Ƿ����
}
bool search_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to) {//����ǰ�����ж�table�Ƿ����
	TSearchResult tresult;
	search_tuples_inf1(tablename, keyname, key, tresult);
	if (tresult.exist) {
		from_result_add_to_points(tablename, keyname, tresult, points_to);
	}
	return tresult.exist;//���عؼ����Ƿ����
}


//����С�ڸùؼ��ֵ�Ԫ��(ֻ�����ڹؼ���Ϊ���Σ�������vector<TSearchResult>��
void search_smaller_tuples_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results) {
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);

	BPlusNode node;
	btree.ReadBPlusNode(btree.get_root(), node);

	//��λ������ߵ�Ҷ�ڵ�
	while (!node.isleaf)
	{
		btree.ReadBPlusNode(node.pointers[0].second, node);
	}

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//�ض�������
		bool flag_no_more_smaller = false;
		for (int i = 0; i < node.nkey; i++) {
			if (node.keys[i] < key) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				results.push_back(tresult);
			}
			else {
				flag_no_more_smaller = true;
				break;
			}
		}

		if (flag_no_more_smaller)//û��С�ڵ��ڵ�keyֵ��,break
			break;

		if (node.pointers[MAX_KEY].second == 0)//û����һ��Ҷ�ڵ���,break
			break;

		btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
	}
}


//���Ҵ��ڸùؼ��ֵ�Ԫ��(ֻ�����ڹؼ���Ϊ���Σ�������vector<TSearchResult>��
void search_bigger_tuples_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results) {
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);

	TRecord trecord;
	TSearchResult result_temp;
	trecord.key = key;
	btree.Search_BPlus_Tree_Bigger(trecord, result_temp);//�Ѳ�ѯ������ص�tresult��

	BPlusNode node;
	btree.ReadBPlusNode(result_temp.Baddress, node);//������

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//�ض�������
		for (int i = 0; i < node.nkey; i++) {
			if (node.keys[i] > key) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				results.push_back(tresult);
			}
			else if (node.keys[i] <= key) {
				//ʲô����������Ȼ����
			}
			else {
				break;
			}
		}

		if (node.pointers[MAX_KEY].second == 0)//û����һ��Ҷ�ڵ���,break
			break;

		btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
	}

}

//���Ҵ���key1С��key2��Ԫ��(ֻ�����ڹؼ���Ϊ���Σ�������vector<TSearchResult>��
void search_bigger_and_smaller_tuples_inf1(string tablename, string keyname, double key1, double key2, vector<TSearchResult> &results) {
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);

	TRecord trecord;
	TSearchResult result_temp;
	trecord.key = key1;
	btree.Search_BPlus_Tree_Bigger(trecord, result_temp);//�Ѳ�ѯ������ص�tresult��

	BPlusNode node;
	btree.ReadBPlusNode(result_temp.Baddress, node);//������

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//�ض�������
		bool flag_no_more_smaller = false;
		for (int i = 0; i < node.nkey; i++) {
			if (node.keys[i] > key1 && node.keys[i] < key2) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				results.push_back(tresult);
			}
			else if (node.keys[i] <= key1) {
				//ʲô����������Ȼ����
			}
			else {
				flag_no_more_smaller = true;
				break;
			}
		}

		if (flag_no_more_smaller)//û��С�ڵ��ڵ�keyֵ��,break
			break;

		if (node.pointers[MAX_KEY].second == 0)//û����һ��Ҷ�ڵ���,break
			break;

		btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
	}

}

//����С�ڸùؼ��ֵ�Ԫ��(ֻ�����ڹؼ���Ϊdouble��������vector<FILEP>��
void search_smaller_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	vector<TSearchResult> results;
	search_smaller_tuples_inf1(tablename, keyname, key, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
}

//���Ҵ��ڸùؼ��ֵ�Ԫ��(ֻ�����ڹؼ���Ϊdouble��������vector<FILEP>��
void search_bigger_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	vector<TSearchResult> results;
	search_bigger_tuples_inf1(tablename, keyname, key, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
}

//���Ҵ���key1С��key2��Ԫ��(ֻ�����ڹؼ���Ϊdouble��������vector<FILEP>��
void search_bigger_and_smaller_tuples_inf2(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to) {
	vector<TSearchResult> results;
	search_bigger_and_smaller_tuples_inf1(tablename, keyname, key1, key2, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
}

//���루���Ȳ����¼���õ���¼��ַ���ٲ���������
void insert_tuple_inf1(string tablename, FILEP address)
{
	vector<string> all_indexs;//��ȡ��������
	search_all_indexs_inf(tablename, all_indexs);
	
	table table_meta;//��ȡ�����ֵ�
	read_tabe_meta_my(tablename, table_meta);
	
	Record record_this;//��ȡ�����¼
	read_record_my(tablename, address, record_this, table_meta);

	for (int i = 0; i < all_indexs.size(); i++) {
		for (int j = 0; j < table_meta.count; j++) {
			string btree_name = "bptree_";
			string keyname = table_meta.col[j].col_name;
			btree_name = btree_name + keyname;
			if (!all_indexs[i].compare(btree_name)) {//�ҵ�������
				if (table_meta.col[j].type == 1) {//��������������double��
					BPlusTree btree;
					btree.ReadBPlusTree(tablename, keyname);
					TRecord trecord;
					trecord.key = *(double*)record_this.data[j];//�ؼ�������
					trecord.Raddress = address;//��¼��ַ����
					btree.Insert_BPlus_Tree(trecord);//�����¼
				}
				else {//������������"string"��
					BPlusTreeC btree;
					btree.ReadBPlusTree(tablename, keyname);
					TRecordC trecord;
					trecord.key = (char*)record_this.data[j];//�ؼ�������
					trecord.Raddress = address;//��¼��ַ����
					btree.Insert_BPlus_Tree(trecord);//�����¼
				}
				break;
			}
		}
	}
	
}

//ɾ�������Ȳ��Ҽ�¼���õ���¼��ַ����ɾ������������ɾ����������ɾ���ļ���
void delete_tuple_inf1(string tablename, FILEP address)
{
	vector<string> all_indexs;//��ȡ��������
	search_all_indexs_inf(tablename, all_indexs);

	table table_meta;//��ȡ�����ֵ�
	read_tabe_meta_my(tablename, table_meta);

	Record record_this;//��ȡ�����¼
	read_record_my(tablename, address, record_this, table_meta);

	for (int i = 0; i < all_indexs.size(); i++) {
		for (int j = 0; j < table_meta.count; j++) {
			string btree_name = "bptree_";
			string keyname = table_meta.col[j].col_name;
			btree_name = btree_name + keyname;
			if (!all_indexs[i].compare(btree_name)) {//�ҵ�������
				if (table_meta.col[j].type == 1) {//��������������double��
					BPlusTree btree;
					btree.ReadBPlusTree(tablename, keyname);
					TRecord trecord;
					trecord.key = *(double*)record_this.data[j];//�ؼ�������
					trecord.Raddress = address;//��¼��ַ����
					btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//ɾ����¼
				}
				else {//������������"neochar"��
					BPlusTreeC btree;
					btree.ReadBPlusTree(tablename, keyname);
					TRecordC trecord;
					trecord.key = (char*)record_this.data[j];//�ؼ�������
					trecord.Raddress = address;//��¼��ַ����
					btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//ɾ����¼
				}
				break;
			}
		}
	}

}

void delete_tuples_inf1(string tablename, vector<FILEP>& points_to)
{
	vector<string> all_indexs;//��ȡ��������
	search_all_indexs_inf(tablename, all_indexs);

	table table_meta;//��ȡ�����ֵ�
	read_tabe_meta_my(tablename, table_meta);

	for (int i = 0; i < all_indexs.size(); i++) {
		for (int j = 0; j < table_meta.count; j++) {
			string btree_name = "bptree_";
			string keyname = table_meta.col[j].col_name;
			btree_name = btree_name + keyname;
			if (!all_indexs[i].compare(btree_name)) {//�ҵ�������
				if (table_meta.col[j].type == 1) {//��������������double��
					BPlusTree btree;
					btree.ReadBPlusTree(tablename, keyname);//�Ƚ��������Ͳ����ظ�������
					for (int k = 0; k < points_to.size(); k++) {
						Record record_this;//��ȡ�����¼
						read_record_my(tablename, points_to[k], record_this, table_meta);

						TRecord trecord;
						trecord.key = *(double*)record_this.data[j];//�ؼ�������
						trecord.Raddress = points_to[k];//��¼��ַ����
						btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//ɾ����¼

					}
				}
				else {//������������"neochar"��
					BPlusTreeC btree;
					btree.ReadBPlusTree(tablename, keyname);//�Ƚ��������Ͳ����ظ�������
					for (int k = 0; k < points_to.size(); k++) {
						Record record_this;//��ȡ�����¼
						read_record_my(tablename, points_to[k], record_this, table_meta);

						TRecordC trecord;
						trecord.key = (char*)record_this.data[j];//�ؼ�������
						trecord.Raddress = points_to[k];//��¼��ַ����
						btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//ɾ����¼

					}
				}
				break;
			}
		}
	}

}

void change_tuple_inf1(string tablename, string keyname, string key_before, string key_now, FILEP address)
{
	if(search_index_exist_inf(tablename, keyname)) {
		BPlusTreeC btree;
		btree.ReadBPlusTree(tablename, keyname);
		TRecordC trecord;

		trecord.key = key_before;
		trecord.Raddress = address;
		btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//ɾ���ɹؼ��ֵļ�¼��ַ
		trecord.key = key_now;
		btree.Insert_BPlus_Tree(trecord);//�����¹ؼ��ֵļ�¼��ַ
	}
}

void change_tuple_inf1(string tablename, string keyname, double key_before, double key_now, FILEP address)
{
	if (search_index_exist_inf(tablename, keyname)) {
		BPlusTree btree;
		btree.ReadBPlusTree(tablename, keyname);
		TRecord trecord;

		trecord.key = key_before;
		trecord.Raddress = address;
		btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//ɾ���ɹؼ��ֵļ�¼��ַ
		trecord.key = key_now;
		btree.Insert_BPlus_Tree(trecord);//�����¹ؼ��ֵļ�¼��ַ
	}
}

void change_tuples_inf1(string tablename, string keyname, string key_before, string key_now, vector<FILEP> &points_to)
{
	BPlusTreeC btree;
	btree.ReadBPlusTree(tablename, keyname);
	TRecordC trecord;

	trecord.key = key_before;
	for (int i = 0; i < points_to.size(); i++) {
		trecord.Raddress = points_to[i];
		btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//ɾ���ɹؼ��ֵļ�¼��ַ
	}
	trecord.key = key_now;
	for (int i = 0; i < points_to.size(); i++) {
		trecord.Raddress = points_to[i];
		btree.Insert_BPlus_Tree(trecord);
	}
}

void change_tuples_inf1(string tablename, string keyname, double key_before, double key_now, vector<FILEP>& points_to)
{
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);
	TRecord trecord;

	trecord.key = key_before;
	for (int i = 0; i < points_to.size(); i++) {
		trecord.Raddress = points_to[i];
		btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//ɾ���ɹؼ��ֵļ�¼��ַ
	}
	trecord.key = key_now;
	for (int i = 0; i < points_to.size(); i++) {
		trecord.Raddress = points_to[i];
		btree.Insert_BPlus_Tree(trecord);
	}
}

bool search_tuples_exist_noindex(string tablename, string keyname, string key) {
	record_wrapper a(tablename);
	return a.search_tuples_exist(keyname, key);
	
}//��������������£�����ָ���ؼ��ֵļ�¼�Ƿ����
bool search_tuples_exist_noindex(string tablename, string keyname, double key) {
	record_wrapper a(tablename);
	return a.search_tuples_exist(keyname, key);
}//��������������£�����ָ���ؼ��ֵļ�¼�Ƿ����

bool search_tuples_noindex(string tablename, string keyname, string key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_tuples(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
	return points_to.size();
}//��������������£��õ�ָ���ؼ��ֵļ�¼��ַ���洢��vector<FILEP> &points_to��
bool search_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_tuples(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
	return points_to.size();
}//��������������£��õ�ָ���ؼ��ֵļ�¼��ַ���洢��vector<FILEP> &points_to��

void search_smaller_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_smaller_tuples(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
}//��������������£��õ��ؼ���С��key�ļ�¼��ַ���洢��vector<FILEP> &points_to�У�
void search_bigger_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_bigger_tuples(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
}//��������������£��õ��ؼ��ִ���key�ļ�¼��ַ���洢��vector<FILEP> &points_to�У�
void search_bigger_and_smaller_tuples_noindex(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n = 0;
	a.search_bigger_and_smaller_tuples(keyname, key1, key2, ary, n);
	for (int i = 0; i < n; i++) {
		ary[i] = points_to[i];
	}
}//��������������£��õ��ؼ��ִ���key1��С��key2�ļ�¼��ַ���洢��vector<FILEP> &points_to�У�

void delete_tuples_from_dicks(string tablename, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n = points_to.size();
	for (int i = 0; i < n; i++) {
		ary[i] = points_to[i];
	}
	a.delete_tuple(ary, n);
}//ɾ��ָ����ַ�ļ�¼��ע�⣬�Ƕ����¼��

void change_tuples_from_dicks(string tablename, string keyname, string key_before, string key_now, vector<FILEP> &points_to) {
	int ary[1000]; int n = points_to.size();
	for (int i = 0; i < n; i++) {
		ary[i] = points_to[i];
	}
	record_wrapper a(tablename);
	//a.change_tuple(keyname, key_before, key_now, ary, n);
}//����ָ����ַ�ļ�¼��ע�⣬�Ƕ����¼��
void change_tuples_from_dicks(string tablename, string keyname, double key_before, double key_now, vector<FILEP> &points_to) {
	int ary[1000]; int n = points_to.size();
	for (int i = 0; i < n; i++) {
		ary[i] = points_to[i];
	}
	record_wrapper a(tablename);
	//a.change_tuple(keyname, key_before, key_now, ary, n);
}//����ָ����ַ�ļ�¼��ע�⣬�Ƕ����¼��

void change_tuple_from_disk(string tablename, string keyname, string key_before, string key_now, FILEP &points_to) {
	int n = FILEP(points_to);
	record_wrapper a(tablename);
	a.change_tuple(keyname, key_before, key_now, n);
}
void change_tuple_from_disk(string tablename, string keyname, double key_before, double key_now, FILEP &points_to) {
	int n = FILEP(points_to);
	record_wrapper a(tablename);
	a.change_tuple(keyname, key_before, key_now, n);
}

bool search_tuples_not_equal_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_tuples_inequal(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
	if (points_to.size() == 0) {
		return 0;
	}
	return 1;
}//��������������£��õ�ָ���ؼ��ֲ���ȵļ�¼��ַ���洢��vector<FILEP> &points_to��
bool search_tuples_not_equal_noindex(string tablename, string keyname, string key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_tuples_inequal(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
	if (points_to.size() == 0) {
		return 0;
	}
	return 1;
}