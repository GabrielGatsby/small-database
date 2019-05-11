#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "BPlusNode.h"

class BPlusTree {
private:
	FILEP NID;				//����������id
	FILEP INDID;			//���������¼��ַ����id
	FILEP ROOT;				//���ڵ��ַ
	FILE	*Bfile;			//B+���ļ���ָ��
	FILE	*Rfile;			//��¼�ļ���ָ��
	string tablename;		//table��
	string keyname;			//�ؼ�����

public:
	FILEP get_root();

	void Build_BPlus_Tree(string tname, string kname);			//����һ��b+����ͨ�����ڲ���
	void ReadBPlusTree(string tname, string kname);				//��ȡb+�������û�еĻ�ֱ�Ӵ���b+����

	void ReadBPlusNode(FILEP addresss, BPlusNode &n);			//��ȡb+�����
	void WriteBPlusNode(FILEP address, BPlusNode &n);			//д��b+�����
	void DeleteBplusNode(FILEP address);						//ɾ��b+�����
	
	void GetTheOnlyRaddressIndex(FILEP INDID_ADDRESS, TRecord& trecord); //�õ���Ψһ�ĵ�ַ���˺����Ƚ����⣬ǧ��Ҫ��
	void WriteRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress);		//д������¼������
	void DeleteRaddressIndex(FILEP INDID_ADDRESS);						//ɾ�������¼������
	int DeleteSingleRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress); //ɾ�������еĵ�����¼

	void insert_bplus_tree(FILEP address, TRecord &trecord);		//���븨������
	void Insert_BPlus_Tree(TRecord &trecord);					//�Ѽ�¼��ַͨ���ؼ��ֲ���b+��

	void Split_BPlus_Node(BPlusNode &father, BPlusNode &current, int childnum);	//����b+�����

	void Search_BPlus_Tree(TRecord &trecord, TSearchResult &tresult);				//��ѯbp��������һ�����

	void Search_BPlus_Tree_Bigger(TRecord & trecord, TSearchResult & tresult);		//��B+����ѯһ��С�ڵ��ڸò��ҹؼ��ֵĽ�������ڴ��ڵķ�Χ���ң�����ĺ����벻Ҫ�ã�

	void delete_BPlus_tree(FILEP address, TRecord &trecord);		//ɾ����������
	void Delete_BPlus_Tree(TRecord &trecord);					//����عؼ��ֵļ�¼��ַ��bp����ɾ��

	void Delete_BPlus_Tree_Address_Under_Key(TRecord &trecord); //ɾ��ָ���ؼ����µ�ָ����¼��ַ


	void EnumLeafKey();											//˳���о�����bp��Ҷ�ڵ�ؼ��֡�������

	void update_mete_tree();									//����bp��


	BPlusTree();
	~BPlusTree();

};

#endif