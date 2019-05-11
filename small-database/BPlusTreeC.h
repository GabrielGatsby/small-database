#ifndef BPLUSTREEC_H
#define BPLUSTREEC_H

#include "BPlusNodeC.h"

class BPlusTreeC {
private:
	FILEP NID;				//用来赋予结点id
	FILEP INDID;			//用来赋予记录地址索引id
	FILEP ROOT;				//根节点地址
	FILE	*Bfile;			//B+树文件的指针
	FILE	*Rfile;			//记录文件的指针
	string tablename;		//table名
	string keyname;			//关键字名

public:
	FILEP get_root();

	void Build_BPlus_Tree(string tname, string kname);			//创建一颗b+树。通常用于测试
	void ReadBPlusTree(string tname, string kname);				//读取b+树（如果没有的话直接创建b+树）

	void ReadBPlusNode(FILEP addresss, BPlusNodeC &n);			//读取b+树结点
	void WriteBPlusNode(FILEP address, BPlusNodeC &n);			//写入b+树结点
	void DeleteBplusNode(FILEP address);						//删除b+树结点

	void GetTheOnlyRaddressIndex(FILEP INDID_ADDRESS, TRecordC& trecord); //得到那唯一的地址。此函数比较特殊，千万不要碰
	void WriteRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress);		//写入多个记录的索引
	void DeleteRaddressIndex(FILEP INDID_ADDRESS);
	int DeleteSingleRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress); //删除索引中的单个记录

	void insert_bplus_tree(FILEP address, TRecordC &trecord);		//插入辅助函数
	void Insert_BPlus_Tree(TRecordC &trecord);					//把记录地址通过关键字插入b+树


	void Split_BPlus_Node(BPlusNodeC &father, BPlusNodeC &current, int childnum);	//分裂b+树结点

	void Search_BPlus_Tree(TRecordC &trecord, TSearchResult &tresult);				//查询bp树并返回一个结果

	void delete_BPlus_tree(FILEP address, TRecordC &trecord);		//删除辅助函数
	void Delete_BPlus_Tree(TRecordC &trecord);					//把相关关键字的记录地址从bp树上删除

	void Delete_BPlus_Tree_Address_Under_Key(TRecordC &trecord); //删除指定关键字下的指定记录地址

	void EnumLeafKey();											//顺序列举所有bp树叶节点关键字。测试用

	void update_mete_tree();									//更新bp树


	BPlusTreeC();
	~BPlusTreeC();

};

#endif