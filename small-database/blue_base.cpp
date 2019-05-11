#pragma once
#include "blue_base.h"
#include "inf.h"
using namespace blue_base;

inline bool is_alpha(char cc) {//�ǲ�����ĸ
	if (cc >= 'a'&&cc <= 'z')return 1;
	if (cc >= 'A'&&cc <= 'Z')return 1;
	return 0;
}

inline bool is_num_bit(char cc) {//�ǲ�������
	if (cc >= '0'&&cc <= '9')return 1;
	return 0;
}

/*
inline bool is_num(string str, int &num) {//��������ֵ
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

inline bool is_num(string str, double &num) {//�����ɸ�����==================δ����
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

inline bool is_string_const(string str) {//�涨�ַ���������������ĸ��������Ķ���
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

inline bool is_legal_user_id(string str) {//�ж��ǲ��ǺϷ��ı�ʶ��
	int len = str.length();
	if (len == 0)return 0;
	if (!is_alpha(str[0]))return 0;
	for (int i = 0; i < len; i++) {
		if (!is_alpha(str[i]) && !is_num_bit(str[i]))return 0;
	}
	if (is_keep_word(str))return 0;//�Ǳ�����
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
inline int get_op_id(string str) {//�ж��ǲ��ǱȽϷ���
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

int table_attr::parse(string str) {//���ַ���������table��attr����ʽ��ʧ���򷵻�false;
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
	//�Ȳ��һ��һ�����ʣ�Ȼ����ʽ�жϼ���
	vector<string> word_vec = space_extraction(str);
	if (word_vec.size() < 2)return -1;//too short

	if (word_vec[1] == "double")type = 1;
	else if (word_vec[1] == "char")type = 2;//ƥ��ײ���޸ĵ�-------------2108.11.17
	else return -2;//�������ô���

	if (!is_legal_user_id(word_vec[0])) {
		return -3;//���ǺϷ��ı�־��
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
		if (!outer_code.parse(word_vec[3]))return -6;//�������ʧ��
		key = 2;
		return 1;
	}
	else if (word_vec.size() == 5) {
		if (word_vec[2] != "primary_key")return -4;
		if (word_vec[3] != "references")return -5;
		if (!outer_code.parse(word_vec[4]))return -6;//�������ʧ��
		key = 3;
		return 1;
	}
	return 0;
}

string error::geterror() {
	switch (this->error_type) {
	case 1: return string("û�д���\n");
	case 2: return string("������Ч\n");
	case 3: return string("��ʶ����Ч\n");
	case 4: return string("��Ч��������\n");
	case 5: return string("ѡ��ȱʧ����Ч\n");		//ȱʧ�����ţ������ź��治�ӡ���������һЩ���߰���Ķ���
	case 6: return string("ȱʧ������\n");
	case 7: return string("��Ч�Ĺ�ϵ�����\n");	//where �����������
	case 8: return string("��Ч����\n");			//��������Ӧ��ֵΪ�ַ���
	case 9: return string("ȱʧ�Ⱥ�\n");			//����=���ʽ
	case 10: return string("������\n");
	case 11: return string("���Ѿ�������\n");
	case 12: return string("����ȱʧ\n");
	case 13: return string("���ڶ������\n");
	default: return "";
	}
}

int expression::parse(string str) {//�����ɱ��ʽ
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
	}//������������
	int riglen = rigstr.length();
	if (riglen > 2) {
		if (rigstr[0] == '"'&&rigstr[riglen - 1] == '"') {//�ж��ǲ����ַ�������
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
	if (is_num(rigstr, num)) {//�ж��ǲ�����ֵ
		type = 1;
		nval = num;
		sval = "";
		return 1;
	}
	if (!rig_attr.parse(rigstr))return -14;//������������
	else {
		type = 0;
		nval = 0;
		sval = "";
	}
	return 1;
}


//===============================��Ĳ���================================
//where table.sno in (
		//SELETE Sno 
		//From student
		//where sdept='CS'	
//)̫���˲�ʵ����

RESULT::RESULT(string tablename, vector<FILEP> points_to){//���캯����������������������������������δ����
	init(tablename, points_to);
}

void RESULT::init(string tablename, vector<FILEP> points_to){//ʹ��ָ�뼯�ϳ�ʼ��
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

RESULT RESULT::Projection(vector<table_attr> ta){//ͶӰ
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

RESULT RESULT::Select(expression cd){//ѡ��(����ɸѡ)_______�����ڲ����ٽ��м���ˣ����������������
	if (cd.type == 0){//���ԱȽ�
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
			if ((type == 1 && satisfy(cd.op, type, data_vec[i][id], (void*)&cd.nval)) || (type == 2 && satisfy(cd.op, type, data_vec[i][id], (void*)(cd.sval.c_str())))){//���������ת����֪���ܲ�����
				newt.data_vec.push_back(this->data_vec[i]);
				newt.rec_addr.push_back(this->rec_addr[i]);
				newt.id.push_back(this->id[i]);
			}
		}
		return newt;
	}
}

RESULT Cartesian_Product(RESULT A, RESULT B){//�ѿ�����
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
			newt.rec_addr[id] = A.rec_addr[i];//���Ե�ַ�����ظ���
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
		printf("union ���ڴ���\n");
		return cc;
	}
	for (int i = 0; i < lattrsiz; i++) {
		if (A.attr_vec[i].tablename != B.attr_vec[i].tablename||A.attr_vec[i].attrname!=B.attr_vec[i].attrname) {
			f = 0;
			printf("union ���ڴ���\n");
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
		printf("intersect ���ڴ���\n");
		return cc;
	}
	for (int i = 0; i < lattrsiz; i++) {
		if (A.attr_vec[i].tablename != B.attr_vec[i].tablename || A.attr_vec[i].attrname != B.attr_vec[i].attrname) {
			f = 0;
			RESULT cc;
			printf("intersect ���ڴ���\n");
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


vector<FILEP> FILEP_intersection(vector<FILEP> lef, vector<FILEP> rig){//������Ԫ�鼯�ϵĽ���
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

vector<FILEP> FILEP_union(vector<FILEP> lef, vector<FILEP> rig){//������Ԫ�鼯�ϵĲ���
	vector <FILEP> ret=lef;
	for (int i = 0; i < rig.size(); i++){
		ret.push_back(rig[i]);
	}
	sort(ret.begin(), ret.end());
	vector<FILEP>::iterator new_end = unique(ret.begin(), ret.end());
	ret.erase(new_end, ret.end());//ɾ��(������ɾ��)�ظ���Ԫ��
	return ret;
}

vector<int> sega_query(string tablename, expression cond){//�����͵�������,���õײ����,�Ѿ���֤�Ϸ�����
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

error MID_OP::mid_select_attr(vector<table_attr> ta, vector<string> tablename, vector<expression> cd) {//����˼򵥵Ĳ���(�������)
	if (ta.size() == 1 && ta[0].tablename == "*")ta.clear();
	set<string> tbn_set;
	for (auto name : tablename){
		if (!search_table_exist_inf(name)){ printf("�в����ڵı���\n"); return error(0); }
		tbn_set.insert(name);
	}
	tablename.clear();
	for (auto name : tbn_set){
		tablename.push_back(name);
	}
	for (auto p : ta){
		if (tbn_set.find(p.tablename) == tbn_set.end()){ printf("ͶӰ����Դ������\n"); return error(0); }
		if (exist_attr_in_table(p.tablename, p.attrname) == -1){ printf("ͶӰ�����Բ�����\n"); return error(0); }
	}
	int tbsize = tablename.size();
	vector<expression> uni;//�漰����Ĳ���
	vector<bool> uni_used;
	vector<vector<expression> > self;
	self.resize(tbsize);
	//�����Թ���
	for (auto p : cd){
		if (p.type == 0){
			string lt = p.lef_attr.tablename, rt = p.rig_attr.tablename;
			string lattr = p.lef_attr.attrname, rattr = p.rig_attr.attrname;
			if (tbn_set.find(lt) == tbn_set.end() || tbn_set.find(rt) == tbn_set.end()){ printf("�����Ƚϱ���������\n"); return error(0); }
			int ltype = exist_attr_in_table(lt, lattr);
			int rtype = exist_attr_in_table(rt, rattr);
			if (ltype == -1 || rtype == -1 || ltype != rtype){ printf("�����Ƚ��������Բ����ڻ������Ͳ�ƥ��\n"); return error(0); }
			if (ltype == 2 && (p.op != 0 && p.op != 5)){ printf("����type=2ֻ֧�ֵ��ںͲ����ڲ�ѯ\n"); return error(0); }
			uni.push_back(p);
			uni_used.push_back(false);
		}
		else{
			string tn = p.lef_attr.tablename, attr = p.lef_attr.attrname;
			if (tbn_set.find(tn) == tbn_set.end()){ printf("�Ƚ������������\n"); return error(0); }
			int type = exist_attr_in_table(tn, attr);
			if (type == -1){ printf("�Ƚ������Բ�����\n"); return error(0); }
			if (type != p.type){ printf("��������������Ͳ�ƥ��\n"); return error(0); }
			if (type == 2 && (p.op != 0 && p.op != 5)){ printf("����type=2ֻ֧�ֵ��ںͲ����ڲ�ѯ\n"); return error(0); }
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
			if (f){//ֱ�Ӷ���ʱ�����
				cur_ret = cur_ret.Select(cd[j]);
			}
			else{//����ʹ��ָ���󽻼�
				vector<int> cnt_iter_set = sega_query(tablename[i], cd[j]);
				if (j == 0)iter_vec = cnt_iter_set;
				else iter_vec = FILEP_intersection(iter_vec, cnt_iter_set);
				if (iter_vec.size() < 500){
					f = 1;
					cur_ret.init(tablename[i], iter_vec);
				}
			}
		}
		if (self[i].size() == 0)search_tuples_all_inf(tablename[i], iter_vec);//û��where���
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
	printf("һ����ѯ����%d��Ԫ��\n", finalret.data_vec.size());
	return error(0);
}

error MID_OP::mid_delete_data(vector<string>tablename, vector<expression> cd) {
	if (tablename.size() != 1){
		printf("ɾ���ı�������������\n"); return error(0);
	}
	string tbn = tablename[0];
	if (!search_table_exist_inf(tbn)){
		printf("ɾ���ı���������\n");
		return error(0);
	}
	for (int i = 0; i < cd.size(); i++){
		string tablename = cd[i].lef_attr.tablename;
		if (tablename != tbn){ printf("��������������ȷ\n"); return error(0); }
		string attrname = cd[i].lef_attr.attrname;
		int type = exist_attr_in_table(tablename, attrname);
		if (type == -1){ printf("����������ȷ\n"); return error(0); }
		if (cd[i].type != type){ printf("��������������Ͳ�ƥ��\n"); return error(0); }
		if (cd[i].type == 2 && cd[i].op != 0 && cd[i].op != 5){ printf("�����ַ�ֻ֧�ֵ�ֵ�Ͳ���ֵ��ѯ\n"); return error(0); }
	}
	vector<int> data_iter_set;
	bool f = 0;
	RESULT ret;
	for (int i = 0; i < cd.size(); i++){
		if (f){//ֱ�Ӷ���ʱ�����
			ret = ret.Select(cd[i]);
		}
		else{//����ʹ��ָ���󽻼�
			vector<int> cnt_iter_set = sega_query(tbn, cd[i]);
			if (i == 0)data_iter_set = cnt_iter_set;
			else data_iter_set = FILEP_intersection(data_iter_set, cnt_iter_set);
			if (data_iter_set.size() < 500){//���size�Ƚ�С��תΪֱ�ӶԱ���в���
				f = 1;
				ret.init(tbn, data_iter_set);
			}
		}
	}
	if (cd.size() == 0)search_tuples_all_inf(tbn, data_iter_set);//û��where���--------------��������������������
	if (f)data_iter_set = ret.rec_addr;

	//��ʼɾ��
	delete_tuples_inf(tbn, data_iter_set);

	printf("�ɹ�ɾ����%d������\n", data_iter_set.size());
	return error(0);
}

error MID_OP::mid_insert(vector<string>tablename, vector<expression>cd) {
	//if (tablename.size() != 1){
	//	return error(10005);//����ı����������
	//}
	string tbn = tablename[0];
	//if (!search_table_exist_inf(tbn)){
	//	return error(10006);//����ı���������
	//}
	set<string> s;
	for (int i = 0; i < cd.size(); i++){
		if (cd[i].type == 0 || cd[i].op != 0){ printf("���������ʽ���ʹ���(�ұ߱�����ֵ�ұ�����=)\n"); return error(0); }
		if (cd[i].lef_attr.tablename != tbn){ printf("��������������ȷ\n"); return error(0); }
		int type = exist_attr_in_table(tbn, cd[i].lef_attr.attrname);
		if (type == -1){ printf("�����������Բ�����\n"); return error(0); }
		if (type != cd[i].type){ printf("��ֵ������Ͳ�ƥ��\n"); return error(0); }
		
		if (s.find(cd[i].lef_attr.attrname) != s.end()){ printf("�����ظ��ĸ�ֵ���\n"); return error(0); }
		else s.insert(cd[i].lef_attr.attrname);
	}
	//��ȡ�������
	vector<string> cnt_attr_vec = get_all_attr(tbn);
	if (cd.size() != cnt_attr_vec.size()){ printf("��ֵ��䲻�������е�����û�б���ֵ��\n"); return error(0); }
	int attr_siz = cd.size();
	//Ϊrecord��ֵ
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
	//����
	if (!insert_tuple_inf(tbn, ccc)){ printf("�����ظ�\n"); return error(0); }
	printf("�ɹ���������\n");
	return error(0);
}

//���������ʽ(=��)���������
error MID_OP::mid_update_data(vector<string> tablename, vector<expression> exp, vector<expression> cd) {
	if (tablename.size() != 1){
		printf("���µı����������\n");
		return error(0);
	}
	string tbn = tablename[0];
	if (!search_table_exist_inf(tbn)){
		printf("���µı���������\n");
		return error(0);
	}
	for (int i = 0; i < exp.size(); i++){
		if (exp[i].lef_attr.tablename != tbn){ printf("��������������ȷ\n"); return error(0); }
		if (exist_attr_in_table(tbn, exp[i].lef_attr.attrname) == -1){ printf("�����������Բ�����\n"); return error(0); }
	}
	//=======================condition��Ԥ����ͷ���============================
	for (int i = 0; i < cd.size(); i++){
		string tablename = cd[i].lef_attr.tablename;
		if (tablename != tbn){ printf("��������������ȷ\n"); return error(0); }
		string attrname = cd[i].lef_attr.attrname;
		int type = exist_attr_in_table(tablename, attrname);
		if (type == -1){ printf("����������ȷ\n"); return error(0); }
		if (cd[i].type != type){ printf("��������������Ͳ�ƥ��\n"); return error(0); }
		if (cd[i].type == 2 && cd[i].op != 0 && cd[i].op != 5){ printf("�����ַ�ֻ֧�ֵ�ֵ�Ͳ���ֵ��ѯ\n"); return error(0); }
	}
	vector<int> data_iter_set;
	bool f = 0;
	RESULT ret;
	for (int i = 0; i < cd.size(); i++){
		if (f){//ֱ�Ӷ���ʱ�����
			ret = ret.Select(cd[i]);
		}
		else{//����ʹ��ָ���󽻼�
			vector<int> cnt_iter_set = sega_query(tbn, cd[i]);
			if (i == 0)data_iter_set = cnt_iter_set;
			else data_iter_set = FILEP_intersection(data_iter_set, cnt_iter_set);
			if (data_iter_set.size() < 500){
				f = 1;
				ret.init(tbn,data_iter_set);
			}
		}
	}
	if (cd.size() == 0)search_tuples_all_inf(tbn, data_iter_set);//û��where���--------------��������������������
	if (f == 0)ret.init(tbn, data_iter_set);
	int siz = ret.rec_addr.size();

	//������ת�±�
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

	//��ʼ�޸�
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

	printf("�ɹ��޸���%d������\n", ret.rec_addr.size());
	return error(0);
}

error MID_OP::mid_drop_table(string tablename) {
	if (!search_table_exist_inf(tablename)){ printf("����������\n"); return error(0); }
	drop_table_inf(tablename);
	printf("�ɹ�ɾ����\n");
	return error(0);
}

error MID_OP::mid_create_index(table_attr ta) {
	if (exist_attr_in_table(ta.tablename, ta.attrname) == -1){ printf("���в����ڸ�����\n"); return error(0); }
	if (create_index_inf(ta.tablename, ta.attrname)) {
		printf("���������ɹ�\n");
	}
	else printf("��������ʧ��\n");
	return error(0);
}

error MID_OP::mid_drop_index(table_attr ta) {
	if (!search_index_exist_inf(ta.tablename, ta.attrname)){ printf("����������\n"); return error(0); }
	bool f = delete_index_inf(ta.tablename, ta.attrname);
	if (f == 0){ printf("����������,ɾ��ʧ��\n"); return error(0); }
	printf("ɾ�������ɹ�\n");
	return error(0);
}

error MID_OP::mid_create_table(string tablename, vector<Base_attribute_sega> baseattr) {
	//�������Ƿ����
	if (search_table_exist_inf(tablename)){
		printf("�����Ѿ�����\n");
		return error(0);
	}
	//����Ƿ���ڶ������
	int attrnum = baseattr.size();
	int pknum = 0;
	string keyname;
	for (int i = 0; i < attrnum; i++){
		if (baseattr[i].key == 1 || baseattr[i].key == 3){
			pknum++;
			keyname = baseattr[i].name;
		}
	}
	if (pknum == 0) { printf("ȱ������\n"); return error(0); }
	if (pknum>1) { printf("���ڶ������\n"); return error(0); }
	//�������Ϸ���

	vector<string>col_nam;
	vector<int>typ;
	vector<int>siz;
	for (int i = 0; i < attrnum; i++){
		/*
		if (baseattr[i].key == 2 || baseattr[i].key == 3){
			string otn = baseattr[i].outer_code.tablename;
			if (!search_table_exist_inf(otn)){
				return error(14);//������յı���������
			}
			string ota = baseattr[i].outer_code.attrname;
			int typecode = exist_attr_in_table(otn, ota);
			if (typecode == -1)return error(14);//�������Բ�����
			if (typecode != baseattr[i].type)return error(15);//�������Ͳ�ƥ��
		}
		*/
		col_nam.push_back(baseattr[i].name);
		typ.push_back(baseattr[i].type);
		siz.push_back(20);

	}
	//����
	create_table_inf(tablename, keyname, col_nam, typ, siz);
	printf("����ɹ�\n");
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
	if (f == 0){//����������ʽ��û��|,��ô�����������е�����
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
	if (orid.size() > 2){//��|����
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
		if (!search_table_exist_inf(name)){ printf("�в����ڵı���\n"); return error(0); }
		if (tbn_set.find(name) != tbn_set.end()){ printf("�ظ��ı���\n"); return error(0); }
		tbn_set.insert(name);
	}
	for (auto p : ta){
		if (tbn_set.find(p.tablename) == tbn_set.end()){ printf("ͶӰ����Դ������\n"); return error(0); }
		if (exist_attr_in_table(p.tablename, p.attrname) == -1){ printf("ͶӰ�����Բ�����\n"); return error(0); }
	}
	for (auto w : word){
		if (w == "(" || w == ")" || w == "|" || w == "&")continue;
		expression cc;
		if (cc.parse(w) != 1){ printf("��������﷨����\n"); return error(0); }
		if (cc.type == 0){
			if (tbn_set.find(cc.lef_attr.tablename) == tbn_set.end()){ printf("�����Ƚϱ���������\n"); return error(0); }
			if (tbn_set.find(cc.rig_attr.tablename) == tbn_set.end()){ printf("�����Ƚϱ���������\n"); return error(0); }
			int ltype = exist_attr_in_table(cc.lef_attr.tablename, cc.lef_attr.attrname);
			int rtype = exist_attr_in_table(cc.rig_attr.tablename, cc.rig_attr.attrname);
			if (ltype == -1 || rtype == -1 || ltype != rtype){ printf("�����Ƚ��������Բ����ڻ������Ͳ�ƥ��\n"); return error(0); }
			if (ltype == 2 && (cc.op != 0 && cc.op != 5)){ printf("����type=2ֻ֧�ֵ��ںͲ����ڲ�ѯ\n"); return error(0); }
		}
		else{
			string tn = cc.lef_attr.tablename, attr = cc.lef_attr.attrname;
			if (tbn_set.find(tn) == tbn_set.end()){ printf("�Ƚ������������\n"); return error(0); }
			int type = exist_attr_in_table(tn, attr);
			if (type == -1){ printf("�Ƚ������Բ�����\n"); return error(0); }
			if (type != cc.type){ printf("��������������Ͳ�ƥ��\n"); return error(0); }
			if (type == 2 && (cc.op != 0 && cc.op != 5)){ printf("����type=2ֻ֧�ֵ��ںͲ����ڲ�ѯ\n"); return error(0); }
		}
	}
	//--------------�����Ǵ�����-----------------

	int siz = word.size();
	vector<int> rig;
	rig.resize(siz);
	stack<int> sta;
	expression cc;
	for (int i = 0; i<siz; i++){
		if (i<siz - 1 && word[i] == ")"&&word[i + 1] == "("){
			printf("�������Ŷ�֮���޲��������");
			return error(0);
		}
		if (word[i] == "("){
			sta.push(i);
		}
		else if (word[i] == ")"){
			if (sta.empty()){
				printf("����������Ų�ƥ��\n");
				return error(0);
			}
			else{
				int lef = sta.top(); sta.pop();
				rig[lef] = i;
			}
		}
		else if(word[i]!="|"&&word[i]!="&"){
			if (cc.parse(word[i]) != 1){
				printf("���ʽ��������\n");
				return error(0);
			}
		}
	}
	if (!sta.empty()){
		printf("����������Ų�ƥ��\n");
		return error(0);
	}

	//----------------Ԥ�Ȱѱ�Ū����-----------------
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
	pre_table.ReNumber();//�Ա���

	RESULT finalret = mid_query_middle(tbn, word, rig, 0, siz - 1);
	if (ta.size() > 0)finalret = finalret.Projection(ta);
	//finalret.print();
	//printf("����ѯ��%d������\n", finalret.data_vec.size());
	return_ret = finalret;
	return error(10086);
}


error MID_OP::mid_query_data_dic(string tbn){
//	void read_table_meta_inf(string tablename, table &table_meta);									//��ȡ�����ֵ䣬������table_meta��
	if (!search_table_exist_inf(tbn)){ printf("��ѯ�ı���������\n"); return error(0); }
	table ToT;
	read_table_meta_inf(tbn, ToT);
	cout << "����:" << tbn << endl;
	printf("������:\t");
	for (int i = 0; i < ToT.count; i++){
		printf("%s\t", ToT.col[i].col_name);
	}
	putchar('\n');
	for (int i = 0; i <= 60; i++)putchar('-');
	putchar('\n');
	printf("����:\t");
	for (int i = 0; i < ToT.count; i++){
		if (ToT.col[i].type == 1)printf("double\t");
		else printf("char\t");
	}
	putchar('\n');
	printf("����:\t%s\n",ToT.main_key);
	return error(0);
}
