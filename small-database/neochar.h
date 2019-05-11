#ifndef NEOCHAR_H
#define NEOCHAR_H
#include "headerer.h"


class neochar
{
public:
	char ch[MAX_CHAR_KEY_SIZE];

	neochar();
	neochar(string str);
	neochar(char[]);
	~neochar();
	
	char & operator[](int i);
	neochar & operator = (char[]);
	neochar & operator = (const char[]);
	friend bool operator <  (neochar &c1, neochar &c2);
	friend bool operator == (neochar &c1, neochar &c2);
	friend bool operator >  (neochar &c1, neochar &c2);
	friend bool operator <= (neochar &c1, neochar &c2);
	friend bool operator >= (neochar &c1, neochar &c2);
};

#endif