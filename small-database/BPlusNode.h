#ifndef BPLUSNODE_H
#define BPLUSNODE_H

#include "headerer.h"

typedef double KEYTYPE;

//插入关键字的数据结构
struct TRecord { //先有这个TRecord再有BPlusnode
	KEYTYPE	key;			  //该记录的关键字
	FILEP Raddress;		     //该关键字对应记录的地址

};


//B+树节点的数据结构

class BPlusNode {
public:
	KEYTYPE	keys[MAX_KEY];		            //关键字域
	pair<bool, FILEP> pointers[MAX_KEY + 1];	//指针域
	int nkey;				                //关键字数
	bool isleaf;				            //是否为叶节点 叶节点:true 否则为false

	BPlusNode();
	~BPlusNode();
};

#endif