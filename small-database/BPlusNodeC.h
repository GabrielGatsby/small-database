#ifndef BPLUSNODEC_H
#define BPLUSNODEC_H

#include "headerer.h"
#include "neochar.h"

typedef neochar KEYTYPE2;

//����ؼ��ֵ����ݽṹ2(for char)
struct TRecordC { //�������TRecord����BPlusnode
	KEYTYPE2	key;			  //�ü�¼�Ĺؼ���
	FILEP Raddress;		     //�ùؼ��ֶ�Ӧ��¼�ĵ�ַ

};


//B+���ڵ�����ݽṹ

class BPlusNodeC {
public:
	KEYTYPE2	keys[MAX_KEY];		            //�ؼ�����
	pair<bool, FILEP> pointers[MAX_KEY + 1];	//ָ����
	int nkey;				                //�ؼ�����
	bool isleaf;				            //�Ƿ�ΪҶ�ڵ� Ҷ�ڵ�:true ����Ϊfalse

	BPlusNodeC();
	~BPlusNodeC();
};

#endif