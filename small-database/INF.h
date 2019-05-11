#pragma once
#ifndef INF_H
#define INF_H

#include "BPlusTree.h"
#include "BPlusTreeC.h"
#include "record.h"
#include "file_handle.h"

//����table


//����index
bool something_exist(char* path);//�����ļ��л��ļ��Ƿ����
void get_all_something_under(char* path, vector<string>& something);//�����ļ����·����ļ����ļ���
FILEP get_raddress_from_filename(string fimename);
void create_index_inf1(string tablename, string keyname);//����������bptree��(�����ж�table�Ƿ����,�����ж�index�Ƿ����)
void delete_index_inf1(string tablename, string keyname);//ɾ��������bptree) (ǧ����ɾ���������������������ȴ������ֵ�ȷ�ϡ�)

//����Ԫ���ѯ
void read_inid(string tablename, string keyname, TSearchResult tresult, vector<FILEP> &points_to);
void from_result_add_to_points(string tablename, string keyname, TSearchResult tresult, vector<FILEP> &points_to);
void from_results_add_to_points(string tablename, string keyname, vector<TSearchResult> &results, vector<FILEP> &points_to);

bool search_tuples_exist_inf1(string tablename, string keyname, string key);//�жϵ�ǰ�ؼ��ֵ�Ԫ���Ƿ����   ������������bptree��
bool search_tuples_exist_inf1(string tablename, string keyname, double key);//�жϵ�ǰ�ؼ��ֵ�Ԫ���Ƿ����   ������������bptree��
bool search_tuples_inf1(string tablename, string keyname, string key, TSearchResult &tresult);
bool search_tuples_inf1(string tablename, string keyname, double key, TSearchResult &tresult);
bool search_tuples_inf2(string tablename, string keyname, string key, vector<FILEP> &points_to);//���ҵ�ǰ�ؼ���Ԫ�飨����vector<FILEP>)������������bptree��
bool search_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to);//���ҵ�ǰ�ؼ���Ԫ�飨����vector<FILEP>)������������bptree��
bool search_tuples_not_equal_inf1(string tablename, string keyname, string key, vector<TSearchResult> &results);
bool search_tuples_not_equal_inf2(string tablename, string keyname, string key, vector<FILEP> &points_to);
bool search_tuples_not_equal_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results);
bool search_tuples_not_equal_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to);

bool seatch_tuples_all_inf1(string tablename, string keyname, int type, vector<TSearchResult> &tresult);
bool search_tuples_all_inf2(string tablename, string keyname, int type, vector<FILEP> &points_to);


void search_smaller_tuples_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results);
void search_bigger_tuples_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results);
void search_bigger_and_smaller_tuples_inf1(string tablename, string keyname, double key1, double key2, vector<TSearchResult> &results);
void search_smaller_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to);//����С�ڸùؼ��ֵ�Ԫ��(ֻ�����ڹؼ���Ϊdouble��������vector<FILEP>��	������������bptree��
void search_bigger_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to);//���Ҵ��ڸùؼ��ֵ�Ԫ��(ֻ�����ڹؼ���Ϊdouble��������vector<FILEP>��		������������bptree��
void search_bigger_and_smaller_tuples_inf2(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to);//���Ҵ���key1С��key2��Ԫ��(ֻ�����ڹؼ���Ϊdouble��������vector<FILEP>��������������bptree��

void insert_tuple_inf1(string tablename, FILEP address);//���루���Ȳ����¼���õ���¼��ַ���ٲ���������������Ǹ�bptree�õģ�

void delete_tuple_inf1(string tablename, FILEP address);
void delete_tuples_inf1(string tablename, vector<FILEP> &points_to);//ɾ�������Ȳ��Ҽ�¼���õ���¼��ַ����ɾ������������ɾ����������ɾ���ļ���������Ǹ�bptree�õģ�

void change_tuple_inf1(string tablename, string keyname, string key_before, string key_now, FILEP address);//�޸�bptree�еĹؼ��֣����������18�ĵ�19����ֻ������һ�Ŵ��ڵ�bptree��
void change_tuple_inf1(string tablename, string keyname, double key_before, double key_now, FILEP address);//�޸�bptree�еĹؼ��֣����������18�ĵ�19����ֻ������һ�Ŵ��ڵ�bptree��
void change_tuples_inf1(string tablename, string keyname, string key_before, string key_now, vector<FILEP> &points_to);//�޸�bptree�еĹؼ��֣����������18�ĵ�19����ֻ������һ�Ŵ��ڵ�bptree��
void change_tuples_inf1(string tablename, string keyname, double key_before, double key_now, vector<FILEP> &points_to);//�޸�bptree�еĹؼ��֣����������18�ĵ�19����ֻ������һ�Ŵ��ڵ�bptree��

FILEP insert_tuple_(string tablename, Record record);//����Ԫ��


//
bool search_tuples_exist_noindex(string tablename, string keyname, string key);//��������������£�����ָ���ؼ��ֵļ�¼�Ƿ����
bool search_tuples_exist_noindex(string tablename, string keyname, double key);//��������������£�����ָ���ؼ��ֵļ�¼�Ƿ����

bool search_tuples_noindex(string tablename, string keyname, string key, vector<FILEP> &points_to);//��������������£��õ�ָ���ؼ��ֵļ�¼��ַ���洢��vector<FILEP> &points_to��
bool search_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to);//��������������£��õ�ָ���ؼ��ֵļ�¼��ַ���洢��vector<FILEP> &points_to��

void search_smaller_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to);//��������������£��õ��ؼ���С��key�ļ�¼��ַ���洢��vector<FILEP> &points_to�У�//����keyname = ���䣬 key = 18����õ�����С��18�ļ�¼�ĵ�ַ���洢
void search_bigger_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to);//��������������£��õ��ؼ��ִ���key�ļ�¼��ַ���洢��vector<FILEP> &points_to�У�
void search_bigger_and_smaller_tuples_noindex(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to);//��������������£��õ��ؼ��ִ���key1��С��key2�ļ�¼��ַ���洢��vector<FILEP> &points_to�У�

void delete_tuples_from_dicks(string tablename, vector<FILEP> &points_to);//ɾ��ָ����ַ�ļ�¼��ע�⣬�Ƕ����¼��

void change_tuples_from_dicks(string tablename, string keyname, string key_before, string key_now, vector<FILEP> &points_to);//����ָ����ַ�ļ�¼��ע�⣬�Ƕ����¼��
void change_tuples_from_dicks(string tablename, string keyname, double key_before, double key_now, vector<FILEP> &points_to);//����ָ����ַ�ļ�¼��ע�⣬�Ƕ����¼��
void change_tuple_from_disk(string tablename, string keyname, string key_before, string key_now, FILEP &points_to);
void change_tuple_from_disk(string tablename, string keyname, double key_before, double key_now, FILEP &points_to);

bool search_tuples_not_equal_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to);	//���Ҳ����ڵ�ǰ�ؼ���Ԫ��,����vector<FILEP> points_to��bool == 1ʱ��ʾԪ����ڣ�
bool search_tuples_not_equal_noindex(string tablename, string keyname, string key, vector<FILEP> &points_to);	//���Ҳ����ڵ�ǰ�ؼ���Ԫ��,����vector<FILEP> points_to��bool == 1ʱ��ʾԪ����ڣ�

//void change_tuples_inf(string tablename, string keyname, string key_before, string key_now, vector<FILEP> points_to);	//����points_to��ָ��ļ�¼ //����
//void change_tuples_inf(string tablename, string keyname, double key_before, double key_now, vector<FILEP> points_to);	//����points_to��ָ��ļ�¼	//����

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*����table*/
void create_table_inf(string tablename, string btree_pk, vector<string>col_nam, vector<int>typ, vector<int>siz);	//������
void give_you_all_tables_inf(vector<string>& tables);																//�������б���������vector<string> tables
bool search_table_exist_inf(string tablename);																		//���ұ��Ƿ���� ��bool == 1ʱ��ʾ���ڣ�
void drop_table_inf(string tablename);																				//ɾ����


/*��������*/
string get_main_key(string tablename);
vector<string> get_all_attr(string tablename);
int exist_attr_in_table(string tablename, string attr_name);					//�����ڲ�������ʱ�������������ͣ�1Ϊdouble��2Ϊvarchar�����������ڲ�������ʱ������-1

/*����index*/
bool search_index_exist_inf(string tablename, string keyname);					//��ѯָ��������bptree���Ƿ���ڣ�bool == 1ʱ��ʾ���ڣ�
void search_all_indexs_inf(string tablename, vector<string>& all_indexs);		//��ѯ���ڵ�������bptree������ѯ����vector<string> all_indexs��Ӧ���ò���
bool create_index_inf(string tablename, string keyname);						//����������bptree) (bool == 0ʱ��ʾ����ʧ�ܣ�
bool delete_index_inf(string tablename, string keyname);						//ɾ��������bptree) (bool == 0ʱ��ʾɾ��ʧ�ܡ�����ԭ��1.ɾ���Ĺؼ������������������޷�ɾ���ġ�2.�޴�������  

/*����Ԫ��*/
void read_table_meta_inf(string tablename, table &table_meta);									//��ȡ�����ֵ䣬������table_meta��
void read_record_inf(string tablename, FILEP point_to, Record &record);							//��ȡ��¼��������record��
void read_records_inf(string tablename, vector<FILEP> points_to, vector<Record> &records);		//��ȡ�����¼��������records��

bool search_tuples_exist_inf(string tablename, string keyname, string key);							//�жϵ�ǰ�ؼ��ֵ�Ԫ���Ƿ���� ��bool == 1ʱ��ʾԪ����ڣ�
bool search_tuples_exist_inf(string tablename, string keyname, double key);							//�жϵ�ǰ�ؼ��ֵ�Ԫ���Ƿ���� ��bool == 1ʱ��ʾԪ����ڣ�

bool search_tuples_inf(string tablename, string keyname, string key, vector<FILEP> &points_to);		//���ҵ�ǰ�ؼ���Ԫ��,����vector<FILEP> points_to��bool == 1ʱ��ʾԪ����ڣ�
bool search_tuples_inf(string tablename, string keyname, double key, vector<FILEP> &points_to);		//���ҵ�ǰ�ؼ���Ԫ��,����vector<FILEP> points_to��bool == 1ʱ��ʾԪ����ڣ�
bool search_tuples_not_equal_inf(string tablename, string keyname, double key, vector<FILEP> &points_to);	//���Ҳ����ڵ�ǰ�ؼ���Ԫ��,����vector<FILEP> points_to��bool == 1ʱ��ʾԪ����ڣ�
bool search_tuples_not_equal_inf(string tablename, string keyname, string key, vector<FILEP> &points_to);	//���Ҳ����ڵ�ǰ�ؼ���Ԫ��,����vector<FILEP> points_to��bool == 1ʱ��ʾԪ����ڣ�
bool search_tuples_all_inf(string tablename,vector<FILEP> &points_to);		//��������Ԫ��(ʹ���������ң�������vector<FILEP> points_to��bool == 1ʱ��ʾԪ����ڣ�


void search_smaller_tuples_inf(string tablename, string keyname, double key, vector<FILEP> &points_to);							//����С�ڸùؼ��ֵ�Ԫ��,����vector<FILEP> points_to
void search_bigger_tuples_inf(string tablename, string keyname, double key, vector<FILEP> &points_to);							//���Ҵ��ڸùؼ��ֵ�Ԫ��,����vector<FILEP> points_to
void search_bigger_and_smaller_tuples_inf(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to);//���Ҵ���key1С��key2��Ԫ��,����vector<FILEP> points_to


void delete_tuples_inf(string tablename, vector<FILEP> &points_to);		//ɾ��vector<FILEP> points_to��ָ��ļ�¼

bool insert_tuple_inf(string tablename, Record &record);				//����record (bool == 0 ʱ��ʾ����ʧ�ܣ���Ϊ���������ظ���


void change_tuple_inf(string tablename, string keyname, string key_before, string key_now, FILEP point_to);		//����point_to��ָ��ļ�¼
void change_tuple_inf(string tablename, string keyname, double key_before, double key_now, FILEP point_to);		//����point_to��ָ��ļ�¼

#endif