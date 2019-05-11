#include "BPlusTreeC.h"

BPlusTreeC::BPlusTreeC() {
}

BPlusTreeC :: ~BPlusTreeC() {

}




void BPlusTreeC::ReadBPlusTree(string tname, string kname) {
 	tablename = tname;
	keyname = kname;
	char *str;
	str = (char*)malloc(sizeof(char)* BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\meta_tree.dat", tablename.c_str(), keyname.c_str());

	ifstream in_file(str, ios::in | ios::binary);//打开meta_tree.dat文件
	//如果meta_tree.dat文件不存在，则新建该表
	if (!in_file) {
		ofstream out_file(str, ios::out | ios::trunc | ios::binary);

		if (!out_file) {//写入失败
			printf("写入磁盘失败");
			abort();
		}

		NID = 0;
		INDID = 0;
		ROOT = 0;
		out_file << NID << " " << INDID << " " << ROOT;
		out_file.close();

		//开始时根节点为叶节点
		BPlusNodeC root;
		//根节点无后续结点
		root.pointers[MAX_KEY].second = 0;
		//开始根节点无关键字
		root.nkey = 0;
		//开始根节点是叶节点
		root.isleaf = true;
		//把根节点写入文件
		WriteBPlusNode(NID, root);
	}
	//如果meta_tree.dat文件存在,读入meta_tree.dat信息即可
	else {
		in_file >> NID >> INDID >> ROOT;
		in_file.close();
	}

	free(str);
}

void BPlusTreeC::update_mete_tree() {
	char *str;
	str = (char*)malloc(sizeof(char)* BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\meta_tree.dat", tablename.c_str(), keyname.c_str());
	ofstream out_file(str, ios::out | ios::trunc | ios::binary);
	free(str);
	//把此树的信息写入文件
	out_file << NID << " " << INDID << " " << ROOT;
	out_file.close();
}


FILEP BPlusTreeC::get_root()
{
	return ROOT;
}

//建立空b+树  作为测试使用
void	BPlusTreeC::Build_BPlus_Tree(string tname, string kname) {
	NID = 0;
	INDID = 0;
	ROOT = 0;
	tablename = tname;
	keyname = kname;
	BPlusNodeC n;
	n.pointers[MAX_KEY].second = 0;
	n.nkey = 0;
	n.isleaf = true;
	WriteBPlusNode(ROOT, n);
}


//向B+树插入关键字
void	BPlusTreeC::Insert_BPlus_Tree(TRecordC &trecord) {

	BPlusNodeC n;
	ReadBPlusNode(ROOT, n);

	//分裂根节点
	if (n.nkey == MAX_KEY)
	{
		BPlusNodeC newroot;
		newroot.nkey = 0;
		newroot.isleaf = false;
		newroot.pointers[0].second = ROOT;

		Split_BPlus_Node(newroot, n, 0);
		WriteBPlusNode(ROOT, n);

		NID++;
		ROOT = NID;

		WriteBPlusNode(ROOT, newroot);

	}
	insert_bplus_tree(ROOT, trecord);
}



void	BPlusTreeC::insert_bplus_tree(FILEP current, TRecordC &trecord) {
	BPlusNodeC x;
	ReadBPlusNode(current, x);

	int	i;
	for (i = 0; i < x.nkey && x.keys[i] < trecord.key; i++);

	if (i < x.nkey && x.isleaf && x.keys[i] == trecord.key) {	//在B+树叶节点找到了相同关键字

		//当前指针为指向多个记录地址的索引时
		if (x.pointers[i].first) {
			WriteRaddressIndex(x.pointers[i].second, trecord.Raddress);//直接更新多个记录地址的索引
		}

		//当前指针为指向单个记录地址时
		else if (!x.pointers[i].first) {
			INDID++;//索引文件加一
			x.pointers[i].first = true;										//代表已经指向多个记录地址的索引
			WriteRaddressIndex(INDID, x.pointers[i].second);	//将原来此处的记录地址加到索引中
			//???
			x.pointers[i].second = INDID;									//改变指针指向：从指向单一记录地址到指向多个记录地址的索引
			WriteBPlusNode(current, x);										//更新结点
			WriteRaddressIndex(INDID, trecord.Raddress);						//更新多个记录地址的索引
			//???
		}

		update_mete_tree();
		return;
	}

	if (!x.isleaf) {	//如果不是叶节点

		BPlusNodeC y;
		ReadBPlusNode(x.pointers[i].second, y);

		if (y.nkey == MAX_KEY) {		//如果x的子节点已满，则这个子节点分裂

			Split_BPlus_Node(x, y, i);
			WriteBPlusNode(current, x);
			WriteBPlusNode(x.pointers[i].second, y);
		}
		if (trecord.key <= x.keys[i] || i == x.nkey) {
			insert_bplus_tree(x.pointers[i].second, trecord);
		}
		else {
			insert_bplus_tree(x.pointers[i + 1].second, trecord);
		}

	}
	else {			//如果是叶节点,则直接将关键字插入key数组中


		for (int j = x.nkey; j > i; j--) {
			x.keys[j] = x.keys[j - 1];
			x.pointers[j] = x.pointers[j - 1];
		}
		x.keys[i] = trecord.key;
		x.nkey++;

		//将记录的地址赋给x.pointers[i]，同时重置判断类型状态
		x.pointers[i].second = trecord.Raddress;
		x.pointers[i].first = false;

		WriteBPlusNode(current, x);

		update_mete_tree();

	}

}


//分裂满的B+树节点
void	BPlusTreeC::Split_BPlus_Node(BPlusNodeC &father, BPlusNodeC &current, int childnum) {
	int half = MAX_KEY / 2;

	int i;

	for (i = father.nkey; i > childnum; i--) {
		father.keys[i] = father.keys[i - 1];
		father.pointers[i + 1] = father.pointers[i];
	}
	father.nkey++;

	BPlusNodeC t;

	NID++;
	FILEP address = NID;

	father.keys[childnum] = current.keys[half];
	father.pointers[childnum + 1].second = address;

	for (i = half + 1; i < MAX_KEY; i++) {
		t.keys[i - half - 1] = current.keys[i];
		t.pointers[i - half - 1] = current.pointers[i];
	}

	t.nkey = MAX_KEY - half - 1;
	t.pointers[t.nkey] = current.pointers[MAX_KEY];

	t.isleaf = current.isleaf;

	current.nkey = half;

	if (current.isleaf) {	//如果当前被分裂节点是叶子

		current.nkey++;
		t.pointers[MAX_KEY] = current.pointers[MAX_KEY];
		current.pointers[MAX_KEY].second = address;
		current.pointers[MAX_KEY].first = false;
	}

	WriteBPlusNode(address, t);

	update_mete_tree();

}


//在B+树查询一个关键字
void BPlusTreeC::Search_BPlus_Tree(TRecordC &trecord, TSearchResult &tresult) {
	int i;

	BPlusNodeC a;
	FILEP current = ROOT;

	do {
		ReadBPlusNode(current, a);

		for (i = 0; i < a.nkey && trecord.key > a.keys[i]; i++);

		if (i < a.nkey && a.isleaf && trecord.key == a.keys[i]) {		//在B+树叶节点找到了等值的关键字

			tresult.Baddress = current;
			tresult.Raddress = a.pointers[i].second;						//返回该关键字所对应的记录的地址
			tresult.exist = true;
			tresult.ismul = a.pointers[i].first;

			return;
		}
		current = a.pointers[i].second;

	} while (!a.isleaf);

	tresult.exist = false;
}




void	BPlusTreeC::delete_BPlus_tree(FILEP current, TRecordC &trecord) {
	int i, j;
	int flag_F1 = 0;
	int flag_F2 = 0;
	int flag_F2_child_position = 0;

	BPlusNodeC x;
	ReadBPlusNode(current, x);


	for (i = 0; i < x.nkey && trecord.key > x.keys[i]; i++);

	if (i < x.nkey && x.keys[i] == trecord.key) {	//在当前节点找到关键字


		if (!x.isleaf) {	  //在内节点找到关键字

			BPlusNodeC child;
			ReadBPlusNode(x.pointers[i].second, child);

			if (child.isleaf) {	 //如果孩子是叶节点

				if (child.nkey > MAX_KEY / 2) {		//情况A

					//释放多个记录地址的索引
					if (child.pointers[child.nkey - 1].first)
						DeleteRaddressIndex(child.pointers[child.nkey - 1].second);

					x.keys[i] = child.keys[child.nkey - 2];//关键字重置为叶节点倒数第二个
					child.nkey--;

					WriteBPlusNode(current, x);
					WriteBPlusNode(x.pointers[i].second, child);

					return;
				}
				else {	//否则孩子节点的关键字数量不过半

					if (i > 0) {		//有左兄弟节点

						BPlusNodeC lbchild;
						ReadBPlusNode(x.pointers[i - 1].second, lbchild);

						if (lbchild.nkey > MAX_KEY / 2) {		//情况B

							for (j = child.nkey; j > 0; j--) {
								child.keys[j] = child.keys[j - 1];
								child.pointers[j] = child.pointers[j - 1];
							}

							child.keys[0] = x.keys[i - 1];
							child.pointers[0] = lbchild.pointers[lbchild.nkey - 1];

							child.nkey++;

							lbchild.nkey--;

							x.keys[i - 1] = lbchild.keys[lbchild.nkey - 1];
							x.keys[i] = child.keys[child.nkey - 2];

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i - 1].second, lbchild);
							WriteBPlusNode(x.pointers[i].second, child);

						}
						else {	//情况C

							for (j = 0; j < child.nkey; j++) {
								lbchild.keys[lbchild.nkey + j] = child.keys[j];
								lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.pointers[MAX_KEY] = child.pointers[MAX_KEY];

							//释放child节点占用的空间x.pointers[i]
							//物理删除
							DeleteBplusNode(x.pointers[i].second);

							for (j = i - 1; j < x.nkey - 1; j++) {
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}
							x.nkey--;

							x.keys[i - 1] = lbchild.keys[lbchild.nkey - 2];

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i - 1].second, lbchild);

							i--;

						}


					}
					else		//只有右兄弟节点
					{
						BPlusNodeC rbchild;
						ReadBPlusNode(x.pointers[i + 1].second, rbchild);

						if (rbchild.nkey > MAX_KEY / 2) {		//情况D

							x.keys[i] = rbchild.keys[0];
							child.keys[child.nkey] = rbchild.keys[0];
							child.pointers[child.nkey] = rbchild.pointers[0];
							child.nkey++;

							for (j = 0; j < rbchild.nkey - 1; j++) {
								rbchild.keys[j] = rbchild.keys[j + 1];
								rbchild.pointers[j] = rbchild.pointers[j + 1];
							}

							rbchild.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i].second, child);
							WriteBPlusNode(x.pointers[i + 1].second, rbchild);

						}
						else {	//情况E
							for (j = 0; j < rbchild.nkey; j++) {
								child.keys[child.nkey + j] = rbchild.keys[j];
								child.pointers[child.nkey + j] = rbchild.pointers[j];
							}
							child.nkey += rbchild.nkey;

							child.pointers[MAX_KEY] = rbchild.pointers[MAX_KEY];

							//释放rbchild占用的空间x.pointers[i+1]
							//物理删除
							DeleteBplusNode(x.pointers[i + 1].second);

							for (j = i; j < x.nkey - 1; j++) {
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}
							x.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i].second, child);

						}

					}

				}

			}
			else {		//情况F

				//找到key在B+树叶节点的左兄弟关键字,将这个关键字取代key的位置

				TRecordC ttrecord;
				ttrecord.key = trecord.key;
				TSearchResult tresult;
				Search_BPlus_Tree(ttrecord, tresult);

				BPlusNodeC last;

				ReadBPlusNode(tresult.Baddress, last);

				KEYTYPE2 should_remember = x.keys[i];


				//要改！！！

				if (last.nkey < 2) {
					//cout << "fuck" << endl;
					return;

				}

				x.keys[i] = last.keys[last.nkey - 2];

				WriteBPlusNode(current, x);


				if (child.nkey > MAX_KEY / 2) {		  //情况H


				}
				else {		  //否则孩子节点的关键字数量不过半,则将兄弟节点的某一个关键字移至孩子

					if (i > 0) {  //x.keys[i]有左兄弟

						BPlusNodeC lbchild;
						ReadBPlusNode(x.pointers[i - 1].second, lbchild);

						if (lbchild.nkey > MAX_KEY / 2) {		//情况I

							for (j = child.nkey; j > 0; j--) {
								child.keys[j] = child.keys[j - 1];
								child.pointers[j + 1] = child.pointers[j];
							}
							child.pointers[1] = child.pointers[0];
							child.keys[0] = x.keys[i - 1];
							child.pointers[0] = lbchild.pointers[lbchild.nkey];

							child.nkey++;

							x.keys[i - 1] = lbchild.keys[lbchild.nkey - 1];
							lbchild.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i - 1].second, lbchild);
							WriteBPlusNode(x.pointers[i].second, child);
						}
						else {		//情况J
							lbchild.keys[lbchild.nkey] = x.keys[i - 1];	//将孩子节点复制到其左兄弟的末尾
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++) {		//将child节点拷贝到lbchild节点的末尾
								lbchild.keys[lbchild.nkey + j] = child.keys[j];
								lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							}
							lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							lbchild.nkey += child.nkey;		  //已经将child拷贝到lbchild节点


							//释放child节点的存储空间,x.pointers[i]
							//物理删除
							DeleteBplusNode(x.pointers[i].second);

							//将找到关键字的孩子child与关键字左兄弟的孩子lbchild合并后,将该关键字前移,使当前节点的关键字减少一个
							for (j = i - 1; j < x.nkey - 1; j++) {
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}
							x.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i - 1].second, lbchild);

							i--;

						}

					}
					else {		  //否则x.keys[i]只有右兄弟

						BPlusNodeC rbchild;
						ReadBPlusNode(x.pointers[i + 1].second, rbchild);

						if (rbchild.nkey > MAX_KEY / 2) {	  //情况K

							//child.keys[child.nkey] = x.keys[i];
							child.keys[child.nkey] = should_remember;
							child.nkey++;

							child.pointers[child.nkey] = rbchild.pointers[0];
							x.keys[i] = rbchild.keys[0];

							for (j = 0; j < rbchild.nkey - 1; j++) {
								rbchild.keys[j] = rbchild.keys[j + 1];
								rbchild.pointers[j] = rbchild.pointers[j + 1];
							}
							rbchild.pointers[j] = rbchild.pointers[j + 1];
							rbchild.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i].second, child);
							WriteBPlusNode(x.pointers[i + 1].second, rbchild);

							//快改！！！特殊情况1
							BPlusNodeC temp_grandchildren;
							ReadBPlusNode(child.pointers[child.nkey - 1].second, temp_grandchildren);
							//if (temp_grandchildren.isleaf) {
							//	//释放多个记录地址的索引
							//	if (child.pointers[child.nkey - 1].first)
							//		DeleteRaddressIndex(child.pointers[child.nkey - 1].second);
							//	temp_grandchildren.nkey--;//删除最后一个地址
							//	WriteBPlusNode(child.pointers[child.nkey - 1].second, temp_grandchildren);
							//	return;
							//}
							flag_F1 = 1;

						}
						else {		//情况L

							//child.keys[child.nkey] = x.keys[i];
							child.keys[child.nkey] = should_remember;
							flag_F2_child_position = child.nkey;
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++) {		//将rbchild节点合并到child节点后
								child.keys[child.nkey + j] = rbchild.keys[j];
								child.pointers[child.nkey + j] = rbchild.pointers[j];
							}
							child.pointers[child.nkey + j] = rbchild.pointers[j];

							child.nkey += rbchild.nkey;

							//释放rbchild节点所占用的空间,x,pointers[i+1]
							//物理删除
							DeleteBplusNode(x.pointers[i + 1].second);

							for (j = i; j < x.nkey - 1; j++) {	  //当前将关键字之后的关键字左移一位,使该节点的关键字数量减一
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}
							x.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i].second, child);

							////快改！！！特殊情况2
							//BPlusNodeC temp_grandchildren;
							//ReadBPlusNode(child.pointers[child.nkey - 2].second, temp_grandchildren);
							//if (temp_grandchildren.isleaf) {
							//	//释放多个记录地址的索引
							//	if (child.pointers[child.nkey - 1].first)
							//		DeleteRaddressIndex(child.pointers[child.nkey - 1].second);
							//	temp_grandchildren.nkey--;//删除最后一个地址
							//	WriteBPlusNode(child.pointers[child.nkey - 2].second, temp_grandchildren);
							//	return;
							//}
							flag_F2 = 1;
						}

					}
				}

			}

			delete_BPlus_tree(x.pointers[i].second, trecord);

			//要改
			if (flag_F1 == 1) {
				child.keys[child.nkey - 1] = x.keys[i];
			}

			if (flag_F2 == 1) {
				child.keys[flag_F2_child_position] = x.keys[i];
			}

		}
		else {  //情况G
			//释放多个记录地址的索引
			if (x.pointers[i].first)
				DeleteRaddressIndex(x.pointers[i].second);

			for (j = i; j < x.nkey - 1; j++) {
				x.keys[j] = x.keys[j + 1];
				x.pointers[j] = x.pointers[j + 1];
			}
			x.nkey--;

			WriteBPlusNode(current, x);

			return;
		}

	}
	else {		  //在当前节点没找到关键字	  
		if (!x.isleaf) {	  //没找到关键字,则关键字必然包含在以pointers[i]为根的子树中

			BPlusNodeC child;
			ReadBPlusNode(x.pointers[i].second, child);

			if (!child.isleaf) {	  //如果其孩子节点是内节点
				if (child.nkey > MAX_KEY / 2) {		  //情况H
					//不做处理
				}
				else {		  //否则孩子节点的关键字数量不过半,则将兄弟节点的某一个关键字移至孩子

					if (i > 0) {  //x.keys[i]有左兄弟

						BPlusNodeC lbchild;
						ReadBPlusNode(x.pointers[i - 1].second, lbchild);

						if (lbchild.nkey > MAX_KEY / 2) {	//情况I

							for (j = child.nkey; j > 0; j--) {
								child.keys[j] = child.keys[j - 1];
								child.pointers[j + 1] = child.pointers[j];
							}
							child.pointers[1] = child.pointers[0];
							child.keys[0] = x.keys[i - 1];
							child.pointers[0] = lbchild.pointers[lbchild.nkey];

							child.nkey++;

							x.keys[i - 1] = lbchild.keys[lbchild.nkey - 1];
							lbchild.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i - 1].second, lbchild);
							WriteBPlusNode(x.pointers[i].second, child);
						}
						else {		//情况J

							lbchild.keys[lbchild.nkey] = x.keys[i - 1];	//将孩子节点复制到其左兄弟的末尾
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++) {		//将child节点拷贝到lbchild节点的末尾,
								lbchild.keys[lbchild.nkey + j] = child.keys[j];
								lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							}
							lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							lbchild.nkey += child.nkey;		  //已经将child拷贝到lbchild节点


							//释放child节点的存储空间x.pointers[i]
							//物理删除
							DeleteBplusNode(x.pointers[i].second);


							//将找到关键字的孩子child与关键字左兄弟的孩子lbchild合并后,将该关键字前移,使当前节点的关键字减少一个
							for (j = i - 1; j < x.nkey - 1; j++) {
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}
							x.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i - 1].second, lbchild);

							i--;

						}

					}
					else		  //否则x.keys[i]只有右兄弟
					{
						BPlusNodeC rbchild;
						ReadBPlusNode(x.pointers[i + 1].second, rbchild);

						if (rbchild.nkey > MAX_KEY / 2) {	  //情况K

							child.keys[child.nkey] = x.keys[i];
							child.nkey++;

							child.pointers[child.nkey] = rbchild.pointers[0];
							x.keys[i] = rbchild.keys[0];

							for (j = 0; j < rbchild.nkey - 1; j++) {
								rbchild.keys[j] = rbchild.keys[j + 1];
								rbchild.pointers[j] = rbchild.pointers[j + 1];
							}
							rbchild.pointers[j] = rbchild.pointers[j + 1];
							rbchild.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i].second, child);
							WriteBPlusNode(x.pointers[i + 1].second, rbchild);

						}
						else {		//情况L

							child.keys[child.nkey] = x.keys[i];
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++) {		//将rbchild节点合并到child节点后
								child.keys[child.nkey + j] = rbchild.keys[j];
								child.pointers[child.nkey + j] = rbchild.pointers[j];
							}
							child.pointers[child.nkey + j] = rbchild.pointers[j];

							child.nkey += rbchild.nkey;

							//释放rbchild节点所占用的空间,x.pointers[i+1]
							//物理删除
							DeleteBplusNode(x.pointers[i + 1].second);

							for (j = i; j < x.nkey - 1; j++) {	  //当前将关键字之后的关键字左移一位,使该节点的关键字数量减一
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}
							x.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i].second, child);

						}

					}
				}
			}
			else {	//否则其孩子节点是外节点
				if (child.nkey > MAX_KEY / 2) {  //情况M
					//不做处理
				}
				else {		//否则孩子节点不到半满

					if (i > 0) { //有左兄弟

						BPlusNodeC lbchild;
						ReadBPlusNode(x.pointers[i - 1].second, lbchild);

						if (lbchild.nkey > MAX_KEY / 2) {	//情况N
							for (j = child.nkey; j > 0; j--) {
								child.keys[j] = child.keys[j - 1];
								child.pointers[j] = child.pointers[j - 1];
							}
							child.keys[0] = x.keys[i - 1];
							child.pointers[0] = lbchild.pointers[lbchild.nkey - 1];
							child.nkey++;
							lbchild.nkey--;

							x.keys[i - 1] = lbchild.keys[lbchild.nkey - 1];

							WriteBPlusNode(x.pointers[i - 1].second, lbchild);
							WriteBPlusNode(x.pointers[i].second, child);
							WriteBPlusNode(current, x);

						}
						else {		//情况O

							for (j = 0; j < child.nkey; j++) {		//与左兄弟孩子节点合并	
								lbchild.keys[lbchild.nkey + j] = child.keys[j];
								lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.pointers[MAX_KEY] = child.pointers[MAX_KEY];

							//释放child占用的空间x.pointers[i]
							//物理删除
							DeleteBplusNode(x.pointers[i].second);

							for (j = i - 1; j < x.nkey - 1; j++) {
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}

							x.nkey--;

							WriteBPlusNode(x.pointers[i - 1].second, lbchild);
							WriteBPlusNode(current, x);

							i--;

						}

					}
					else {		  //否则只有右兄弟

						BPlusNodeC rbchild;
						ReadBPlusNode(x.pointers[i + 1].second, rbchild);

						if (rbchild.nkey > MAX_KEY / 2) {		//情况P

							x.keys[i] = rbchild.keys[0];
							child.keys[child.nkey] = rbchild.keys[0];
							child.pointers[child.nkey] = rbchild.pointers[0];
							child.nkey++;

							for (j = 0; j < rbchild.nkey - 1; j++) {
								rbchild.keys[j] = rbchild.keys[j + 1];
								rbchild.pointers[j] = rbchild.pointers[j + 1];
							}
							rbchild.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i + 1].second, rbchild);
							WriteBPlusNode(x.pointers[i].second, child);

						}
						else		//情况Q
						{
							for (j = 0; j < rbchild.nkey; j++) {
								child.keys[child.nkey + j] = rbchild.keys[j];
								child.pointers[child.nkey + j] = rbchild.pointers[j];
							}
							child.nkey += rbchild.nkey;
							child.pointers[MAX_KEY] = rbchild.pointers[MAX_KEY];

							//释放rbchild占用的空间x.pointers[i+1]
							//物理删除
							DeleteBplusNode(x.pointers[i + 1].second);

							for (j = i; j < x.nkey - 1; j++) {
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}
							x.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i].second, child);


						}

					}

				}

			}

			delete_BPlus_tree(x.pointers[i].second, trecord);
		}


	}


}



void	BPlusTreeC::Delete_BPlus_Tree(TRecordC &trecord)	//在B+中删除一个关键字
{
	delete_BPlus_tree(ROOT, trecord);

	BPlusNodeC rootnode;
	ReadBPlusNode(ROOT, rootnode);

	if (!rootnode.isleaf && rootnode.nkey == 0)	  //如果删除关键字后根节点不是叶节点，并且关键字数量为0时根节点也应该被删除
	{
		//释放ROOT节点占用的空间
		//物理删除
		DeleteBplusNode(ROOT);

		ROOT = rootnode.pointers[0].second;			//根节点下移,B+树高度减1

		update_mete_tree();

	}

}

void BPlusTreeC::Delete_BPlus_Tree_Address_Under_Key(TRecordC & trecord)
{

	TSearchResult tresult;
	Search_BPlus_Tree(trecord, tresult);
	if (!tresult.exist) {
		//cout << "错误！！！不可能出现这个信息的。这种情况下是无此关键字但无此文件地址"<<endl;
		//system("pause");
	}
	else if (!tresult.ismul) {//直接删除记录指针
		if (trecord.Raddress != tresult.Raddress) {
			//cout << "错误！！！不可能出现此信息。这种情况下是关键字和文件地址不符"<<endl;
			//system("pause");
			//return;
		}
		Delete_BPlus_Tree(trecord);
	}
	else {//指针是多个记录的索引指针，所以删除单个记录就可以了
		int count = DeleteSingleRaddressIndex(tresult.Raddress, trecord.Raddress);//删除索引中的单个记录地址
		if (count == 0) {
			//cout << "错误！！！不可能出现这个信息的。这种情况下是有此关键字，有此索引，但索引就被你减到0了，不可能的"<<endl;
			//system("pause");
		}
		if (count == 1) {//当count==1时，索引作用失去，删除索引
			TRecordC temprecord;
			GetTheOnlyRaddressIndex(tresult.Raddress, temprecord);//得到最后的记录地址
			temprecord.key = trecord.key;
			Delete_BPlus_Tree(trecord);//删除原树中的指针（就是把那个索引删掉）
			Insert_BPlus_Tree(temprecord);
		}
	}
}




void  BPlusTreeC::EnumLeafKey()	//依次枚举B+树叶节点的所有关键字
{
	BPlusNodeC head;

	ReadBPlusNode(ROOT, head);

	while (!head.isleaf)
	{
		ReadBPlusNode(head.pointers[0].second, head);
	}

	while (1)
	{
		for (int i = 0; i < head.nkey; i++)
			printf("%s\n", head.keys[i].ch);

		if (head.pointers[MAX_KEY].second == 0)
			break;

		ReadBPlusNode(head.pointers[MAX_KEY].second, head);
	}

}




//读取一个结点
inline void	BPlusTreeC::ReadBPlusNode(FILEP address, BPlusNodeC	&n)
{
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\tree%d.dat", tablename.c_str(), keyname.c_str(), address);
	ifstream in_file(str, ios::in | ios::binary);
	free(str);
	//把in_file文件写入BPlusNodeC n中
	in_file.read((char*)&n, sizeof(n));
	in_file.close();

}

//写入一个结点
inline void	BPlusTreeC::WriteBPlusNode(FILEP address, BPlusNodeC &n) {
	char *str;
	str = (char*)malloc(sizeof(char)* BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\tree%d.dat", tablename.c_str(), keyname.c_str(), address);
	ofstream out_file(str, ios::out | ios::trunc | ios::binary);
	free(str);
	//把BPlusNodeC n写入out_file文件
	out_file.write((char*)&n, sizeof(n));
	out_file.close();

}

inline void BPlusTreeC::DeleteBplusNode(FILEP address) {
	char *str;
	str = (char*)malloc(sizeof(char)* BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\tree%d.dat", tablename.c_str(), keyname.c_str(), address);
	int err = remove(str);
	if (err == -1)
		cout << "删除树结点失败";
}

void BPlusTreeC::GetTheOnlyRaddressIndex(FILEP INDID_ADDRESS, TRecordC & trecord)
{
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), INDID_ADDRESS);
	fstream f;
	f.open(str, ios::in | ios::binary);
	FILEP s;
	f.read((char*)&s, sizeof(s));
	f.close();
	trecord.Raddress = s;
}

inline void BPlusTreeC::WriteRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress) {
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), INDID_ADDRESS);
	ofstream out_file(str, ios::out | ios::binary | ios::app);
	free(str);
	//把Raddress追加写入索引中
	out_file.write((char*)&raddress, sizeof(raddress));
	out_file.close();
}

int BPlusTreeC::DeleteSingleRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress)
{
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), INDID_ADDRESS);
	fstream f;
	f.open(str, ios::in | ios::binary);
	vector<FILEP> a;
	FILEP s;
	int flag = 0;
	while (1) {
		f.read((char*)&s, sizeof(s));
		if (f.eof()) break;//当读到末尾时就结束
		if (s == raddress) {
			flag ++;
		}
		if (s != raddress)
			a.push_back(s);//留下非删除的记录地址

	}
	f.close();

	if (flag == 0) {
		cout << "索引内无此地址！！！:";
		cout << raddress << endl;;
		system("pause");
	}
	if(flag>=2){
		cout << "有多个地址重复！";
		cout << raddress << endl;
		system("pause");
	}

	f.open(str, ios::out | ios::binary | ios::trunc);
	for (int i = 0; i < a.size(); i++) {
		f.write((char*)&a[i], sizeof(FILEP));
	}
	f.close();

	int count = a.size();
	return count;
}

inline void BPlusTreeC::DeleteRaddressIndex(FILEP INDID_ADDRESS) {
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), INDID_ADDRESS);
	int err = remove(str);
	if (err == -1) {
		//cout << "删除多个记录地址的索引失败";
	}
}


