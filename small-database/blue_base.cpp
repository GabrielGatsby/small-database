#pragma once
#include "blue_base.h"
#include "inf.h"
using namespace blue_base;

inline bool is_alpha(char cc) {//是不是字母
	if (cc >= 'a'&&cc <= 'z')return 1;
	if (cc >= 'A'&&cc <= 'Z')return 1;
	return 0;
}

inline bool is_num_bit(char cc) {//是不是数字
	if (cc >= '0'&&cc <= '9')return 1;
	return 0;
}

/*
inline bool is_num(string str, int &num) {//解析成数值
	int len = str.length();
	if (len == 0)return 0;
	num = 0;
	for (int i = 0; i < len; i++) {
		if (!is_num_bit(str[i]))return 0;
		num = num * 10 + str[i] - '0';
	}
	return 1;
}
*/

inline bool is_num(string str, double &num) {//解析成浮点数==================未测试
	int xi = 1;
	if (str[0] == '-'){
		str = str.substr(1, str.length() - 1);
		xi = -1;
	}
	int len = str.length();
	if (len == 0)return 0;
	num = 0;
	int i;
	for (i = 0; i < len; i++) {
		if (str[i] == '.'){ i++; break; }
		if (!is_num_bit(str[i]))return 0;
		num = num * 10 + str[i] - '0';
	}
	int k = 1;
	for (; i < len; i++){
		if (str[i] == '.')return 0;
		if (!is_num_bit(str[i]))return 0;
		k *= 10;
		num += double(str[i] - '0') / k;
	}
	num = num*xi;
	return 1;
}

inline bool is_string_const(string str) {//规定字符串常量不能有字母和数字外的东西
	int len = str.length();
	if (len == 0)return 0;
	for (int i = 0; i < len; i++) {
		if (!is_num_bit(str[i]) && !is_alpha(str[i]))return 0;
	}
	return 1;
}

inline bool is_keep_word(string str) {
	if (str == "create_table")return 1;
	if (str == "create_index")return 1;
	if (str == "select")return 1;
	if (str == "from")return 1;
	if (str == "where")return 1;
	if (str == "select")return 1;
	if (str == "delete")return 1;
	if (str == "update")return 1;
	if (str == "insert")return 1;
	return 0;
}

inline bool is_legal_user_id(string str) {//判断是不是合法的标识符
	int len = str.length();
	if (len == 0)return 0;
	if (!is_alpha(str[0]))return 0;
	for (int i = 0; i < len; i++) {
		if (!is_alpha(str[i]) && !is_num_bit(str[i]))return 0;
	}
	if (is_keep_word(str))return 0;//是保留字
	return 1;
}

inline vector<string> space_extraction(string str) {
	vector<string> ret;
	int len = str.length();
	int beg = 0;
	for (int i = 0; i <= len; i++) {
		if (i == len || str[i] == ' ') {
			while (beg < i&&str[beg] == ' ')beg++;
			string word = str.substr(beg, i - beg);
			beg = i + 1;
			if (word.size() != 0) {
				ret.push_back(word);
			}
		}
	}
	return ret;
}

//0 /= ;1 / <; 2 / >; 3 / <= ; 4 / >= ; 5 / <>;
inline int get_op_id(string str) {//判断是不是比较符号
	int len = str.length();
	if (len == 1) {
		if (str[0] == '=')return 0;
		if (str[0] == '<')return 1;
		if (str[0] == '>')return 2;
	}
	else if (len == 2) {
		if (str[0] == '<'&&str[1] == '=')return 3;
		if (str[0] == '>'&&str[1] == '=')return 4;
		if (str[0] == '<'&&str[1] == '>')return 5;
	}
	return -1;
}

inline int checkpoint(char ch) {
	if (ch == '.') return 1;
	return 0;
}

table_attr::table_attr(string a, string b){
	tablename = a;
	attrname = b;
}

bool table_attr::operator==(const table_attr b)const{
	if (tablename != b.tablename)return 0;
	if (attrname != b.attrname)return 0;
	return 1;
}

int table_attr::parse(string str) {//将字符串解析成table和attr的形式，失败则返回false;
	int len = str.length();
	if (len == 1 && str[0] == '*') {
		tablename = str;
		return 1;
	}
	if (len <= 2)return -1;
	if (str[0] == '.' || str[len - 1] == '.')return -7;
	int pos = -1;
	for (int i = 1; i < len - 1; i++) {
		if (str[i] == '.') { pos = i; break; }
	}
	if (pos == -1)return -8;
	string lef = str.substr(0, pos);

	string rig = str.substr(pos + 1, len - pos - 1);

	if (!is_legal_user_id(lef))return -9;
	if (!is_legal_user_id(rig))return -10;
	tablename = lef; attrname = rig;
	return 1;
}

int Base_attribute_sega::parse(string str) {
	//先拆成一个一个单词，然后制式判断即可
	vector<string> word_vec = space_extraction(str);
	if (word_vec.size() < 2)return -1;//too short

	if (word_vec[1] == "double")type = 1;
	else if (word_vec[1] == "char")type = 2;//匹配底层而修改的-------------2108.11.17
	else return -2;//类型设置错误

	if (!is_legal_user_id(word_vec[0])) {
		return -3;//不是合法的标志符
	}
	else name = word_vec[0];

	if (word_vec.size() == 2) {
		key = 0;
		return 1;
	}
	else if (word_vec.size() == 3) {
		//cout << "test" << endl;		gg1122
		if (word_vec[2] != "primary_key")return -4;
		key = 1;
		return 1;
	}
	else if (word_vec.size() == 4) {
		//cout << "test2" << endl;		gg1122
		if (word_vec[2] != "references")return -5;
		if (!outer_code.parse(word_vec[3]))return -6;//外码解析失败
		key = 2;
		return 1;
	}
	else if (word_vec.size() == 5) {
		if (word_vec[2] != "primary_key")return -4;
		if (word_vec[3] != "references")return -5;
		if (!outer_code.parse(word_vec[4]))return -6;//外码解析失败
		key = 3;
		return 1;
	}
	return 0;
}

string error::geterror() {
	switch (this->error_type) {
	case 1: return string("没有错误\n");
	case 2: return string("表名无效\n");
	case 3: return string("标识符无效\n");
	case 4: return string("无效数据类型\n");
	case 5: return string("选项缺失或无效\n");		//缺失左括号；右括号后面不接‘；’而是一些乱七八糟的东西
	case 6: return string("缺失右括号\n");
	case 7: return string("无效的关系运算符\n");	//where 不是条件语句
	case 8: return string("无效数字\n");			//浮点数对应的值为字符串
	case 9: return string("缺失等号\n");			//列名=表达式
	case 10: return string("表不存在\n");
	case 11: return string("表已经存在了\n");
	case 12: return string("主码缺失\n");
	case 13: return string("存在多个主码\n");
	default: return "";
	}
}

int expression::parse(string str) {//解析成表达式
	int len = str.length();
	if (len <= 2)return -1;
	int lef = -1, rig = -1;
	for (int i = 0; i < len; i++) {
		if (!is_alpha(str[i]) && !is_num_bit(str[i]) && str[i] != '.') {
			lef = rig = i;
			while (rig + 1 < len && !is_alpha(str[rig + 1]) && !is_num_bit(str[rig + 1]) && str[rig + 1] != '"') {
				rig++;
			}
			if (lef == -1)return -11;
			string op_str = str.substr(lef, rig - lef + 1);
			op = get_op_id(op_str);
			if (op >= 0) break;
			else return -12;
		}
	}
	/*if (lef == -1)return 0;
	string op_str = str.substr(lef, rig - lef + 1);
	cout << "op:" << op_str << endl;
	op = get_op_id(op_str);
	if (op == -1)return 0;*/
	string lefstr = str.substr(0, lef);
	string rigstr = str.substr(rig + 1, len - rig - 1);

	int errorcode = lef_attr.parse(lefstr);
	if (errorcode != 1)
	{
		if (errorcode == -9)return -13;
		if (errorcode == -10)return -14;
		return -13;
	}//解析成左属性
	int riglen = rigstr.length();
	if (riglen > 2) {
		if (rigstr[0] == '"'&&rigstr[riglen - 1] == '"') {//判断是不是字符串常量
			string rig1 = rigstr.substr(1, riglen - 2);
			if (is_string_const(rig1)) {
				type = 2;
				nval = 0;
				sval = rig1;
				return 1;
			}
		}
	}
	double num;
	if (is_num(rigstr, num)) {//判断是不是数值
		type = 1;
		nval = num;
		sval = "";
		return 1;
	}
	if (!rig_attr.parse(rigstr))return -14;//解析成右属性
	else {
		type = 0;
		nval = 0;
		sval = "";
	}
	return 1;
}


//===============================表的操作================================
//where table.sno in (
		//SELETE Sno 
		//From student
		//where sdept='CS'	
//)太难了不实现了

RESULT::RESULT(string tablename, vector<FILEP> points_to){//构造函数————————————————未测试
	init(tablename, points_to);
}

void RESULT::init(string tablename, vector<FILEP> points_to){//使用指针集合初始化
	vector<string> cnt_vec = get_all_attr(tablename);
	int attrsiz = cnt_vec.size();
	for (auto attr : cnt_vec){
		attr_vec.push_back(table_attr(tablename,attr));
	}
	int siz = points_to.size();
	data_vec.resize(siz);
	rec_addr.resize(siz);
	for (int i = 0; i < siz; i++){
		rec_addr[i] = points_to[i];
		Record cnt_record;
		read_record_inf(tablename, points_to[i], cnt_record);
		for (int j = 0; j < attrsiz; j++){
			data_vec[i].push_back(cnt_record.data[j]);
		}
	}
	this->ReNumber();
}

RESULT RESULT::Projection(vector<table_attr> ta){//投影
	int siz = this->attr_vec.size();
	RESULT newt;
	vector<int> vec;
	for (auto v : ta){
		for (int i = 0; i < siz;i++){
			if (v == attr_vec[i]){
				vec.push_back(i);
				break;
			}
		}
	}
	for (auto i : vec){
		newt.attr_vec.push_back(this->attr_vec[i]);
	}
	int datasiz = data_vec.size();
	newt.data_vec.resize(datasiz);
	for (int i = 0; i < datasiz; i++){
		for (auto j : vec){
			newt.data_vec[i].push_back(this->data_vec[i][j]);
		}
	}
	newt.rec_addr.resize(datasiz);
	for (int i = 0; i < datasiz; i++){
		newt.rec_addr[i] = this->rec_addr[i];
	}
	return newt;
}

bool satisfy(int op, int type, void * lef, void *rig){
	if (type == 1){
		if (op == 0)return *((double*)lef) == *((double*)rig);
		else if (op == 1)return *((double*)lef) < *((double*)rig);
		else if (op == 2)return *((double*)lef) > *((double*)rig);
		else if (op == 3)return *((double*)lef) <= *((double*)rig);
		else if (op == 4)return *((double*)lef) >= *((double*)rig);
		else if (op == 5)return *((double*)lef) != *((double*)rig);
	}
	else if (type == 2){
		if (op == 0)return string((char*)lef) == string((char*)rig);
		else if (op == 5)return string((char*)lef) != string((char*)rig);
	}
}

RESULT RESULT::Select(expression cd){//选择(条件筛选)_______函数内不会再进行检查了，所以外面得做完检查
	if (cd.type == 0){//属性比较
		RESULT newt;
		newt.attr_vec = this->attr_vec;
		int lefid, rigid;
		int rowsize = attr_vec.size();
		for (int i = 0; i < rowsize; i++){
			if (attr_vec[i] == cd.lef_attr){
				lefid = i;
			}
			if (attr_vec[i] == cd.rig_attr){
				rigid = i;
			}
		}
		int type = exist_attr_in_table(attr_vec[lefid].tablename, attr_vec[lefid].attrname);
		int datasize = data_vec.size();
		for (int i = 0; i < datasize; i++){
			if (satisfy(cd.op, type, data_vec[i][lefid],data_vec[i][rigid])){
				newt.data_vec.push_back(data_vec[i]);
				newt.rec_addr.push_back(rec_addr[i]);
				newt.id.push_back(id[i]);
			}
		}
		return newt;
	}
	else if (cd.type == 1 || cd.type== 2){
		RESULT newt;
		newt.attr_vec = this->attr_vec;
		int id;
		int rowsize = attr_vec.size();
		for (int i = 0; i < rowsize; i++){
			if (attr_vec[i] == cd.lef_attr){
				id = i; break;
			}
		}
		int type = exist_attr_in_table(attr_vec[id].tablename, attr_vec[id].attrname);
		int datasize = data_vec.size();
		for (int i = 0; i < datasize; i++){
			if ((type == 1 && satisfy(cd.op, type, data_vec[i][id], (void*)&cd.nval)) || (type == 2 && satisfy(cd.op, type, data_vec[i][id], (void*)(cd.sval.c_str())))){//这里的类型转换不知道能不能用
				newt.data_vec.push_back(this->data_vec[i]);
				newt.rec_addr.push_back(this->rec_addr[i]);
				newt.id.push_back(this->id[i]);
			}
		}
		return newt;
	}
}

RESULT Cartesian_Product(RESULT A, RESULT B){//笛卡尔积
	RESULT newt;
	int lefrow = A.attr_vec.size();
	int rigrow = B.attr_vec.size();
	for (int i = 0; i < lefrow; i++)newt.attr_vec.push_back(A.attr_vec[i]);
	for (int i = 0; i < rigrow; i++)newt.attr_vec.push_back(B.attr_vec[i]);
	int lefdat = A.data_vec.size();
	int rigdat = B.data_vec.size();
	int totdat = lefdat*rigdat;
	newt.data_vec.resize(totdat);
	newt.rec_addr.resize(totdat);
	for (int i = 0; i < lefdat; i++){
		for (int j = 0; j < rigdat; j++){
			int id = i*rigdat + j;
			newt.rec_addr[id] = A.rec_addr[i];//所以地址会有重复的
			for (int ii = 0; ii < lefrow; ii++)newt.data_vec[id].push_back(A.data_vec[i][ii]);
			for (int jj = 0; jj < rigrow; jj++)newt.data_vec[id].push_back(B.data_vec[j][jj]);
		}
	}
	return newt;
}

RESULT MID_OP::brute_force_union(RESULT A, RESULT B,bool &f){
	int lattrsiz = A.attr_vec.size();
	int rattrsiz = B.attr_vec.size();
	if (lattrsiz != rattrsiz) {
		f = 0;
		RESULT cc;
		printf("union 存在错误\n");
		return cc;
	}
	for (int i = 0; i < lattrsiz; i++) {
		if (A.attr_vec[i].tablename != B.attr_vec[i].tablename||A.attr_vec[i].attrname!=B.attr_vec[i].attrname) {
			f = 0;
			printf("union 存在错误\n");
			RESULT cc;
			return cc;
		}
	}
	RESULT ret;
	ret.attr_vec = A.attr_vec;
	int lefdatasiz = A.data_vec.size();
	int rigdatasiz = B.data_vec.size();
	int i = 0, j = 0;
	while (i < lefdatasiz&&j < rigdatasiz){
		while (j<rigdatasiz && i < lefdatasiz && A.id[i] < B.id[j]){
			vector<void*> cc=A.data_vec[i];
			ret.data_vec.push_back(cc);
			ret.id.push_back(A.id[i]);
			ret.rec_addr.push_back(A.rec_addr[i]);
			i++;
		}
		while (j < rigdatasiz && i < lefdatasiz && A.id[i] > B.id[j]){
			vector<void*> cc = B.data_vec[j];
			ret.data_vec.push_back(cc);
			ret.id.push_back(B.id[j]);
			ret.rec_addr.push_back(B.rec_addr[j]);
			j++;
		}
		while (j < rigdatasiz && i < lefdatasiz && A.id[i] == B.id[j]){
			vector<void*> cc = A.data_vec[i];
			ret.data_vec.push_back(cc);
			ret.id.push_back(A.id[i]);
			ret.rec_addr.push_back(A.rec_addr[i]);
			i++; j++;
		}
	}
	while (i < lefdatasiz){
		vector<void*> cc = A.data_vec[i];
		ret.data_vec.push_back(cc);
		ret.id.push_back(A.id[i]);
		ret.rec_addr.push_back(A.rec_addr[i]);
		i++;
	}
	while (j < rigdatasiz){
		vector<void*> cc = B.data_vec[j];
		ret.data_vec.push_back(cc);
		ret.id.push_back(B.id[j]);
		ret.rec_addr.push_back(B.rec_addr[j]);
		j++;
	}
	return ret;
}

RESULT MID_OP::brute_force_intersect(RESULT A, RESULT B,bool & f){
	int lattrsiz = A.attr_vec.size();
	int rattrsiz = B.attr_vec.size();
	if (lattrsiz != rattrsiz) {
		f = 0;
		RESULT cc;
		printf("intersect 存在错误\n");
		return cc;
	}
	for (int i = 0; i < lattrsiz; i++) {
		if (A.attr_vec[i].tablename != B.attr_vec[i].tablename || A.attr_vec[i].attrname != B.attr_vec[i].attrname) {
			f = 0;
			RESULT cc;
			printf("intersect 存在错误\n");
			return cc;
		}
	}
	RESULT ret;
	ret.attr_vec = A.attr_vec;
	int lefdatasiz = A.data_vec.size();
	int rigdatasiz = B.data_vec.size();
	int i = 0;
	for (int j = 0; j < rigdatasiz; j++){
		while (i < lefdatasiz&&A.id[i] < B.id[j])i++;
		if (i < lefdatasiz&&A.id[i] == B.id[j]){
			ret.data_vec.push_back(B.data_vec[j]);
			ret.rec_addr.push_back(B.rec_addr[j]);
			ret.id.push_back(B.id[j]);
		}
	}
	return ret;
}

void RESULT::ReNumber(){
	int siz = data_vec.size();
	id.resize(siz);
	for (int i = 0; i < siz; i++){
		id[i] = i;
	}
	return;
}

void RESULT::clear(){
	attr_vec.clear();
	data_vec.clear();
	id.clear();
	rec_addr.clear();
}

void RESULT::print() {
	int attrsiz = attr_vec.size();
	vector<int> type;
	for (int i = 0; i < attrsiz; i++) {

		cout << setw(20) << left << attr_vec[i].tablename + "." + attr_vec[i].attrname;
		if (i == attrsiz - 1) cout << endl;
		type.push_back(exist_attr_in_table(attr_vec[i].tablename, attr_vec[i].attrname));
	}
	for (int i = 0; i < 20 * attrsiz; i++)printf("-");
	printf("\n");
	int datasiz = data_vec.size();
	for (int i = 0; i < datasiz; i++) {
		for (int j = 0; j < attrsiz; j++) {
			if (type[j] == 1)cout << setw(20) << left << *(double*)data_vec[i][j];
			else cout << setw(20) << left << string((char*)data_vec[i][j]);
			if (j == attrsiz - 1)cout << endl;
		}
	}
}


vector<FILEP> FILEP_intersection(vector<FILEP> lef, vector<FILEP> rig){//求两个元组集合的交集
	sort(lef.begin(), lef.end());
	sort(rig.begin(), rig.end());
	vector<FILEP> ret;
	int j = 0;
	int rigsiz = rig.size();
	for (int i = 0; i < lef.size(); i++){
		while (j < rigsiz&&rig[j] < lef[i])j++;
		if (lef[i] == rig[j])ret.push_back(lef[i]);
	}
	return ret;
}

vector<FILEP> FILEP_union(vector<FILEP> lef, vector<FILEP> rig){//求两个元组集合的并集
	vector <FILEP> ret=lef;
	for (int i = 0; i < rig.size(); i++){
		ret.push_back(rig[i]);
	}
	sort(ret.begin(), ret.end());
	vector<FILEP>::iterator new_end = unique(ret.begin(), ret.end());
	ret.erase(new_end, ret.end());//删除(真正的删除)重复的元素
	return ret;
}

vector<int> sega_query(string tablename, expression cond){//表名和单个条件,调用底层求解,已经保证合法性了
	vector<FILEP> ret_iter;
	if (cond.op==0){//==
		if (cond.type == 1)search_tuples_inf(tablename, cond.lef_attr.attrname, cond.nval, ret_iter);
		else if (cond.type == 2)search_tuples_inf(tablename, cond.lef_attr.attrname, cond.sval, ret_iter);
	}
	else if (cond.op == 1){//<
		search_smaller_tuples_inf(tablename, cond.lef_attr.attrname, cond.nval, ret_iter);
	}
	else if (cond.op == 2){//>
		search_bigger_tuples_inf(tablename, cond.lef_attr.attrname, cond.nval, ret_iter);
	}
	else if (cond.op == 3){//<=
		search_smaller_tuples_inf(tablename, cond.lef_attr.attrname, cond.nval, ret_iter);
		vector<FILEP> ret_iter1;
		search_tuples_inf(tablename, cond.lef_attr.attrname, cond.nval, ret_iter1);
		for (auto u : ret_iter1){
			ret_iter.push_back(u);
		}
	}
	else if (cond.op == 4){//>=
		search_bigger_tuples_inf(tablename, cond.lef_attr.attrname, cond.nval, ret_iter);
		vector<FILEP> ret_iter1;
		search_tuples_inf(tablename, cond.lef_attr.attrname, cond.nval, ret_iter1);
		for (auto u : ret_iter1){
			ret_iter.push_back(u);
		}
	}
	else if (cond.op == 5){//!=
		if (cond.type == 1)search_tuples_not_equal_inf(tablename, cond.lef_attr.attrname, cond.nval, ret_iter);
		else if (cond.type == 2)search_tuples_not_equal_inf(tablename, cond.lef_attr.attrname, cond.sval, ret_iter);
	}
	return ret_iter;
}

error MID_OP::mid_select_attr(vector<table_attr> ta, vector<string> tablename, vector<expression> cd) {//完成了简单的测试(单表测试)
	if (ta.size() == 1 && ta[0].tablename == "*")ta.clear();
	set<string> tbn_set;
	for (auto name : tablename){
		if (!search_table_exist_inf(name)){ printf("有不存在的表名\n"); return error(0); }
		tbn_set.insert(name);
	}
	tablename.clear();
	for (auto name : tbn_set){
		tablename.push_back(name);
	}
	for (auto p : ta){
		if (tbn_set.find(p.tablename) == tbn_set.end()){ printf("投影的来源表不存在\n"); return error(0); }
		if (exist_attr_in_table(p.tablename, p.attrname) == -1){ printf("投影的属性不存在\n"); return error(0); }
	}
	int tbsize = tablename.size();
	vector<expression> uni;//涉及两表的操作
	vector<bool> uni_used;
	vector<vector<expression> > self;
	self.resize(tbsize);
	//给属性归类
	for (auto p : cd){
		if (p.type == 0){
			string lt = p.lef_attr.tablename, rt = p.rig_attr.tablename;
			string lattr = p.lef_attr.attrname, rattr = p.rig_attr.attrname;
			if (tbn_set.find(lt) == tbn_set.end() || tbn_set.find(rt) == tbn_set.end()){ printf("关联比较表名不存在\n"); return error(0); }
			int ltype = exist_attr_in_table(lt, lattr);
			int rtype = exist_attr_in_table(rt, rattr);
			if (ltype == -1 || rtype == -1 || ltype != rtype){ printf("关联比较左右属性不存在或者类型不匹配\n"); return error(0); }
			if (ltype == 2 && (p.op != 0 && p.op != 5)){ printf("对于type=2只支持等于和不等于查询\n"); return error(0); }
			uni.push_back(p);
			uni_used.push_back(false);
		}
		else{
			string tn = p.lef_attr.tablename, attr = p.lef_attr.attrname;
			if (tbn_set.find(tn) == tbn_set.end()){ printf("比较左表名不存在\n"); return error(0); }
			int type = exist_attr_in_table(tn, attr);
			if (type == -1){ printf("比较左属性不存在\n"); return error(0); }
			if (type != p.type){ printf("条件语句左右类型不匹配\n"); return error(0); }
			if (type == 2 && (p.op != 0 && p.op != 5)){ printf("对于type=2只支持等于和不等于查询\n"); return error(0); }
			for (int i = 0; i < tbsize; i++){
				if (tablename[i] == tn){
					self[i].push_back(p);
					break;
				}
			}
		}
	}

	int unisiz = uni.size();

	RESULT finalret;
	set<string> have_done;
	for (int i = 0; i < tbsize; i++){
		have_done.insert(tablename[i]);
		RESULT cur_ret;
		vector<int> iter_vec;
		bool f = 0;
		for (int j = 0; j < self[i].size(); j++){
			if (f){//直接对临时表操作
				cur_ret = cur_ret.Select(cd[j]);
			}
			else{//否则使用指针求交集
				vector<int> cnt_iter_set = sega_query(tablename[i], cd[j]);
				if (j == 0)iter_vec = cnt_iter_set;
				else iter_vec = FILEP_intersection(iter_vec, cnt_iter_set);
				if (iter_vec.size() < 500){
					f = 1;
					cur_ret.init(tablename[i], iter_vec);
				}
			}
		}
		if (self[i].size() == 0)search_tuples_all_inf(tablename[i], iter_vec);//没有where语句
		if (f == 0)cur_ret.init(tablename[i], iter_vec);
		if (i == 0)finalret = cur_ret;
		else{
			finalret = Cartesian_Product(finalret, cur_ret);
		}
		for (int ii = 0; ii < unisiz; ii++){
			if (uni_used[ii])continue;
			if (have_done.find(uni[ii].lef_attr.tablename) == have_done.end())continue;
			if (have_done.find(uni[ii].rig_attr.tablename) == have_done.end())continue;
			finalret = finalret.Select(uni[ii]);
			uni_used[ii] = 1;
		}
	}
	if(ta.size())finalret=finalret.Projection(ta);
	finalret.print();
	printf("一共查询到了%d个元组\n", finalret.data_vec.size());
	return error(0);
}

error MID_OP::mid_delete_data(vector<string>tablename, vector<expression> cd) {
	if (tablename.size() != 1){
		printf("删除的表名的数量错误\n"); return error(0);
	}
	string tbn = tablename[0];
	if (!search_table_exist_inf(tbn)){
		printf("删除的表名不存在\n");
		return error(0);
	}
	for (int i = 0; i < cd.size(); i++){
		string tablename = cd[i].lef_attr.tablename;
		if (tablename != tbn){ printf("条件语句表名不正确\n"); return error(0); }
		string attrname = cd[i].lef_attr.attrname;
		int type = exist_attr_in_table(tablename, attrname);
		if (type == -1){ printf("属性名不正确\n"); return error(0); }
		if (cd[i].type != type){ printf("条件语句左右类型不匹配\n"); return error(0); }
		if (cd[i].type == 2 && cd[i].op != 0 && cd[i].op != 5){ printf("对于字符只支持等值和不等值查询\n"); return error(0); }
	}
	vector<int> data_iter_set;
	bool f = 0;
	RESULT ret;
	for (int i = 0; i < cd.size(); i++){
		if (f){//直接对临时表操作
			ret = ret.Select(cd[i]);
		}
		else{//否则使用指针求交集
			vector<int> cnt_iter_set = sega_query(tbn, cd[i]);
			if (i == 0)data_iter_set = cnt_iter_set;
			else data_iter_set = FILEP_intersection(data_iter_set, cnt_iter_set);
			if (data_iter_set.size() < 500){//表的size比较小，转为直接对表进行操作
				f = 1;
				ret.init(tbn, data_iter_set);
			}
		}
	}
	if (cd.size() == 0)search_tuples_all_inf(tbn, data_iter_set);//没有where语句--------------？？？？？？？？？？
	if (f)data_iter_set = ret.rec_addr;

	//开始删除
	delete_tuples_inf(tbn, data_iter_set);

	printf("成功删除了%d个数据\n", data_iter_set.size());
	return error(0);
}

error MID_OP::mid_insert(vector<string>tablename, vector<expression>cd) {
	//if (tablename.size() != 1){
	//	return error(10005);//插入的表的数量错误
	//}
	string tbn = tablename[0];
	//if (!search_table_exist_inf(tbn)){
	//	return error(10006);//插入的表名不存在
	//}
	set<string> s;
	for (int i = 0; i < cd.size(); i++){
		if (cd[i].type == 0 || cd[i].op != 0){ printf("插入语句表达式类型错误(右边必须是值且必须是=)\n"); return error(0); }
		if (cd[i].lef_attr.tablename != tbn){ printf("插入语句表名不正确\n"); return error(0); }
		int type = exist_attr_in_table(tbn, cd[i].lef_attr.attrname);
		if (type == -1){ printf("插入语句的属性不存在\n"); return error(0); }
		if (type != cd[i].type){ printf("赋值语句类型不匹配\n"); return error(0); }
		
		if (s.find(cd[i].lef_attr.attrname) != s.end()){ printf("存在重复的赋值语句\n"); return error(0); }
		else s.insert(cd[i].lef_attr.attrname);
	}
	//读取表的属性
	vector<string> cnt_attr_vec = get_all_attr(tbn);
	if (cd.size() != cnt_attr_vec.size()){ printf("赋值语句不完整，有的属性没有被赋值到\n"); return error(0); }
	int attr_siz = cd.size();
	//为record赋值
	Record ccc;
	for (int i = 0; i < attr_siz; i++){
		for (int j = 0; j < attr_siz; j++){
			if (cd[i].lef_attr.attrname == cnt_attr_vec[j]){
				if (cd[i].type == 1)ccc.data[j] = (void*)&cd[i].nval;
				else if (cd[i].type == 2){
					//char data[50];
					//strcpy(data, cd[i].sval.c_str());
					ccc.data[j] = (void*)cd[i].sval.c_str();
				}
				break;
			}
		}
	}
	//插入
	if (!insert_tuple_inf(tbn, ccc)){ printf("主码重复\n"); return error(0); }
	printf("成功插入数据\n");
	return error(0);
}

//表名，表达式(=的)，条件语句
error MID_OP::mid_update_data(vector<string> tablename, vector<expression> exp, vector<expression> cd) {
	if (tablename.size() != 1){
		printf("更新的表的数量错误\n");
		return error(0);
	}
	string tbn = tablename[0];
	if (!search_table_exist_inf(tbn)){
		printf("更新的表名不存在\n");
		return error(0);
	}
	for (int i = 0; i < exp.size(); i++){
		if (exp[i].lef_attr.tablename != tbn){ printf("更新语句表名不正确\n"); return error(0); }
		if (exist_attr_in_table(tbn, exp[i].lef_attr.attrname) == -1){ printf("更新语句的属性不存在\n"); return error(0); }
	}
	//=======================condition的预处理和分组============================
	for (int i = 0; i < cd.size(); i++){
		string tablename = cd[i].lef_attr.tablename;
		if (tablename != tbn){ printf("条件语句表名不正确\n"); return error(0); }
		string attrname = cd[i].lef_attr.attrname;
		int type = exist_attr_in_table(tablename, attrname);
		if (type == -1){ printf("属性名不正确\n"); return error(0); }
		if (cd[i].type != type){ printf("条件语句左右类型不匹配\n"); return error(0); }
		if (cd[i].type == 2 && cd[i].op != 0 && cd[i].op != 5){ printf("对于字符只支持等值和不等值查询\n"); return error(0); }
	}
	vector<int> data_iter_set;
	bool f = 0;
	RESULT ret;
	for (int i = 0; i < cd.size(); i++){
		if (f){//直接对临时表操作
			ret = ret.Select(cd[i]);
		}
		else{//否则使用指针求交集
			vector<int> cnt_iter_set = sega_query(tbn, cd[i]);
			if (i == 0)data_iter_set = cnt_iter_set;
			else data_iter_set = FILEP_intersection(data_iter_set, cnt_iter_set);
			if (data_iter_set.size() < 500){
				f = 1;
				ret.init(tbn,data_iter_set);
			}
		}
	}
	if (cd.size() == 0)search_tuples_all_inf(tbn, data_iter_set);//没有where语句--------------？？？？？？？？？？
	if (f == 0)ret.init(tbn, data_iter_set);
	int siz = ret.rec_addr.size();

	//属性名转下标
	vector<pair<int,int> > changid;
	vector<string> cnt_attr_vec = get_all_attr(tbn);
	int attr_siz = cnt_attr_vec.size();
	for (auto cc : exp){
		for (int i = 0; i < attr_siz; i++){
			if (cc.lef_attr.attrname == cnt_attr_vec[i]){
				changid.push_back(make_pair(i,exist_attr_in_table(tbn,cnt_attr_vec[i])));
				break;
			}
		}
	}

	//开始修改
	int changsiz = changid.size();
	for (int i = 0; i < siz;i++){
		int addr = ret.rec_addr[i];
		for (int j = 0; j < changsiz;j++){
			int col = changid[j].first, type = changid[j].second;
			string attrname = exp[j].lef_attr.attrname;
			if (type == 1)change_tuple_inf(tbn, attrname, *(double*)ret.data_vec[i][col], exp[j].nval, addr);
			else if (type == 2)change_tuple_inf(tbn, attrname, string((char*)ret.data_vec[i][col]), exp[j].sval, addr);
		}
	}

	printf("成功修改了%d个数据\n", ret.rec_addr.size());
	return error(0);
}

error MID_OP::mid_drop_table(string tablename) {
	if (!search_table_exist_inf(tablename)){ printf("表名不存在\n"); return error(0); }
	drop_table_inf(tablename);
	printf("成功删除表\n");
	return error(0);
}

error MID_OP::mid_create_index(table_attr ta) {
	if (exist_attr_in_table(ta.tablename, ta.attrname) == -1){ printf("表中不存在该属性\n"); return error(0); }
	if (create_index_inf(ta.tablename, ta.attrname)) {
		printf("创建索引成功\n");
	}
	else printf("创建索引失败\n");
	return error(0);
}

error MID_OP::mid_drop_index(table_attr ta) {
	if (!search_index_exist_inf(ta.tablename, ta.attrname)){ printf("索引不存在\n"); return error(0); }
	bool f = delete_index_inf(ta.tablename, ta.attrname);
	if (f == 0){ printf("索引是主码,删除失败\n"); return error(0); }
	printf("删除索引成功\n");
	return error(0);
}

error MID_OP::mid_create_table(string tablename, vector<Base_attribute_sega> baseattr) {
	//检查表名是否存在
	if (search_table_exist_inf(tablename)){
		printf("表名已经存在\n");
		return error(0);
	}
	//检查是否存在多个主码
	int attrnum = baseattr.size();
	int pknum = 0;
	string keyname;
	for (int i = 0; i < attrnum; i++){
		if (baseattr[i].key == 1 || baseattr[i].key == 3){
			pknum++;
			keyname = baseattr[i].name;
		}
	}
	if (pknum == 0) { printf("缺少主码\n"); return error(0); }
	if (pknum>1) { printf("存在多个主码\n"); return error(0); }
	//检查外码合法性

	vector<string>col_nam;
	vector<int>typ;
	vector<int>siz;
	for (int i = 0; i < attrnum; i++){
		/*
		if (baseattr[i].key == 2 || baseattr[i].key == 3){
			string otn = baseattr[i].outer_code.tablename;
			if (!search_table_exist_inf(otn)){
				return error(14);//外码参照的表名不存在
			}
			string ota = baseattr[i].outer_code.attrname;
			int typecode = exist_attr_in_table(otn, ota);
			if (typecode == -1)return error(14);//外码属性不存在
			if (typecode != baseattr[i].type)return error(15);//外码类型不匹配
		}
		*/
		col_nam.push_back(baseattr[i].name);
		typ.push_back(baseattr[i].type);
		siz.push_back(20);

	}
	//建表
	create_table_inf(tablename, keyname, col_nam, typ, siz);
	printf("建表成功\n");
	return error(0);
}


RESULT pre_table;

RESULT MID_OP::pre_table_select_attr( vector<expression> cd) {
	RESULT ret = pre_table;
	for (auto p : cd){
		ret=ret.Select(p);
	}
	return ret;
}


RESULT  MID_OP::mid_query_middle(vector<string> &tbn, vector<string> &word, vector<int> jmp, int lef, int rig){
	bool f = 0;
	for (int i = lef; i <= rig; i++){
		if (word[i] == "|"){
			f = 1; break;
		}
	}
	if (f == 0){//如果整个表达式都没有|,那么可以无视所有的括号
		vector<expression> exp;
		expression cnt;
		for (int i = lef; i <= rig; i++){
			if (word[i] == "(" || word[i] == ")"||word[i]=="&")continue;
			cnt.parse(word[i]);
			exp.push_back(cnt);
		}
		return  MID_OP::pre_table_select_attr(exp);
	}

	if (word[lef] == "("&&jmp[lef] == rig)return mid_query_middle( tbn, word, jmp, lef + 1, rig - 1);

	RESULT finalret;
	f = 0;
	vector<int> orid;
	orid.push_back(lef - 1);//first divide by |
	for (int i = lef; i <= rig; i++){
		if (word[i] == "("){
			i = jmp[i];
			continue; 
		}
		else if (word[i] == "|"){
			orid.push_back(i);
		}
	}
	orid.push_back(rig + 1);
	if (orid.size() > 2){//有|符号
		for (int i = 0; i < orid.size()-1; i++){
			RESULT cntret = mid_query_middle( tbn, word, jmp, orid[i] + 1, orid[i + 1] - 1);
			if (f == 0){ finalret = cntret; f = 1; }
			else { bool f = 1;  finalret = brute_force_union(finalret, cntret, f); }
		}
	}
	else{//second divide by()
		vector<expression> exp;
		for (int i = lef; i <= rig; i++){
			if (word[i] == "("){
				RESULT cntret = mid_query_middle(tbn, word, jmp, i + 1, jmp[i] - 1);
				if (f == 0){ finalret = cntret; f = 1; }
				else {
					bool f = 1; finalret = MID_OP::brute_force_intersect(finalret, cntret,f);
				}
				i = jmp[i];
			}
			else if (word[i] == "&")continue;
			else {
				expression cc;
				cc.parse(word[i]);
				exp.push_back(cc);
			}
		}
		RESULT cntret = MID_OP::pre_table_select_attr(exp);
		if (f == 0){ finalret = cntret; f = 1; }
		else {
			bool f = 1;
			finalret = MID_OP::brute_force_intersect(finalret, cntret,f);
		}
	}
	//thrid search
	return finalret;
}

// ( ( s1 & s2 ) | ( s3 & s4 ) ) & s5
error MID_OP::mid_query_outer(vector<table_attr> ta, vector<string> tbn, vector<string> word,RESULT & return_ret){
	if (ta.size() == 1 && ta[0].tablename == "*")ta.clear();
	set<string> tbn_set;
	for (auto name : tbn){
		if (!search_table_exist_inf(name)){ printf("有不存在的表名\n"); return error(0); }
		if (tbn_set.find(name) != tbn_set.end()){ printf("重复的表名\n"); return error(0); }
		tbn_set.insert(name);
	}
	for (auto p : ta){
		if (tbn_set.find(p.tablename) == tbn_set.end()){ printf("投影的来源表不存在\n"); return error(0); }
		if (exist_attr_in_table(p.tablename, p.attrname) == -1){ printf("投影的属性不存在\n"); return error(0); }
	}
	for (auto w : word){
		if (w == "(" || w == ")" || w == "|" || w == "&")continue;
		expression cc;
		if (cc.parse(w) != 1){ printf("条件语句语法错误\n"); return error(0); }
		if (cc.type == 0){
			if (tbn_set.find(cc.lef_attr.tablename) == tbn_set.end()){ printf("关联比较表名不存在\n"); return error(0); }
			if (tbn_set.find(cc.rig_attr.tablename) == tbn_set.end()){ printf("关联比较表名不存在\n"); return error(0); }
			int ltype = exist_attr_in_table(cc.lef_attr.tablename, cc.lef_attr.attrname);
			int rtype = exist_attr_in_table(cc.rig_attr.tablename, cc.rig_attr.attrname);
			if (ltype == -1 || rtype == -1 || ltype != rtype){ printf("关联比较左右属性不存在或者类型不匹配\n"); return error(0); }
			if (ltype == 2 && (cc.op != 0 && cc.op != 5)){ printf("对于type=2只支持等于和不等于查询\n"); return error(0); }
		}
		else{
			string tn = cc.lef_attr.tablename, attr = cc.lef_attr.attrname;
			if (tbn_set.find(tn) == tbn_set.end()){ printf("比较左表名不存在\n"); return error(0); }
			int type = exist_attr_in_table(tn, attr);
			if (type == -1){ printf("比较左属性不存在\n"); return error(0); }
			if (type != cc.type){ printf("条件语句左右类型不匹配\n"); return error(0); }
			if (type == 2 && (cc.op != 0 && cc.op != 5)){ printf("对于type=2只支持等于和不等于查询\n"); return error(0); }
		}
	}
	//--------------以上是错误检查-----------------

	int siz = word.size();
	vector<int> rig;
	rig.resize(siz);
	stack<int> sta;
	expression cc;
	for (int i = 0; i<siz; i++){
		if (i<siz - 1 && word[i] == ")"&&word[i + 1] == "("){
			printf("存在括号对之间无布尔运算符");
			return error(0);
		}
		if (word[i] == "("){
			sta.push(i);
		}
		else if (word[i] == ")"){
			if (sta.empty()){
				printf("条件语句括号不匹配\n");
				return error(0);
			}
			else{
				int lef = sta.top(); sta.pop();
				rig[lef] = i;
			}
		}
		else if(word[i]!="|"&&word[i]!="&"){
			if (cc.parse(word[i]) != 1){
				printf("表达式解析错误\n");
				return error(0);
			}
		}
	}
	if (!sta.empty()){
		printf("条件语句括号不匹配\n");
		return error(0);
	}

	//----------------预先把表弄出来-----------------
	pre_table.clear();
	bool f = 0;
	for (auto name : tbn_set){
		vector<int> iter_vec;
		search_tuples_all_inf(name, iter_vec);
		RESULT cnt;
		cnt.init(name, iter_vec);
		if (f == 0){
			pre_table = cnt;
			f = 1;
		}
		else pre_table = Cartesian_Product(pre_table, cnt);
	}
	pre_table.ReNumber();//对表编号

	RESULT finalret = mid_query_middle(tbn, word, rig, 0, siz - 1);
	if (ta.size() > 0)finalret = finalret.Projection(ta);
	//finalret.print();
	//printf("共查询到%d个数据\n", finalret.data_vec.size());
	return_ret = finalret;
	return error(10086);
}


error MID_OP::mid_query_data_dic(string tbn){
//	void read_table_meta_inf(string tablename, table &table_meta);									//读取数据字典，返回在table_meta中
	if (!search_table_exist_inf(tbn)){ printf("查询的表名不存在\n"); return error(0); }
	table ToT;
	read_table_meta_inf(tbn, ToT);
	cout << "表名:" << tbn << endl;
	printf("属性名:\t");
	for (int i = 0; i < ToT.count; i++){
		printf("%s\t", ToT.col[i].col_name);
	}
	putchar('\n');
	for (int i = 0; i <= 60; i++)putchar('-');
	putchar('\n');
	printf("类型:\t");
	for (int i = 0; i < ToT.count; i++){
		if (ToT.col[i].type == 1)printf("double\t");
		else printf("char\t");
	}
	putchar('\n');
	printf("主码:\t%s\n",ToT.main_key);
	return error(0);
}
