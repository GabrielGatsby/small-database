#include "BPlusNode.h"

BPlusNode::BPlusNode() {
	for (int i = 0; i < MAX_KEY + 1; i++)
		pointers[i].first = false;
}


BPlusNode::~BPlusNode()
{
}
