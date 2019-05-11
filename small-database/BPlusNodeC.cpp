#include "BPlusNodeC.h"

BPlusNodeC::BPlusNodeC() {
	for (int i = 0; i < MAX_KEY + 1; i++)
		pointers[i].first = false;
}


BPlusNodeC::~BPlusNodeC()
{
}
