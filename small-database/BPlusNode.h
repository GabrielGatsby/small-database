#ifndef BPLUSNODE_H
#define BPLUSNODE_H

#include "headerer.h"

typedef double KEYTYPE;

//����ؼ��ֵ����ݽṹ
struct TRecord { //�������TRecord����BPlusnode
	KEYTYPE	key;			  //�ü�¼�Ĺؼ���
	FILEP Raddress;		     //�ùؼ��ֶ�Ӧ��¼�ĵ�ַ

};


//B+���ڵ�����ݽṹ

class BPlusNode {
public:
	KEYTYPE	keys[MAX_KEY];		            //�ؼ�����
	pair<bool, FILEP> pointers[MAX_KEY + 1];	//ָ����
	int nkey;				                //�ؼ�����
	bool isleaf;				            //�Ƿ�ΪҶ�ڵ� Ҷ�ڵ�:true ����Ϊfalse

	BPlusNode();
	~BPlusNode();
};

#endif