#include "neochar.h"

neochar::neochar()
{
}

neochar::neochar(string str)
{
	strcpy(ch, str.c_str());
}

neochar::neochar(char str[])
{
	strcpy(ch, str);
}


neochar::~neochar()
{
}

char & neochar::operator[](int i)
{
	if (i >= 0 && i <= MAX_CHAR_KEY_SIZE)
		return ch[i];
}

neochar & neochar::operator=(char str[])
{
	strcpy(this->ch, str);
	return *this;
}

neochar & neochar::operator=(const char str[])
{
	strcpy(this->ch, str);
	return *this;
}


bool operator<(neochar & c1, neochar & c2)
{
	if(strcmp(c1.ch, c2.ch) == -1)
		return true;
	return false;
}

bool operator==(neochar & c1, neochar & c2)
{
	if (strcmp(c1.ch, c2.ch) == 0)
		return true;
	return false;
}

bool operator>(neochar & c1, neochar & c2)
{
	if (strcmp(c1.ch, c2.ch) == 1)
		return true;
	return false;
}

bool operator<=(neochar & c1, neochar & c2)
{
	if ((strcmp(c1.ch, c2.ch) == -1) || (strcmp(c1.ch, c2.ch) == 0))
		return true;
	return false;
}

bool operator>=(neochar & c1, neochar & c2)
{
	if ((strcmp(c1.ch, c2.ch) == 1) || (strcmp(c1.ch, c2.ch) == 0))
		return true;
	return false;
}
