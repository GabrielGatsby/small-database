#include "INF.h"


//传递所有表名
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
//查找表是否存在
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

//创建表
void create_table_inf(string tablename, string btree_pk, vector<string>col_nam, vector<int>typ, vector<int>siz) {
	//建table前请先判断table是否存在

	//写入table_list
	fstream f;
	f.open("table\\table_list", ios::out | ios::binary | ios::app);
	f << tablename;
	f << '\n';
	f.close();

	char *str;

	//创建table文件夹
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s", tablename.c_str());
	_mkdir(str);
	free(str);

	//创建表的数据字典

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

	

	//创建表数据存储文件夹
	ofstream fp;
	string filepath = "table\\" + tablename + "\\file.data";
	fp.open(filepath, ios::binary);
	struct sin_record rec[1000];
	fp.write((char*)rec, sizeof(rec));
	fp.close();

	//创建索引文件夹
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index", tablename.c_str());
	_mkdir(str);
	free(str);

	//创建主码b+树文件夹	这里要在这里创建
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s", tablename.c_str(), btree_pk.c_str());
	_mkdir(str);
	free(str);

	return;
}

//删除表
void drop_table_inf(string tablename) {//删table前请先判断table是否存在
	//删除表
	char *str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "rd /s/q table\\%s", tablename.c_str());
	system(str);
	free(str);

	//删除table_list中的表名
	fstream f;
	f.open("table\\table_list", ios::in);
	vector<string> a;
	string s;
	while (f >> s) {
		if (s.compare(tablename))//把非删除的table先存放
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

//查找文件夹或文件是否存在
bool something_exist(char* path){
	int flag = _access(path, 0);
	if (flag == 0)
		return 1;
	if (flag == -1)
		return 0;
}

//查找文件夹下方的文件或文件夹
void get_all_something_under(char * path, vector<string>& something)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;
	// \\* 代表要遍历所有的类型,如改成\\*.jpg表示遍历jpg类型文件
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

//创建索引（bptree）
void create_index_inf1(string tablename, string keyname)//请先判断table是否存在,请先判断index是否存在
{
	//创建文件夹
	char* str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s", tablename.c_str(), keyname.c_str());
	_mkdir(str);
	free(str);

	table table_meta;
	read_tabe_meta_my(tablename, table_meta);
	int key_positon = find_keyname_position_my(keyname, table_meta);//找到创建索引的属性在哪个位置（列-1）

	if (table_meta.col[key_positon].type == 1) {//当关键字是double型
		BPlusTree btree;//建树
		btree.ReadBPlusTree(tablename, keyname);

		vector<FILEP> points_to;
		search_tuples_all_inf(tablename, points_to);

		for (int i = 0; i < points_to.size(); i++) {

			TRecord trecord;//给树的record

			trecord.Raddress = points_to[i];//载入记录地址

			Record rec;
			read_record_my(tablename, points_to[i], rec, table_meta);//读取记录
			trecord.key =*(double*)rec.data[key_positon];//载入关键字
			btree.Insert_BPlus_Tree(trecord);
		}
	}
	else {//当关键字是neochar型时
		BPlusTreeC btree;//建树
		btree.ReadBPlusTree(tablename, keyname);

		vector<FILEP> points_to;
		search_tuples_all_inf(tablename, points_to);

		for (int i = 0; i < points_to.size(); i++) {

			TRecordC trecord;//给树的record
			
			trecord.Raddress = points_to[i];//载入记录地址

			Record rec;
			read_record_my(tablename, points_to[i], rec, table_meta);//读取记录
			trecord.key = (char*)rec.data[key_positon];//载入关键字
			btree.Insert_BPlus_Tree(trecord);
		}
	}
	
}

//删除索引（bptree)
void delete_index_inf1(string tablename, string keyname)//千万不能删除主键！！！！！！请先从数据字典确认。
{
	char* str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	if (search_index_exist_inf(tablename, keyname)) {//索引存在
		sprintf(str, "rd /s/q table\\%s\\index\\bptree_%s", tablename.c_str(), keyname.c_str());
		system(str);
	}
	free(str);
}

//查询存在的索引
void search_all_indexs_inf(string tablename, vector<string>& all_indexs)
{
	char *str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index", tablename.c_str());
	get_all_something_under(str, all_indexs);
	free(str);
}

//查询目标属性索引是否存在
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
		//有这个索引了，啥都不做
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

	if (table_meta.main_key == keyname) {//无法删除主键索引
		return false;
	}
	else if(!search_index_exist_inf(tablename, keyname)){//没有这个属性的索引
		return false;
	}
	else {//满足删除索引条件
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
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		return search_tuples_exist_inf1(tablename, keyname, key);
	}
	else {//索引不存在
		return search_tuples_exist_noindex(tablename, keyname, key);
	}
}

bool search_tuples_exist_inf(string tablename, string keyname, double key)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		return search_tuples_exist_inf1(tablename, keyname, key);
	}
	else {//索引不存在
		return search_tuples_exist_noindex(tablename, keyname, key);
	}
}

bool search_tuples_inf(string tablename, string keyname, string key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		return search_tuples_inf2(tablename, keyname, key, points_to);
	}
	else {//索引不存在
		return search_tuples_noindex(tablename, keyname, key, points_to);
	}
}

bool search_tuples_inf(string tablename, string keyname, double key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		return search_tuples_inf2(tablename, keyname, key, points_to);
	}
	else {//索引不存在
		return search_tuples_noindex(tablename, keyname, key, points_to);
	}
}

bool search_tuples_not_equal_inf(string tablename, string keyname, double key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		return search_tuples_not_equal_inf2(tablename, keyname, key, points_to);
	}
	else {//索引不存在
		return search_tuples_not_equal_noindex(tablename, keyname, key, points_to);
	}
}

bool search_tuples_not_equal_inf(string tablename, string keyname, string key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		return search_tuples_not_equal_inf2(tablename, keyname, key, points_to);
	}
	else {//索引不存在
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
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		search_smaller_tuples_inf2(tablename, keyname, key, points_to);
	}
	else {//索引不存在
		search_smaller_tuples_noindex(tablename, keyname, key, points_to);
	}
}

void search_bigger_tuples_inf(string tablename, string keyname, double key, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		search_bigger_tuples_inf2(tablename, keyname, key, points_to);
	}
	else {//索引不存在
		search_bigger_tuples_noindex(tablename, keyname, key, points_to);
	}
}

void search_bigger_and_smaller_tuples_inf(string tablename, string keyname, double key1, double key2, vector<FILEP>& points_to)
{
	if (search_index_exist_inf(tablename, keyname)) {//bptree索引存在
		search_bigger_and_smaller_tuples_inf2(tablename, keyname, key1, key2, points_to);
	}
	else {//索引不存在
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

FILEP insert_tuple_(string tablename, Record record)//插入记录到磁盘
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
	delete_tuples_inf1(tablename, points_to);//删除索引内的记录地址

	delete_tuples_from_dicks(tablename, points_to);//删除记录
}

bool insert_tuple_inf(string tablename, Record & record)
{
	//先要查看是否主码重复
	table table_meta;
	read_tabe_meta_my(tablename, table_meta);
	int i = find_keyname_position_my(table_meta.main_key,table_meta);//记录主码位置
	
	bool is_exist = 0;//默认主码没有重复
	if (table_meta.col[i].type == 1) {//当是double时
		is_exist = search_tuples_exist_inf(tablename, table_meta.col[i].col_name, *(double*)record.data[i]);
	}
	if (table_meta.col[i].type == 2) {//当是neochar时
		is_exist = search_tuples_exist_inf(tablename, table_meta.col[i].col_name, (char*)record.data[i]);
	}

	if (is_exist) {
		return false;
	}

	else {
		FILEP address = insert_tuple_(tablename, record);//插入记录，返回记录地址
		insert_tuple_inf1(tablename, address);
	}
}


//void change_tuples_inf(string tablename, string keyname, string key_before, string key_now, vector<FILEP> points_to)
//{
//	change_tuples_inf1(tablename, keyname, key_before, key_now, points_to);//更新索引内的记录地址对应的属性值
//
//	change_tuples_from_dicks(tablename, keyname, key_before, key_now, points_to);//更新记录
//
//}
//
//void change_tuples_inf(string tablename, string keyname, double key_before, double key_now, vector<FILEP> points_to)
//{
//	change_tuples_inf1(tablename, keyname, key_before, key_now, points_to);//更新索引内的记录地址对应的属性值
//
//	change_tuples_from_dicks(tablename, keyname, key_before, key_now, points_to);//更新记录
//}

void change_tuple_inf(string tablename, string keyname, string key_before, string key_now, FILEP point_to)
{
	table table_meta;
	read_table_meta_inf(tablename, table_meta);
	if (!keyname.compare(table_meta.main_key)) {//要修改的属性是主码
		vector<FILEP> points_to_pk;
		search_tuples_inf(tablename, keyname, key_now, points_to_pk);
		if (points_to_pk.size()) {//当存在了要修改成为的主码
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
	if (!keyname.compare(table_meta.main_key)) {//要修改的属性是主码
		vector<FILEP> points_to_pk;
		search_tuples_inf(tablename, keyname, key_now, points_to_pk);
		if (points_to_pk.size()) {//当存在了要修改成为的主码
			return;
		}
	}
	change_tuple_inf1(tablename, keyname, key_before, key_now, point_to);
	change_tuple_from_disk(tablename, keyname, key_before, key_now, point_to);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////

//读取多个记录地址的索引文件中的记录地址
void read_inid(string tablename, string keyname, TSearchResult tresult, vector<FILEP> &points_to) {
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), tresult.Raddress);
	ifstream in_file(str, ios::in | ios::binary);
	free(str);

	FILEP _point = 0;
	while (1) {
		in_file.read((char*)&_point, sizeof(_point));
		if (in_file.eof()) break;//当读到末尾时就结束
		points_to.push_back(_point);

	}

	in_file.close();
}

//从TSearchResult中读记录地址到地址数组中
void from_result_add_to_points(string tablename, string keyname, TSearchResult tresult, vector<FILEP> &points_to) {
	if (tresult.exist) {
		if (tresult.ismul) {//当关键字指向多个记录地址的索引
			read_inid(tablename, keyname, tresult, points_to);
		}
		else {//当关键字指向单个记录地址
			FILEP _point;
			_point = tresult.Raddress;
			points_to.push_back(_point);
		}
	}
}

//从TSearchResult数组中读记录地址到地址数组中
void from_results_add_to_points(string tablename, string keyname, vector<TSearchResult> &results, vector<FILEP> &points_to) {
	for (int i = 0; i < results.size(); i++) {
		from_result_add_to_points(tablename, keyname, results[i], points_to);
	}
}

//判断当前关键字的元组是否存在
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

//查找当前关键字元组（返回TSearchResult）
bool search_tuples_inf1(string tablename, string keyname, string key, TSearchResult &tresult) {//查找前请先判断table是否存在
	BPlusTreeC btree;
	btree.ReadBPlusTree(tablename, keyname);
	TRecordC trecord;
	trecord.key = key;
	btree.Search_BPlus_Tree(trecord, tresult);//把查询结果返回到tresult中
	return tresult.exist;//返回关键字是否存在
}
bool search_tuples_inf1(string tablename, string keyname, double key, TSearchResult &tresult) {//查找前请先判断table是否存在
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);
	TRecord trecord;
	trecord.key = key;
	btree.Search_BPlus_Tree(trecord, tresult);//把查询结果返回到tresult中
	return tresult.exist;//返回关键字是否存在
}

bool search_tuples_not_equal_inf1(string tablename, string keyname, string key, vector<TSearchResult> &results)
{
	BPlusTreeC btree;
	btree.ReadBPlusTree(tablename, keyname);

	BPlusNodeC node;
	btree.ReadBPlusNode(btree.get_root(), node);

	//定位到最左边的叶节点
	while (!node.isleaf)
	{
		btree.ReadBPlusNode(node.pointers[0].second, node);
	}

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//必定存在啦
		for (int i = 0; i < node.nkey; i++) {
			tresult.ismul = node.pointers[i].first;
			tresult.Raddress = node.pointers[i].second;
			results.push_back(tresult);
		}

		if (node.pointers[MAX_KEY].second == 0)//没有下一个叶节点了,break
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

	//定位到最左边的叶节点
	while (!node.isleaf)
	{
		btree.ReadBPlusNode(node.pointers[0].second, node);
	}

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//必定存在啦
		for (int i = 0; i < node.nkey; i++) {
			tresult.ismul = node.pointers[i].first;
			tresult.Raddress = node.pointers[i].second;
			results.push_back(tresult);
		}

		if (node.pointers[MAX_KEY].second == 0)//没有下一个叶节点了,break
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

		//定位到最左边的叶节点
		while (!node.isleaf)
		{
			btree.ReadBPlusNode(node.pointers[0].second, node);
		}

		while (1)
		{
			TSearchResult tresult;
			tresult.exist = true;//必定存在啦
			for (int i = 0; i < node.nkey; i++) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				tresults.push_back(tresult);
			}

			if (node.pointers[MAX_KEY].second == 0)//没有下一个叶节点了,break
				break;

			btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
		}
	}
	if (type == 2) {
		BPlusTreeC btree;
		btree.ReadBPlusTree(tablename, keyname);

		BPlusNodeC node;
		btree.ReadBPlusNode(btree.get_root(), node);

		//定位到最左边的叶节点
		while (!node.isleaf)
		{
			btree.ReadBPlusNode(node.pointers[0].second, node);
		}

		while (1)
		{
			TSearchResult tresult;
			tresult.exist = true;//必定存在啦
			for (int i = 0; i < node.nkey; i++) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				tresults.push_back(tresult);
			}

			if (node.pointers[MAX_KEY].second == 0)//没有下一个叶节点了,break
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





//查找当前关键字元组（返回vector<FILEP>)
bool search_tuples_inf2(string tablename, string keyname, string key, vector<FILEP> &points_to) {//查找前请先判断table是否存在
	TSearchResult tresult;
	search_tuples_inf1(tablename, keyname, key, tresult);
	if (tresult.exist) {
		from_result_add_to_points(tablename, keyname, tresult, points_to);
	}
	return tresult.exist;//返回关键字是否存在
}
bool search_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to) {//查找前请先判断table是否存在
	TSearchResult tresult;
	search_tuples_inf1(tablename, keyname, key, tresult);
	if (tresult.exist) {
		from_result_add_to_points(tablename, keyname, tresult, points_to);
	}
	return tresult.exist;//返回关键字是否存在
}


//查找小于该关键字的元组(只适用于关键字为整形）（返回vector<TSearchResult>）
void search_smaller_tuples_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results) {
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);

	BPlusNode node;
	btree.ReadBPlusNode(btree.get_root(), node);

	//定位到最左边的叶节点
	while (!node.isleaf)
	{
		btree.ReadBPlusNode(node.pointers[0].second, node);
	}

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//必定存在啦
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

		if (flag_no_more_smaller)//没有小于等于的key值了,break
			break;

		if (node.pointers[MAX_KEY].second == 0)//没有下一个叶节点了,break
			break;

		btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
	}
}


//查找大于该关键字的元组(只适用于关键字为整形）（返回vector<TSearchResult>）
void search_bigger_tuples_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results) {
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);

	TRecord trecord;
	TSearchResult result_temp;
	trecord.key = key;
	btree.Search_BPlus_Tree_Bigger(trecord, result_temp);//把查询结果返回到tresult中

	BPlusNode node;
	btree.ReadBPlusNode(result_temp.Baddress, node);//读入结点

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//必定存在啦
		for (int i = 0; i < node.nkey; i++) {
			if (node.keys[i] > key) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				results.push_back(tresult);
			}
			else if (node.keys[i] <= key) {
				//什么都不做，自然跳过
			}
			else {
				break;
			}
		}

		if (node.pointers[MAX_KEY].second == 0)//没有下一个叶节点了,break
			break;

		btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
	}

}

//查找大于key1小于key2的元组(只适用于关键字为整形）（返回vector<TSearchResult>）
void search_bigger_and_smaller_tuples_inf1(string tablename, string keyname, double key1, double key2, vector<TSearchResult> &results) {
	BPlusTree btree;
	btree.ReadBPlusTree(tablename, keyname);

	TRecord trecord;
	TSearchResult result_temp;
	trecord.key = key1;
	btree.Search_BPlus_Tree_Bigger(trecord, result_temp);//把查询结果返回到tresult中

	BPlusNode node;
	btree.ReadBPlusNode(result_temp.Baddress, node);//读入结点

	while (1)
	{
		TSearchResult tresult;
		tresult.exist = true;//必定存在啦
		bool flag_no_more_smaller = false;
		for (int i = 0; i < node.nkey; i++) {
			if (node.keys[i] > key1 && node.keys[i] < key2) {
				tresult.ismul = node.pointers[i].first;
				tresult.Raddress = node.pointers[i].second;
				results.push_back(tresult);
			}
			else if (node.keys[i] <= key1) {
				//什么都不做，自然跳过
			}
			else {
				flag_no_more_smaller = true;
				break;
			}
		}

		if (flag_no_more_smaller)//没有小于等于的key值了,break
			break;

		if (node.pointers[MAX_KEY].second == 0)//没有下一个叶节点了,break
			break;

		btree.ReadBPlusNode(node.pointers[MAX_KEY].second, node);
	}

}

//查找小于该关键字的元组(只适用于关键字为double）（返回vector<FILEP>）
void search_smaller_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	vector<TSearchResult> results;
	search_smaller_tuples_inf1(tablename, keyname, key, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
}

//查找大于该关键字的元组(只适用于关键字为double）（返回vector<FILEP>）
void search_bigger_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	vector<TSearchResult> results;
	search_bigger_tuples_inf1(tablename, keyname, key, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
}

//查找大于key1小于key2的元组(只适用于关键字为double）（返回vector<FILEP>）
void search_bigger_and_smaller_tuples_inf2(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to) {
	vector<TSearchResult> results;
	search_bigger_and_smaller_tuples_inf1(tablename, keyname, key1, key2, results);
	from_results_add_to_points(tablename, keyname, results, points_to);
}

//插入（请先插入记录，得到记录地址后，再插入索引）
void insert_tuple_inf1(string tablename, FILEP address)
{
	vector<string> all_indexs;//读取所有索引
	search_all_indexs_inf(tablename, all_indexs);
	
	table table_meta;//读取数据字典
	read_tabe_meta_my(tablename, table_meta);
	
	Record record_this;//读取插入记录
	read_record_my(tablename, address, record_this, table_meta);

	for (int i = 0; i < all_indexs.size(); i++) {
		for (int j = 0; j < table_meta.count; j++) {
			string btree_name = "bptree_";
			string keyname = table_meta.col[j].col_name;
			btree_name = btree_name + keyname;
			if (!all_indexs[i].compare(btree_name)) {//找到此索引
				if (table_meta.col[j].type == 1) {//此索引的属性是double型
					BPlusTree btree;
					btree.ReadBPlusTree(tablename, keyname);
					TRecord trecord;
					trecord.key = *(double*)record_this.data[j];//关键字载入
					trecord.Raddress = address;//记录地址载入
					btree.Insert_BPlus_Tree(trecord);//插入记录
				}
				else {//此索引属性是"string"型
					BPlusTreeC btree;
					btree.ReadBPlusTree(tablename, keyname);
					TRecordC trecord;
					trecord.key = (char*)record_this.data[j];//关键字载入
					trecord.Raddress = address;//记录地址载入
					btree.Insert_BPlus_Tree(trecord);//插入记录
				}
				break;
			}
		}
	}
	
}

//删除（我先查找记录，得到记录地址后，再删除索引）（请删除索引后再删除文件）
void delete_tuple_inf1(string tablename, FILEP address)
{
	vector<string> all_indexs;//读取所有索引
	search_all_indexs_inf(tablename, all_indexs);

	table table_meta;//读取数据字典
	read_tabe_meta_my(tablename, table_meta);

	Record record_this;//读取插入记录
	read_record_my(tablename, address, record_this, table_meta);

	for (int i = 0; i < all_indexs.size(); i++) {
		for (int j = 0; j < table_meta.count; j++) {
			string btree_name = "bptree_";
			string keyname = table_meta.col[j].col_name;
			btree_name = btree_name + keyname;
			if (!all_indexs[i].compare(btree_name)) {//找到此索引
				if (table_meta.col[j].type == 1) {//此索引的属性是double型
					BPlusTree btree;
					btree.ReadBPlusTree(tablename, keyname);
					TRecord trecord;
					trecord.key = *(double*)record_this.data[j];//关键字载入
					trecord.Raddress = address;//记录地址载入
					btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//删除记录
				}
				else {//此索引属性是"neochar"型
					BPlusTreeC btree;
					btree.ReadBPlusTree(tablename, keyname);
					TRecordC trecord;
					trecord.key = (char*)record_this.data[j];//关键字载入
					trecord.Raddress = address;//记录地址载入
					btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//删除记录
				}
				break;
			}
		}
	}

}

void delete_tuples_inf1(string tablename, vector<FILEP>& points_to)
{
	vector<string> all_indexs;//读取所有索引
	search_all_indexs_inf(tablename, all_indexs);

	table table_meta;//读取数据字典
	read_tabe_meta_my(tablename, table_meta);

	for (int i = 0; i < all_indexs.size(); i++) {
		for (int j = 0; j < table_meta.count; j++) {
			string btree_name = "bptree_";
			string keyname = table_meta.col[j].col_name;
			btree_name = btree_name + keyname;
			if (!all_indexs[i].compare(btree_name)) {//找到此索引
				if (table_meta.col[j].type == 1) {//此索引的属性是double型
					BPlusTree btree;
					btree.ReadBPlusTree(tablename, keyname);//先建好树，就不用重复建树了
					for (int k = 0; k < points_to.size(); k++) {
						Record record_this;//读取插入记录
						read_record_my(tablename, points_to[k], record_this, table_meta);

						TRecord trecord;
						trecord.key = *(double*)record_this.data[j];//关键字载入
						trecord.Raddress = points_to[k];//记录地址载入
						btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//删除记录

					}
				}
				else {//此索引属性是"neochar"型
					BPlusTreeC btree;
					btree.ReadBPlusTree(tablename, keyname);//先建好树，就不用重复建树了
					for (int k = 0; k < points_to.size(); k++) {
						Record record_this;//读取插入记录
						read_record_my(tablename, points_to[k], record_this, table_meta);

						TRecordC trecord;
						trecord.key = (char*)record_this.data[j];//关键字载入
						trecord.Raddress = points_to[k];//记录地址载入
						btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//删除记录

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
		btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//删除旧关键字的记录地址
		trecord.key = key_now;
		btree.Insert_BPlus_Tree(trecord);//增加新关键字的记录地址
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
		btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//删除旧关键字的记录地址
		trecord.key = key_now;
		btree.Insert_BPlus_Tree(trecord);//增加新关键字的记录地址
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
		btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//删除旧关键字的记录地址
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
		btree.Delete_BPlus_Tree_Address_Under_Key(trecord);//删除旧关键字的记录地址
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
	
}//在无索引的情况下，查找指定关键字的记录是否存在
bool search_tuples_exist_noindex(string tablename, string keyname, double key) {
	record_wrapper a(tablename);
	return a.search_tuples_exist(keyname, key);
}//在无索引的情况下，查找指定关键字的记录是否存在

bool search_tuples_noindex(string tablename, string keyname, string key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_tuples(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
	return points_to.size();
}//在无索引的情况下，得到指定关键字的记录地址，存储在vector<FILEP> &points_to中
bool search_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_tuples(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
	return points_to.size();
}//在无索引的情况下，得到指定关键字的记录地址，存储在vector<FILEP> &points_to中

void search_smaller_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_smaller_tuples(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
}//在无索引的情况下，得到关键字小于key的记录地址，存储在vector<FILEP> &points_to中）
void search_bigger_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n;
	a.search_bigger_tuples(keyname, key, ary, n);
	for (int i = 0; i < n; i++) {
		points_to.push_back(ary[i]);
	}
}//在无索引的情况下，得到关键字大于key的记录地址，存储在vector<FILEP> &points_to中）
void search_bigger_and_smaller_tuples_noindex(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n = 0;
	a.search_bigger_and_smaller_tuples(keyname, key1, key2, ary, n);
	for (int i = 0; i < n; i++) {
		ary[i] = points_to[i];
	}
}//在无索引的情况下，得到关键字大于key1，小于key2的记录地址，存储在vector<FILEP> &points_to中）

void delete_tuples_from_dicks(string tablename, vector<FILEP> &points_to) {
	record_wrapper a(tablename);
	int ary[1000]; int n = points_to.size();
	for (int i = 0; i < n; i++) {
		ary[i] = points_to[i];
	}
	a.delete_tuple(ary, n);
}//删除指定地址的记录。注意，是多个记录。

void change_tuples_from_dicks(string tablename, string keyname, string key_before, string key_now, vector<FILEP> &points_to) {
	int ary[1000]; int n = points_to.size();
	for (int i = 0; i < n; i++) {
		ary[i] = points_to[i];
	}
	record_wrapper a(tablename);
	//a.change_tuple(keyname, key_before, key_now, ary, n);
}//更新指定地址的记录。注意，是多个记录。
void change_tuples_from_dicks(string tablename, string keyname, double key_before, double key_now, vector<FILEP> &points_to) {
	int ary[1000]; int n = points_to.size();
	for (int i = 0; i < n; i++) {
		ary[i] = points_to[i];
	}
	record_wrapper a(tablename);
	//a.change_tuple(keyname, key_before, key_now, ary, n);
}//更新指定地址的记录。注意，是多个记录。

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
}//在无索引的情况下，得到指定关键字不相等的记录地址，存储在vector<FILEP> &points_to中
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