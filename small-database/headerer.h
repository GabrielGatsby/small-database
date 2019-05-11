#ifndef HEADERER_H
#define HEADERER_H

#include <io.h>
#include <iomanip>
#include <stdlib.h>
#include <process.h>
#include <windows.h> 
#include <stdio.h>
#include <stack>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include<set>
#include <string>
#include <algorithm>
#include <io.h>
using namespace std;

#define FILE_FIRST_LENGTH 4	//用于记录文件，表示记录文件头几个是字母，比如（file）
#define FILE_LAST_LENGTH 4	//用于记录文件，表示记录文件尾几个是字母，比如（.dat）
#define BPTREE_MAX_FILE_PATH_SIZE 1000//文件名最长长度
#define MAX_CHAR_KEY_SIZE 100	//b+树（for neochar）中最长的关键字长度
#define MAX_KEY 15			//B+树的阶,必须为大于3奇数

typedef int Base_attribute;
typedef	int FILEP;




//保存查询结果的数据结构
struct TSearchResult {
	bool exist;
	bool ismul;      //判断Raddress是单个记录地址还是多个记录地址的索引
	FILEP Baddress;	 //保存包含该Taddress的B+树节点地址
	FILEP Raddress;	 //该关键字的所指向的地址

};




#endif // HEADERER_H