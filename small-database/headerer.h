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

#define FILE_FIRST_LENGTH 4	//���ڼ�¼�ļ�����ʾ��¼�ļ�ͷ��������ĸ�����磨file��
#define FILE_LAST_LENGTH 4	//���ڼ�¼�ļ�����ʾ��¼�ļ�β��������ĸ�����磨.dat��
#define BPTREE_MAX_FILE_PATH_SIZE 1000//�ļ��������
#define MAX_CHAR_KEY_SIZE 100	//b+����for neochar������Ĺؼ��ֳ���
#define MAX_KEY 15			//B+���Ľ�,����Ϊ����3����

typedef int Base_attribute;
typedef	int FILEP;




//�����ѯ��������ݽṹ
struct TSearchResult {
	bool exist;
	bool ismul;      //�ж�Raddress�ǵ�����¼��ַ���Ƕ����¼��ַ������
	FILEP Baddress;	 //���������Taddress��B+���ڵ��ַ
	FILEP Raddress;	 //�ùؼ��ֵ���ָ��ĵ�ַ

};




#endif // HEADERER_H