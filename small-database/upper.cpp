#pragma once
#include "blue_base.h"
using namespace std;
using namespace blue_base;
//�� (,) (��) (�ո�) Ϊ�ֽ����
//�²���,û�зֺž�һֱ����ȥ��ƴ�ӳ�һ���ַ���

void errordisplay(int x)
{
	if (x == -1)cout << "�﷨����too short" << endl;
	if (x == -2)cout << "�﷨����wrong type" << endl;
	if (x == -3)cout << "�﷨����not a legal user_id" << endl;
	if (x == -4)cout << "�﷨����set primary_key wrong" << endl;
	if (x == -5)cout << "�﷨����set references wrong" << endl;
	if (x == -6)cout << "�﷨����cannot prase references" << endl;
	if (x == -7)cout << "�﷨����no tablename or attrname" << endl;
	if (x == -8)cout << "�﷨����illegal format, should be tablename.attr" << endl;
	if (x == -9)cout << "�﷨����illegal tablename" << endl;
	if (x == -10)cout << "�﷨����illegal attrname" << endl;
	if (x == -11)cout << "�﷨����expression:cannot detecte operator" << endl;
	if (x == -12)cout << "�﷨����expression:illegal operator or no operator" << endl;
	if (x == -13)cout << "�﷨����expression:prase lef_attr failue" << endl;
	if (x == -14)cout << "�﷨����expression:prase rig_attr failue" << endl;

}
void upper_error(string str, int id) {
	if (id == 0) printf("��������table.attr\n");
	else if (id == 1) printf("���ܽ�����op���ʽ\n");
	else if (id == 2) printf("ɾ����䲻����\n");
	else if (id == 3) printf("������䲻����\n");

}

vector<string> SQLline;

bool f = 0;//�Ƿ��ǽ���ģʽ

bool parse(string str) {
	int len = str.length();
	string word;
	int beg = 0;
	for (int i = 0; i <= len; i++) {
		if (i == len || str[i] == ',' || (f == 0 && str[i] == ' ') || str[i] == ';') {
			while (beg<i&&str[beg] == ' ')beg++;
			string word = str.substr(beg, i - beg);
			beg = i + 1;
			if (word.size() != 0) {
				if (word != "&&")SQLline.push_back(word);
				if (word == "create_table") f = 1;
			}
			if (str[i] == ';')return 1;
		}
	}
	return 0;
}

bool work_with_select(int &beg, vector<string> &SQLline, RESULT &cc){
	vector<table_attr> ta_vec;
	vector<string> tn_vec;
	vector<string> cd_vec;
	int i = 0, j = 0, k = 0;
	int siz = SQLline.size();
	int newbeg = -1;
	if (beg >= siz || SQLline[beg] != "select"){
		printf("ȱ��select\n");
		return 0;
	}
	for (i = beg+1; i < siz; i++) {
		if (SQLline[i] == "from")break;
		table_attr cnt;	//����table.attr
		int errorkey = cnt.parse(SQLline[i]);
		if (errorkey != 1) {
			errordisplay(errorkey);
			upper_error(SQLline[i], 0);
			return 0;
		}
		else ta_vec.push_back(cnt);
	}
	for (j = i + 1; j < siz; j++) {
		if (SQLline[j] == "intersection" || SQLline[j] == "union"){
			newbeg = j;
			break;
		}
		if (SQLline[j] == "where")break;
		tn_vec.push_back(SQLline[j]);
	}
	if (newbeg==-1)
	for (k = j + 1; k < siz; k++) {
		if (SQLline[k] == "intersection" || SQLline[k] == "union"){
			newbeg = k;
			break;
		}
		cd_vec.push_back(SQLline[k]);
	}
	if (newbeg == -1)beg = siz;
	else beg = newbeg;
	if (beg>siz)beg = siz;
	error mid_error = MID_OP::mid_query_outer(ta_vec, tn_vec, cd_vec,cc);
	if (mid_error.error_type == 10086){ return 1; }
	return 0;
}

void Analysis() {

	//���ڷִ��������
	
	//cout << "out:" << endl;
	//for (auto str : SQLline) {
	//	cout << str << endl;
	//}
	
	//Ŀǰ����ִʲ��Ժ���˳���
	//��Ҫ��ÿһ����֧���в���
	//	return;

	int siz = SQLline.size();
	if (SQLline[0] == "create_table") {
		/*
		create_table talbe1
		cc double,
		sno double primary_key,
		cno char references table2.cno,
		cno char primary_key references table2.cno;
		*/
		if (siz < 3) {//���ٰ���keyword tablename attrname type
			//�������ṹ������������
			cout << "���㹻��Ϣ����" << endl;
			return;
		}
		vector<Base_attribute_sega> Ba_vec;
		string tn = SQLline[1];
		for (int i = 2; i < siz; i++) {
			Base_attribute_sega cnt;
			int errorkey = cnt.parse(SQLline[i]);
			if (errorkey != 1) {
				errordisplay(errorkey);
				upper_error(SQLline[1], 100);//��������
				return;
			}
			else Ba_vec.push_back(cnt);
		}
		error mid_error = MID_OP::mid_create_table(tn, Ba_vec);
		string error_content = mid_error.geterror();
		if (error_content != "")cout << error_content << endl;
		return;
	}
	else if (SQLline[0] == "drop_table") {
		//drop_table table1;
		if (siz != 2) {
			if (siz > 2)
			{
				cout << "�﷨����drop table too long" << endl;
			}
			else
			{
				cout << "�﷨����:drop table too short" << endl;
			}

			return;
		}
		error mid_error = MID_OP::mid_drop_table(SQLline[1]);
		string error_content = mid_error.geterror();
		if (error_content != "")cout << error_content << endl;
		return;
	}
	else if (SQLline[0] == "create_index") {
		//create_index table.name;
		if (siz != 2) {
			if (siz > 2)
			{
				cout << "�﷨����create_index too long" << endl;
			}
			else
			{
				cout << "�﷨����:create_index too short" << endl;
			}
			return;
		}
		table_attr cnt;
		if (!cnt.parse(SQLline[1])) {
			upper_error(SQLline[1], 0);
			return;
		}
		error mid_error = MID_OP::mid_create_index(cnt);
		string error_content = mid_error.geterror();
		if (error_content != "")cout << error_content << endl;
		return;
	}
	else if (SQLline[0] == "drop_index") {
		//drop_index table.attr;
		if (siz != 2) {
			if (siz > 2)
			{
				cout << "�﷨����drop_index too long" << endl;
			}
			else
			{
				cout << "�﷨����:drop_index too short" << endl;
			}
			return;
		}
		table_attr cnt;
		if (!cnt.parse(SQLline[1])) {
			upper_error(SQLline[1], 0);
			return;
		}
		error mid_error = MID_OP::mid_drop_index(cnt);
		string error_content = mid_error.geterror();
		if (error_content != "")cout << error_content << endl;
		return;
	}
	else if (SQLline[0] == "select") {
		/*
		select table1.attr1,table2.attr2
		from table1,table2
		where table1.attr1=table2.attr2,table1.attr1=111,table2.attr2="dwa";
		select *
		from table1,table2
		where table1.attr1=table2.attr2,table1.attr1=111,table2.attr2="dwa";
		*/
		RESULT finalret,cc;
		bool f = 0;
		int beg = 0;
		int type=-1;
		bool exist_error=0;
		while(1){
			int rc = work_with_select(beg, SQLline, cc);
			if (rc == 0){ exist_error = 1; break; }
			if (f == 0){ f = 1; finalret = cc; }
			else{
				//if (cc.attr_vec != finalret.attr_vec){
				//	printf("union ���ڴ���\n");
				//	exist_error = 1;
				//	break;
				//}
				if (type == 1){//union
					bool f=1;
					finalret = MID_OP::brute_force_union(finalret, cc,f);
					if (f == 0) {
						exist_error = 1;
						break;
					}
				}
				else if (type==2){//intersection
					bool f=1;
					finalret = MID_OP::brute_force_intersect(finalret,cc,f);
					if (f == 0) {
						exist_error = 1;
						break;
					}
				}
			}
			if (beg == SQLline.size())break;
			if (SQLline[beg] == "union")type = 1;
			else if(SQLline[beg]=="intersection") type = 2;
			beg++;
		}
		if (!exist_error){
			finalret.print();
			printf("һ����ѯ����%d������\n", finalret.data_vec.size());
		}
		return;
	}
	else if (SQLline[0] == "delete") {
		//cout << siz << endl;
		/*
		delete from table1
		where table1.attr1=table2.attr2,table1.attr1=111,table2.attr2="dwa";
		*/
		vector<string> tn_vec;
		vector<expression> cd_vec;
		if (siz == 1) { upper_error(SQLline[0], 2); return; }
		if (siz < 2 || SQLline[1] != "from") {
			upper_error(SQLline[1], 2);//ɾ����䲻����,û��from
			return;
		}
		int i, j;
		//cout << "size:" << siz << endl;
		for (i = 2; i < siz; i++) {
			//cout<<SQLline[i]<<endl;
			if (SQLline[i] == "where")break;
			tn_vec.push_back(SQLline[i]);
		}

		for (j = i + 1; j < siz; j++) {
			expression cnt;
			int errorkey = cnt.parse(SQLline[j]);
			if (errorkey != 1) {
				errordisplay(errorkey);
				upper_error(SQLline[j], 1);
				return;
			}
			else cd_vec.push_back(cnt);
		}

		error mid_error = MID_OP::mid_delete_data(tn_vec, cd_vec);
		string error_content = mid_error.geterror();
		if (error_content != "")cout << error_content << endl;
		return;

	}
	else if (SQLline[0] == "update") {
		/*
		update talbe1
		set table1.sno=111,table1.sc="dwa"
		where table1.sno=111,table1.age<=5;
		*/
		vector<string> tn_vec;
		vector<expression> exp_vec;
		vector<expression> cd_vec;
		int i, j, k;

		for (i = 1; i < siz; i++) {
			if (SQLline[i] == "set")break;
			tn_vec.push_back(SQLline[i]);
		}

		for (j = i + 1; j < siz; j++) {
			if (SQLline[j] == "where")break;
			expression cnt;
			int errorkey = cnt.parse(SQLline[j]);
			if (errorkey != 1) {
				errordisplay(errorkey);
				upper_error(SQLline[j], 1);
				return;
			}
			else exp_vec.push_back(cnt);
		}

		for (k = j + 1; k < siz; k++) {
			expression cnt;
			int errorkey2 = cnt.parse(SQLline[k]);
			if (errorkey2 != 1) {
				errordisplay(errorkey2);
				upper_error(SQLline[k], 1);
				return;
			}
			else cd_vec.push_back(cnt);
		}

		error mid_error = MID_OP::mid_update_data(tn_vec, exp_vec, cd_vec);
		string error_content = mid_error.geterror();
		if (error_content != "")cout << error_content << endl;
		return;

	}
	else if (SQLline[0] == "insert") {
		/*
		insert into table1
		value table1.sno=111,table1.sc="dwa";
		*/
		vector<string> tn_vec;
		vector<expression> cd_vec;
		if (siz == 1) { upper_error(SQLline[0], 3); return; }
		if (siz < 2 || SQLline[1] != "into") {
			upper_error(SQLline[1], 3);//������䲻����,û��from
			return;
		}
		int i = 0, j = 0;
		int valuecode = 0;
		for (i = 2; i < siz; i++) {
			if (SQLline[i] == "value")
			{
				valuecode = 1; break;
			}
			tn_vec.push_back(SQLline[i]);
		}
		if (valuecode != 1){ cout << "ȱʧ�ؼ���: value" << endl; return; }

		for (j = i + 1; j < siz; j++) {
			expression cnt;
			int errorkey = cnt.parse(SQLline[j]);
			if (errorkey != 1) {
				errordisplay(errorkey);
				upper_error(SQLline[j], 1);
				return;
			}
			else cd_vec.push_back(cnt);
		}

		error mid_error = MID_OP::mid_insert(tn_vec, cd_vec);
		string error_content = mid_error.geterror();
		if (error_content != "")cout << error_content << endl;
		return;

	}
	else if(SQLline[0]=="show_table"){
		if (SQLline.size() != 2){
			printf("�����ֵ��ѯ�ı����������Ϸ�\n");
		}
		else{
			MID_OP::mid_query_data_dic(SQLline[1]);
		}
	}
	else printf("�޷�ʶ��Ĺؼ���\n");
	return;
}

void helpdisplay()
{
	cout << "DDL Grammer:" << endl;
	//create_table
	{
		cout << "-----------create_table---------------" << endl;
		cout << "create_table talbename" << endl;
		cout << "attrname type," << endl;
		cout << "attrname2 type primary_key," << endl;
		cout << "\ntype=char||double" << endl;
	}
	//drop_table
	{
		cout << endl;
		cout << "-----------drop_table---------------" << endl;
		cout << "drop_table talbename" << endl;
	}
	//create_index
	{
		cout << endl;
		cout << "-----------create_index---------------" << endl;
		cout << "create_index talbename.attrname" << endl;

	}
	//drop_index
	{
		cout << endl;
		cout << "-----------drop_index---------------" << endl;
		cout << "drop_index talbename.attrname" << endl;

	}
	cout << endl;
	cout << "DML Grammer:" << endl;
	//select
	{
		cout << "-----------select---------------" << endl;
		cout << "select table1.attr1,table2.attr2" << endl;
		cout << "from table1, table2" << endl;
		cout << "where table1.attr1 = table2.attr2, table1.attr1 = 111, table2.attr2 = \"dwa\";" << endl;
		cout << "Or:" << endl;
		cout << "select *" << endl;
		cout << "from table1, table2" << endl;
		cout << "where table1.attr1 = table2.attr2, table1.attr1 = 111, table2.attr2 = \"dwa\";" << endl;
	}
	//delete
	{
		cout << endl;
		cout << "-----------delete---------------" << endl;
		cout << "delete from table1" << endl;
		cout << "where table1.attr1 = table2.attr2, table1.attr1 = 111, table2.attr2 = \"dwa\";" << endl;
	}
	//update
	{
		cout << endl;
		cout << "-----------update---------------" << endl;
		cout << "update talbe1" << endl;
		cout << "set table1.sno = 111, table1.sc = \"dwa\"" << endl;
		cout << "where table1.sno = 111, table1.age <= 5;" << endl;
	}
	//insert
	{
		cout << endl;
		cout << "-----------insert---------------" << endl;
		cout << "insert into table1" << endl;
		cout << "value table1.sno = 111, table1.sc = \"dwa\";" << endl;
	}

	{
		cout << endl;
		cout << "operator list: >, <, =, >=, <=, <>" << endl;
	}

}

int main() {


	//testing=============
	//expression cc;
	//cc.parse("table.name=5.545");
	//===================

	_mkdir("table");

	//cout << "Sega Statement parsing version 1.0" << endl << endl;
	cout << "Our Small Database Community Edition Release 11.2.2 - Production" << endl;
	string line;
	bool begin = 1;
	while (1) {
		if (begin)cout << ">>>";
		else cout << "   ";
		getline(cin, line);
		transform(line.begin(), line.end(), line.begin(), ::tolower);
		if (begin &&line == "quit") { exit(0); }//����ʲô�����ֻҪ��һ���ǵ�����quit,���˳�
		if (begin &&line == "help") { helpdisplay(); continue; }
		begin = 0;
		if (parse(line)) {//�õ������������,��ʼ��������
			Analysis();
			SQLline.clear();
			f = 0;
			begin = 1;
		}
	}
	return 0;
}

