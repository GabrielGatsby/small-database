#ifndef BPLUSTREEC_H
#define BPLUSTREEC_H

#include "BPlusNodeC.h"

class BPlusTreeC {
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

	void ReadBPlusNode(FILEP addresss, BPlusNodeC &n);			//��ȡb+�����
	void WriteBPlusNode(FILEP address, BPlusNodeC &n);			//д��b+�����
	void DeleteBplusNode(FILEP address);						//ɾ��b+�����

	void GetTheOnlyRaddressIndex(FILEP INDID_ADDRESS, TRecordC& trecord); //�õ���Ψһ�ĵ�ַ���˺����Ƚ����⣬ǧ��Ҫ��
	void WriteRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress);		//д������¼������
	void DeleteRaddressIndex(FILEP INDID_ADDRESS);
	int DeleteSingleRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress); //ɾ�������еĵ�����¼

	void insert_bplus_tree(FILEP address, TRecordC &trecord);		//���븨������
	void Insert_BPlus_Tree(TRecordC &trecord);					//�Ѽ�¼��ַͨ���ؼ��ֲ���b+��


	void Split_BPlus_Node(BPlusNodeC &father, BPlusNodeC &current, int childnum);	//����b+�����

	void Search_BPlus_Tree(TRecordC &trecord, TSearchResult &tresult);				//��ѯbp��������һ�����

	void delete_BPlus_tree(FILEP address, TRecordC &trecord);		//ɾ����������
	void Delete_BPlus_Tree(TRecordC &trecord);					//����عؼ��ֵļ�¼��ַ��bp����ɾ��

	void Delete_BPlus_Tree_Address_Under_Key(TRecordC &trecord); //ɾ��ָ���ؼ����µ�ָ����¼��ַ

	void EnumLeafKey();											//˳���о�����bp��Ҷ�ڵ�ؼ��֡�������

	void update_mete_tree();									//����bp��


	BPlusTreeC();
	~BPlusTreeC();

};

#endif