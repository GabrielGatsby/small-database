#pragma once
#ifndef INF_H
#define INF_H

#include "BPlusTree.h"
#include "BPlusTreeC.h"
#include "record.h"
#include "file_handle.h"

//关于table


//关于index
bool something_exist(char* path);//查找文件夹或文件是否存在
void get_all_something_under(char* path, vector<string>& something);//查找文件夹下方的文件或文件夹
FILEP get_raddress_from_filename(string fimename);
void create_index_inf1(string tablename, string keyname);//创建索引（bptree）(请先判断table是否存在,请先判断index是否存在)
void delete_index_inf1(string tablename, string keyname);//删除索引（bptree) (千万不能删除主键！！！！！！请先从数据字典确认。)

//关于元组查询
void read_inid(string tablename, string keyname, TSearchResult tresult, vector<FILEP> &points_to);
void from_result_add_to_points(string tablename, string keyname, TSearchResult tresult, vector<FILEP> &points_to);
void from_results_add_to_points(string tablename, string keyname, vector<TSearchResult> &results, vector<FILEP> &points_to);

bool search_tuples_exist_inf1(string tablename, string keyname, string key);//判断当前关键字的元组是否存在   （仅适用于有bptree）
bool search_tuples_exist_inf1(string tablename, string keyname, double key);//判断当前关键字的元组是否存在   （仅适用于有bptree）
bool search_tuples_inf1(string tablename, string keyname, string key, TSearchResult &tresult);
bool search_tuples_inf1(string tablename, string keyname, double key, TSearchResult &tresult);
bool search_tuples_inf2(string tablename, string keyname, string key, vector<FILEP> &points_to);//查找当前关键字元组（返回vector<FILEP>)（仅适用于有bptree）
bool search_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to);//查找当前关键字元组（返回vector<FILEP>)（仅适用于有bptree）
bool search_tuples_not_equal_inf1(string tablename, string keyname, string key, vector<TSearchResult> &results);
bool search_tuples_not_equal_inf2(string tablename, string keyname, string key, vector<FILEP> &points_to);
bool search_tuples_not_equal_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results);
bool search_tuples_not_equal_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to);

bool seatch_tuples_all_inf1(string tablename, string keyname, int type, vector<TSearchResult> &tresult);
bool search_tuples_all_inf2(string tablename, string keyname, int type, vector<FILEP> &points_to);


void search_smaller_tuples_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results);
void search_bigger_tuples_inf1(string tablename, string keyname, double key, vector<TSearchResult> &results);
void search_bigger_and_smaller_tuples_inf1(string tablename, string keyname, double key1, double key2, vector<TSearchResult> &results);
void search_smaller_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to);//查找小于该关键字的元组(只适用于关键字为double）（返回vector<FILEP>）	（仅适用于有bptree）
void search_bigger_tuples_inf2(string tablename, string keyname, double key, vector<FILEP> &points_to);//查找大于该关键字的元组(只适用于关键字为double）（返回vector<FILEP>）		（仅适用于有bptree）
void search_bigger_and_smaller_tuples_inf2(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to);//查找大于key1小于key2的元组(只适用于关键字为double）（返回vector<FILEP>）（仅适用于有bptree）

void insert_tuple_inf1(string tablename, FILEP address);//插入（请先插入记录，得到记录地址后，再插入索引）（这个是给bptree用的）

void delete_tuple_inf1(string tablename, FILEP address);
void delete_tuples_inf1(string tablename, vector<FILEP> &points_to);//删除（我先查找记录，得到记录地址后，再删除索引）（请删除索引后再删除文件）（这个是给bptree用的）

void change_tuple_inf1(string tablename, string keyname, string key_before, string key_now, FILEP address);//修改bptree中的关键字（比如年龄从18改到19）（只适用于一颗存在的bptree）
void change_tuple_inf1(string tablename, string keyname, double key_before, double key_now, FILEP address);//修改bptree中的关键字（比如年龄从18改到19）（只适用于一颗存在的bptree）
void change_tuples_inf1(string tablename, string keyname, string key_before, string key_now, vector<FILEP> &points_to);//修改bptree中的关键字（比如年龄从18改到19）（只适用于一颗存在的bptree）
void change_tuples_inf1(string tablename, string keyname, double key_before, double key_now, vector<FILEP> &points_to);//修改bptree中的关键字（比如年龄从18改到19）（只适用于一颗存在的bptree）

FILEP insert_tuple_(string tablename, Record record);//插入元组


//
bool search_tuples_exist_noindex(string tablename, string keyname, string key);//在无索引的情况下，查找指定关键字的记录是否存在
bool search_tuples_exist_noindex(string tablename, string keyname, double key);//在无索引的情况下，查找指定关键字的记录是否存在

bool search_tuples_noindex(string tablename, string keyname, string key, vector<FILEP> &points_to);//在无索引的情况下，得到指定关键字的记录地址，存储在vector<FILEP> &points_to中
bool search_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to);//在无索引的情况下，得到指定关键字的记录地址，存储在vector<FILEP> &points_to中

void search_smaller_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to);//在无索引的情况下，得到关键字小于key的记录地址，存储在vector<FILEP> &points_to中）//比如keyname = 年龄， key = 18，则得到年龄小于18的记录的地址，存储
void search_bigger_tuples_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to);//在无索引的情况下，得到关键字大于key的记录地址，存储在vector<FILEP> &points_to中）
void search_bigger_and_smaller_tuples_noindex(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to);//在无索引的情况下，得到关键字大于key1，小于key2的记录地址，存储在vector<FILEP> &points_to中）

void delete_tuples_from_dicks(string tablename, vector<FILEP> &points_to);//删除指定地址的记录。注意，是多个记录。

void change_tuples_from_dicks(string tablename, string keyname, string key_before, string key_now, vector<FILEP> &points_to);//更新指定地址的记录。注意，是多个记录。
void change_tuples_from_dicks(string tablename, string keyname, double key_before, double key_now, vector<FILEP> &points_to);//更新指定地址的记录。注意，是多个记录。
void change_tuple_from_disk(string tablename, string keyname, string key_before, string key_now, FILEP &points_to);
void change_tuple_from_disk(string tablename, string keyname, double key_before, double key_now, FILEP &points_to);

bool search_tuples_not_equal_noindex(string tablename, string keyname, double key, vector<FILEP> &points_to);	//查找不等于当前关键字元组,返回vector<FILEP> points_to（bool == 1时表示元组存在）
bool search_tuples_not_equal_noindex(string tablename, string keyname, string key, vector<FILEP> &points_to);	//查找不等于当前关键字元组,返回vector<FILEP> points_to（bool == 1时表示元组存在）

//void change_tuples_inf(string tablename, string keyname, string key_before, string key_now, vector<FILEP> points_to);	//更改points_to所指向的记录 //弃用
//void change_tuples_inf(string tablename, string keyname, double key_before, double key_now, vector<FILEP> points_to);	//更改points_to所指向的记录	//弃用

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*关于table*/
void create_table_inf(string tablename, string btree_pk, vector<string>col_nam, vector<int>typ, vector<int>siz);	//创建表
void give_you_all_tables_inf(vector<string>& tables);																//传递所有表名，返回vector<string> tables
bool search_table_exist_inf(string tablename);																		//查找表是否存在 （bool == 1时表示存在）
void drop_table_inf(string tablename);																				//删除表


/*关于属性*/
string get_main_key(string tablename);
vector<string> get_all_attr(string tablename);
int exist_attr_in_table(string tablename, string attr_name);					//当存在查找属性时，返回属性类型（1为double，2为varchar）；当不存在查找属性时，返回-1

/*关于index*/
bool search_index_exist_inf(string tablename, string keyname);					//查询指定索引（bptree）是否存在（bool == 1时表示存在）
void search_all_indexs_inf(string tablename, vector<string>& all_indexs);		//查询存在的索引（bptree）。查询返回vector<string> all_indexs。应该用不上
bool create_index_inf(string tablename, string keyname);						//创建索引（bptree) (bool == 0时表示创建失败）
bool delete_index_inf(string tablename, string keyname);						//删除索引（bptree) (bool == 0时表示删除失败。可能原因：1.删除的关键字是主键，主键是无法删除的。2.无此索引）  

/*关于元组*/
void read_table_meta_inf(string tablename, table &table_meta);									//读取数据字典，返回在table_meta中
void read_record_inf(string tablename, FILEP point_to, Record &record);							//读取记录，返回在record中
void read_records_inf(string tablename, vector<FILEP> points_to, vector<Record> &records);		//读取多个记录，返回在records中

bool search_tuples_exist_inf(string tablename, string keyname, string key);							//判断当前关键字的元组是否存在 （bool == 1时表示元组存在）
bool search_tuples_exist_inf(string tablename, string keyname, double key);							//判断当前关键字的元组是否存在 （bool == 1时表示元组存在）

bool search_tuples_inf(string tablename, string keyname, string key, vector<FILEP> &points_to);		//查找当前关键字元组,返回vector<FILEP> points_to（bool == 1时表示元组存在）
bool search_tuples_inf(string tablename, string keyname, double key, vector<FILEP> &points_to);		//查找当前关键字元组,返回vector<FILEP> points_to（bool == 1时表示元组存在）
bool search_tuples_not_equal_inf(string tablename, string keyname, double key, vector<FILEP> &points_to);	//查找不等于当前关键字元组,返回vector<FILEP> points_to（bool == 1时表示元组存在）
bool search_tuples_not_equal_inf(string tablename, string keyname, string key, vector<FILEP> &points_to);	//查找不等于当前关键字元组,返回vector<FILEP> points_to（bool == 1时表示元组存在）
bool search_tuples_all_inf(string tablename,vector<FILEP> &points_to);		//查找所有元组(使用主键查找），返回vector<FILEP> points_to（bool == 1时表示元组存在）


void search_smaller_tuples_inf(string tablename, string keyname, double key, vector<FILEP> &points_to);							//查找小于该关键字的元组,返回vector<FILEP> points_to
void search_bigger_tuples_inf(string tablename, string keyname, double key, vector<FILEP> &points_to);							//查找大于该关键字的元组,返回vector<FILEP> points_to
void search_bigger_and_smaller_tuples_inf(string tablename, string keyname, double key1, double key2, vector<FILEP> &points_to);//查找大于key1小于key2的元组,返回vector<FILEP> points_to


void delete_tuples_inf(string tablename, vector<FILEP> &points_to);		//删除vector<FILEP> points_to所指向的记录

bool insert_tuple_inf(string tablename, Record &record);				//插入record (bool == 0 时表示插入失败，因为插入主码重复）


void change_tuple_inf(string tablename, string keyname, string key_before, string key_now, FILEP point_to);		//更改point_to所指向的记录
void change_tuple_inf(string tablename, string keyname, double key_before, double key_now, FILEP point_to);		//更改point_to所指向的记录

#endif