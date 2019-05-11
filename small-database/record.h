#pragma once

#include "headerer.h"
#include "file_handle.h"

class Record {
public:
	char table_name[50];
	int rec_addr;
	void * data[30];
	int count; // no of attributes

};

void read_record_my(string t_name, int rec_ad, Record &rec, table &tab);
void read_record_my_by_director_name(string t_name, string file_name, Record &rec, table &tab);