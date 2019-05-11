#include "BPlusTree.h"

BPlusTree::BPlusTree() {
}

BPlusTree :: ~BPlusTree() {

}




void BPlusTree::ReadBPlusTree(string tname, string kname) {
	tablename = tname;
	keyname = kname;
	char *str;
	str = (char*)malloc(sizeof(char)* BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\meta_tree.dat", tablename.c_str(), keyname.c_str());

	ifstream in_file(str, ios::in | ios::binary);//��meta_tree.dat�ļ�
	//���meta_tree.dat�ļ������ڣ����½��ñ�
	if (!in_file) {
		ofstream out_file(str, ios::out | ios::trunc | ios::binary);

		if (!out_file) {//д��ʧ��
			printf("д�����ʧ��");
			abort();
		}

		NID = 0;
		INDID = 0;
		ROOT = 0;
		out_file << NID << " " << INDID << " " << ROOT;
		out_file.close();

		//��ʼʱ���ڵ�ΪҶ�ڵ�
		BPlusNode root;
		//���ڵ��޺������
		root.pointers[MAX_KEY].second = 0;
		//��ʼ���ڵ��޹ؼ���
		root.nkey = 0;
		//��ʼ���ڵ���Ҷ�ڵ�
		root.isleaf = true;
		//�Ѹ��ڵ�д���ļ�
		WriteBPlusNode(NID, root);
	}
	//���meta_tree.dat�ļ�����,����meta_tree.dat��Ϣ����
	else {
		in_file >> NID >> INDID >> ROOT;
		in_file.close();
	}

	free(str);
}

void BPlusTree::update_mete_tree() {
	char *str;
	str = (char*)malloc(sizeof(char)* BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\meta_tree.dat", tablename.c_str(), keyname.c_str());
	ofstream out_file(str, ios::out | ios::trunc | ios::binary);
	free(str);
	//�Ѵ�������Ϣд���ļ�
	out_file << NID << " " << INDID << " " << ROOT;
	out_file.close();
}


FILEP BPlusTree::get_root()
{
	return ROOT;
}

//������b+��  ��Ϊ����ʹ��
void	BPlusTree::Build_BPlus_Tree(string tname, string kname) {
	NID = 0;
	INDID = 0;
	ROOT = 0;
	tablename = tname;
	keyname = kname;
	BPlusNode n;
	n.pointers[MAX_KEY].second = 0;
	n.nkey = 0;
	n.isleaf = true;
	WriteBPlusNode(ROOT, n);
}


//��B+������ؼ���
void	BPlusTree::Insert_BPlus_Tree(TRecord &trecord) {

	BPlusNode n;
	ReadBPlusNode(ROOT, n);

	//���Ѹ��ڵ�
	if (n.nkey == MAX_KEY)
	{
		BPlusNode newroot;
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



void	BPlusTree::insert_bplus_tree(FILEP current, TRecord &trecord) {
	BPlusNode x;
	ReadBPlusNode(current, x);

	int	i;
	for (i = 0; i < x.nkey && x.keys[i] < trecord.key; i++);

	if (i < x.nkey && x.isleaf && x.keys[i] == trecord.key) {	//��B+��Ҷ�ڵ��ҵ�����ͬ�ؼ���

		//��ǰָ��Ϊָ������¼��ַ������ʱ
		if (x.pointers[i].first) {
			WriteRaddressIndex(x.pointers[i].second, trecord.Raddress);//ֱ�Ӹ��¶����¼��ַ������
		}

		//��ǰָ��Ϊָ�򵥸���¼��ַʱ
		else if (!x.pointers[i].first) {
			INDID++;//�����ļ���һ
			x.pointers[i].first = true;//�����Ѿ�ָ������¼��ַ������
			WriteRaddressIndex(INDID, x.pointers[i].second);	//��ԭ���˴��ļ�¼��ַ�ӵ�������
			x.pointers[i].second = INDID;//�ı�ָ��ָ�򣺴�ָ��һ��¼��ַ��ָ������¼��ַ������
			WriteBPlusNode(current, x);//���½��
			WriteRaddressIndex(INDID, trecord.Raddress);//���¶����¼��ַ������
		}

		update_mete_tree();
		return;
	}

	if (!x.isleaf) {	//�������Ҷ�ڵ�

		BPlusNode y;
		ReadBPlusNode(x.pointers[i].second, y);

		if (y.nkey == MAX_KEY) {		//���x���ӽڵ�������������ӽڵ����

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
	else {			//�����Ҷ�ڵ�,��ֱ�ӽ��ؼ��ֲ���key������


		for (int j = x.nkey; j > i; j--) {
			x.keys[j] = x.keys[j - 1];
			x.pointers[j] = x.pointers[j - 1];
		}
		x.keys[i] = trecord.key;
		x.nkey++;

		//����¼�ĵ�ַ����x.pointers[i]��ͬʱ�����ж�����״̬
		x.pointers[i].second = trecord.Raddress;
		x.pointers[i].first = false;

		WriteBPlusNode(current, x);

		update_mete_tree();

	}

}


//��������B+���ڵ�
void	BPlusTree::Split_BPlus_Node(BPlusNode &father, BPlusNode &current, int childnum) {
	int half = MAX_KEY / 2;

	int i;

	for (i = father.nkey; i > childnum; i--) {
		father.keys[i] = father.keys[i - 1];
		father.pointers[i + 1] = father.pointers[i];
	}
	father.nkey++;

	BPlusNode t;

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

	if (current.isleaf) {	//�����ǰ�����ѽڵ���Ҷ��

		current.nkey++;
		t.pointers[MAX_KEY] = current.pointers[MAX_KEY];
		current.pointers[MAX_KEY].second = address;
		current.pointers[MAX_KEY].first = false;
	}

	WriteBPlusNode(address, t);

	update_mete_tree();

}


//��B+����ѯһ���ؼ��ֵĽ��
void BPlusTree::Search_BPlus_Tree(TRecord &trecord, TSearchResult &tresult) {
	int i;

	BPlusNode a;
	FILEP current = ROOT;

	do {
		ReadBPlusNode(current, a);

		for (i = 0; i < a.nkey && trecord.key > a.keys[i]; i++);

		if (i < a.nkey && a.isleaf && trecord.key == a.keys[i]) {		//��B+��Ҷ�ڵ��ҵ��˵�ֵ�Ĺؼ���

			tresult.Baddress = current;
			tresult.Raddress = a.pointers[i].second;						//���ظùؼ�������Ӧ�ļ�¼�ĵ�ַ
			tresult.exist = true;
			tresult.ismul = a.pointers[i].first;

			return;
		}
		current = a.pointers[i].second;

	} while (!a.isleaf);

	tresult.exist = false;
}

//��B+����ѯһ�����ڵ��ڸò��ҹؼ��ֵĽ�������ڴ��ڵķ�Χ���ң�����ĺ����벻Ҫ�ã�
void BPlusTree::Search_BPlus_Tree_Bigger(TRecord &trecord, TSearchResult &tresult) {
	int i;

	BPlusNode a;
	FILEP current = ROOT;

	do {
		ReadBPlusNode(current, a);

		for (i = 0; i < a.nkey && trecord.key > a.keys[i]; i++);

		if (i < a.nkey && a.isleaf && trecord.key == a.keys[i]) {		//��B+��Ҷ�ڵ��ҵ��˵�ֵ�Ĺؼ���

			tresult.Baddress = current;
			tresult.Raddress = a.pointers[i].second;						//���ظùؼ�������Ӧ�ļ�¼�ĵ�ַ
			tresult.exist = true;
			tresult.ismul = a.pointers[i].first;

			return;
		}
		else if (a.isleaf) {
			for (i = 0; trecord.key > a.keys[i]; i++);//��λ����һ�����ڲ��ҹؼ��ֵĹؼ���
			tresult.Baddress = current;
			tresult.Raddress = a.pointers[i].second;
			tresult.exist = true;
			tresult.ismul = a.pointers[i].first;
		}

		current = a.pointers[i].second;

	} while (!a.isleaf);

}



void	BPlusTree::delete_BPlus_tree(FILEP current, TRecord &trecord) {
	int i, j;
	int flag_F1 = 0;
	int flag_F2 = 0;
	int flag_F2_child_position = 0;

	BPlusNode x;
	ReadBPlusNode(current, x);


	for (i = 0; i < x.nkey && trecord.key > x.keys[i]; i++);

	if (i < x.nkey && x.keys[i] == trecord.key) {	//�ڵ�ǰ�ڵ��ҵ��ؼ���


		if (!x.isleaf) {	  //���ڽڵ��ҵ��ؼ���

			BPlusNode child;
			ReadBPlusNode(x.pointers[i].second, child);

			if (child.isleaf) {	 //���������Ҷ�ڵ�

				if (child.nkey > MAX_KEY / 2) {		//���A

					//�ͷŶ����¼��ַ������
					if (child.pointers[child.nkey - 1].first)
						DeleteRaddressIndex(child.pointers[child.nkey - 1].second);

					x.keys[i] = child.keys[child.nkey - 2];//�ؼ�������ΪҶ�ڵ㵹���ڶ���
					child.nkey--;

					WriteBPlusNode(current, x);
					WriteBPlusNode(x.pointers[i].second, child);

					return;
				}
				else {	//�����ӽڵ�Ĺؼ�������������

					if (i > 0) {		//�����ֵܽڵ�

						BPlusNode lbchild;
						ReadBPlusNode(x.pointers[i - 1].second, lbchild);

						if (lbchild.nkey > MAX_KEY / 2) {		//���B

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
						else {	//���C

							for (j = 0; j < child.nkey; j++) {
								lbchild.keys[lbchild.nkey + j] = child.keys[j];
								lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.pointers[MAX_KEY] = child.pointers[MAX_KEY];

							//�ͷ�child�ڵ�ռ�õĿռ�x.pointers[i]
							//����ɾ��
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
					else		//ֻ�����ֵܽڵ�
					{
						BPlusNode rbchild;
						ReadBPlusNode(x.pointers[i + 1].second, rbchild);

						if (rbchild.nkey > MAX_KEY / 2) {		//���D

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
						else {	//���E
							for (j = 0; j < rbchild.nkey; j++) {
								child.keys[child.nkey + j] = rbchild.keys[j];
								child.pointers[child.nkey + j] = rbchild.pointers[j];
							}
							child.nkey += rbchild.nkey;

							child.pointers[MAX_KEY] = rbchild.pointers[MAX_KEY];

							//�ͷ�rbchildռ�õĿռ�x.pointers[i+1]
							//����ɾ��
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
			else {		//���F

				//�ҵ�key��B+��Ҷ�ڵ�����ֵܹؼ���,������ؼ���ȡ��key��λ��

				TRecord ttrecord;
				ttrecord.key = trecord.key;
				TSearchResult tresult;
				Search_BPlus_Tree(ttrecord, tresult);

				BPlusNode last;

				ReadBPlusNode(tresult.Baddress, last);


				//Ҫ�ģ�����

				if (last.nkey < 2) {
					//cout << "no posible last key -2" << endl;
					return;
				}

				KEYTYPE should_remember = x.keys[i];//��¼ԭ����

				x.keys[i] = last.keys[last.nkey - 2];

				WriteBPlusNode(current, x);


				if (child.nkey > MAX_KEY / 2) {		  //���H


				}
				else {		  //�����ӽڵ�Ĺؼ�������������,���ֵܽڵ��ĳһ���ؼ�����������

					if (i > 0) {  //x.keys[i]�����ֵ�

						BPlusNode lbchild;
						ReadBPlusNode(x.pointers[i - 1].second, lbchild);

						if (lbchild.nkey > MAX_KEY / 2) {		//���I

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
						else {		//���J
							lbchild.keys[lbchild.nkey] = x.keys[i - 1];	//�����ӽڵ㸴�Ƶ������ֵܵ�ĩβ
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++) {		//��child�ڵ㿽����lbchild�ڵ��ĩβ
								lbchild.keys[lbchild.nkey + j] = child.keys[j];
								lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							}
							lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							lbchild.nkey += child.nkey;		  //�Ѿ���child������lbchild�ڵ�


							//�ͷ�child�ڵ�Ĵ洢�ռ�,x.pointers[i]
							//����ɾ��
							DeleteBplusNode(x.pointers[i].second);

							//���ҵ��ؼ��ֵĺ���child��ؼ������ֵܵĺ���lbchild�ϲ���,���ùؼ���ǰ��,ʹ��ǰ�ڵ�Ĺؼ��ּ���һ��
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
					else {		  //����x.keys[i]ֻ�����ֵ�

						BPlusNode rbchild;
						ReadBPlusNode(x.pointers[i + 1].second, rbchild);

						if (rbchild.nkey > MAX_KEY / 2) {	  //���K

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

							////��ģ������������1
							//BPlusNode temp_grandchildren;
							//ReadBPlusNode(child.pointers[child.nkey - 1].second, temp_grandchildren);
							//if (temp_grandchildren.isleaf) {
							//	//�ͷŶ����¼��ַ������
							//	if (child.pointers[child.nkey - 1].first)
							//		DeleteRaddressIndex(child.pointers[child.nkey - 1].second);
							//	temp_grandchildren.nkey--;//ɾ�����һ����ַ
							//	WriteBPlusNode(child.pointers[child.nkey - 1].second, temp_grandchildren);
							//	return;
							//}
							flag_F1 = 1;

						}
						else {		//���L

							//child.keys[child.nkey] = x.keys[i];
							child.keys[child.nkey] = should_remember;
							flag_F2_child_position = child.nkey;

							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++) {		//��rbchild�ڵ�ϲ���child�ڵ��
								child.keys[child.nkey + j] = rbchild.keys[j];
								child.pointers[child.nkey + j] = rbchild.pointers[j];
							}
							child.pointers[child.nkey + j] = rbchild.pointers[j];

							child.nkey += rbchild.nkey;

							//�ͷ�rbchild�ڵ���ռ�õĿռ�,x,pointers[i+1]
							//����ɾ��
							DeleteBplusNode(x.pointers[i + 1].second);

							for (j = i; j < x.nkey - 1; j++) {	  //��ǰ���ؼ���֮��Ĺؼ�������һλ,ʹ�ýڵ�Ĺؼ���������һ
								x.keys[j] = x.keys[j + 1];
								x.pointers[j + 1] = x.pointers[j + 2];
							}
							x.nkey--;

							WriteBPlusNode(current, x);
							WriteBPlusNode(x.pointers[i].second, child);

							////��ģ������������2
							//BPlusNode temp_grandchildren;
							//ReadBPlusNode(child.pointers[child.nkey - 2].second, temp_grandchildren);
							//if (temp_grandchildren.isleaf) {
							//	//�ͷŶ����¼��ַ������
							//	if (child.pointers[child.nkey - 1].first)
							//		DeleteRaddressIndex(child.pointers[child.nkey - 1].second);
							//	temp_grandchildren.nkey--;//ɾ�����һ����ַ
							//	WriteBPlusNode(child.pointers[child.nkey - 2].second, temp_grandchildren);
							//	return;
							//}

							flag_F2 = 1;

						}

					}
				}

			}

			delete_BPlus_tree(x.pointers[i].second, trecord);

			//Ҫ��
			if (flag_F1 == 1) {
				child.keys[child.nkey - 1] = x.keys[i];
			}

			if (flag_F2 == 1) {
				child.keys[flag_F2_child_position] = x.keys[i];
			}
		}
		else {  //���G
			//�ͷŶ����¼��ַ������
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
	else {		  //�ڵ�ǰ�ڵ�û�ҵ��ؼ���	  
		if (!x.isleaf) {	  //û�ҵ��ؼ���,��ؼ��ֱ�Ȼ��������pointers[i]Ϊ����������

			BPlusNode child;
			ReadBPlusNode(x.pointers[i].second, child);

			if (!child.isleaf) {	  //����亢�ӽڵ����ڽڵ�
				if (child.nkey > MAX_KEY / 2) {		  //���H
					//��������
				}
				else {		  //�����ӽڵ�Ĺؼ�������������,���ֵܽڵ��ĳһ���ؼ�����������

					if (i > 0) {  //x.keys[i]�����ֵ�

						BPlusNode lbchild;
						ReadBPlusNode(x.pointers[i - 1].second, lbchild);

						if (lbchild.nkey > MAX_KEY / 2) {	//���I

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
						else {		//���J

							lbchild.keys[lbchild.nkey] = x.keys[i - 1];	//�����ӽڵ㸴�Ƶ������ֵܵ�ĩβ
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++) {		//��child�ڵ㿽����lbchild�ڵ��ĩβ,
								lbchild.keys[lbchild.nkey + j] = child.keys[j];
								lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							}
							lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							lbchild.nkey += child.nkey;		  //�Ѿ���child������lbchild�ڵ�


							//�ͷ�child�ڵ�Ĵ洢�ռ�x.pointers[i]
							//����ɾ��
							DeleteBplusNode(x.pointers[i].second);


							//���ҵ��ؼ��ֵĺ���child��ؼ������ֵܵĺ���lbchild�ϲ���,���ùؼ���ǰ��,ʹ��ǰ�ڵ�Ĺؼ��ּ���һ��
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
					else		  //����x.keys[i]ֻ�����ֵ�
					{
						BPlusNode rbchild;
						ReadBPlusNode(x.pointers[i + 1].second, rbchild);

						if (rbchild.nkey > MAX_KEY / 2) {	  //���K

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
						else {		//���L

							child.keys[child.nkey] = x.keys[i];
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++) {		//��rbchild�ڵ�ϲ���child�ڵ��
								child.keys[child.nkey + j] = rbchild.keys[j];
								child.pointers[child.nkey + j] = rbchild.pointers[j];
							}
							child.pointers[child.nkey + j] = rbchild.pointers[j];

							child.nkey += rbchild.nkey;

							//�ͷ�rbchild�ڵ���ռ�õĿռ�,x.pointers[i+1]
							//����ɾ��
							DeleteBplusNode(x.pointers[i + 1].second);

							for (j = i; j < x.nkey - 1; j++) {	  //��ǰ���ؼ���֮��Ĺؼ�������һλ,ʹ�ýڵ�Ĺؼ���������һ
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
			else {	//�����亢�ӽڵ�����ڵ�
				if (child.nkey > MAX_KEY / 2) {  //���M
					//��������
				}
				else {		//�����ӽڵ㲻������

					if (i > 0) { //�����ֵ�

						BPlusNode lbchild;
						ReadBPlusNode(x.pointers[i - 1].second, lbchild);

						if (lbchild.nkey > MAX_KEY / 2) {	//���N
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
						else {		//���O

							for (j = 0; j < child.nkey; j++) {		//�����ֵܺ��ӽڵ�ϲ�	
								lbchild.keys[lbchild.nkey + j] = child.keys[j];
								lbchild.pointers[lbchild.nkey + j] = child.pointers[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.pointers[MAX_KEY] = child.pointers[MAX_KEY];

							//�ͷ�childռ�õĿռ�x.pointers[i]
							//����ɾ��
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
					else {		  //����ֻ�����ֵ�

						BPlusNode rbchild;
						ReadBPlusNode(x.pointers[i + 1].second, rbchild);

						if (rbchild.nkey > MAX_KEY / 2) {		//���P

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
						else		//���Q
						{
							for (j = 0; j < rbchild.nkey; j++) {
								child.keys[child.nkey + j] = rbchild.keys[j];
								child.pointers[child.nkey + j] = rbchild.pointers[j];
							}
							child.nkey += rbchild.nkey;
							child.pointers[MAX_KEY] = rbchild.pointers[MAX_KEY];

							//�ͷ�rbchildռ�õĿռ�x.pointers[i+1]
							//����ɾ��
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



void	BPlusTree::Delete_BPlus_Tree(TRecord &trecord)	//��B+��ɾ��һ���ؼ���
{
	delete_BPlus_tree(ROOT, trecord);

	BPlusNode rootnode;
	ReadBPlusNode(ROOT, rootnode);

	if (!rootnode.isleaf && rootnode.nkey == 0)	  //���ɾ���ؼ��ֺ���ڵ㲻��Ҷ�ڵ㣬���ҹؼ�������Ϊ0ʱ���ڵ�ҲӦ�ñ�ɾ��
	{
		//�ͷ�ROOT�ڵ�ռ�õĿռ�
		//����ɾ��
		DeleteBplusNode(ROOT);

		ROOT = rootnode.pointers[0].second;			//���ڵ�����,B+���߶ȼ�1

		update_mete_tree();

	}

}



void BPlusTree::Delete_BPlus_Tree_Address_Under_Key(TRecord & trecord)//ɾ��ָ���ؼ����µ�ָ����¼��ַ����trecord�а������ҵĹؼ��ֺ�Ҫɾ���ļ�¼��ַ
{

	TSearchResult tresult;
	Search_BPlus_Tree(trecord, tresult);
	if (!tresult.exist) {
		//cout << "���󣡣��������ܳ��������Ϣ�ġ�������������޴˹ؼ��ֵ��޴��ļ���ַ" << endl;
		//system("pause");
	}
	else if (!tresult.ismul) {//ֱ��ɾ����¼ָ��
		if (trecord.Raddress != tresult.Raddress) {
			//cout << "���󣡣��������ܳ��ִ���Ϣ������������ǹؼ��ֺ��ļ���ַ����" << endl;
			//system("pause");
		}
		Delete_BPlus_Tree(trecord);
		
	}
	else {//ָ���Ƕ����¼������ָ�룬����ɾ��������¼�Ϳ�����
		int count = DeleteSingleRaddressIndex(tresult.Raddress, trecord.Raddress);//ɾ�������еĵ�����¼��ַ

		if (count == 0) {
			//cout << "���󣡣��������ܳ��������Ϣ�ġ�������������д˹ؼ��֣��д��������������ͱ������0�ˣ������ܵ�" << endl;
			//system("pause");
		}
		else if (count == 1) {//��count==1ʱ����������ʧȥ��ɾ������
			TRecord temprecord;
			GetTheOnlyRaddressIndex(tresult.Raddress, temprecord);//�õ����ļ�¼��ַ
			temprecord.key = trecord.key;
			Delete_BPlus_Tree(trecord);//ɾ��ԭ���е�ָ�루���ǰ��Ǹ�����ɾ����
			Insert_BPlus_Tree(temprecord);
		}
		else {//����count������0
		}
	}
}




void  BPlusTree::EnumLeafKey()	//����ö��B+��Ҷ�ڵ�����йؼ���
{
	BPlusNode head;

	ReadBPlusNode(ROOT, head);

	while (!head.isleaf)
	{
		ReadBPlusNode(head.pointers[0].second, head);
	}

	while (1)
	{
		for (int i = 0; i < head.nkey; i++)
			printf("%f\n", head.keys[i]);

		if (head.pointers[MAX_KEY].second == 0)//û����һ��Ҷ�ڵ�
			break;

		ReadBPlusNode(head.pointers[MAX_KEY].second, head);
	}

}




//��ȡһ�����
inline void	BPlusTree::ReadBPlusNode(FILEP address, BPlusNode	&n)
{
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\tree%d.dat", tablename.c_str(), keyname.c_str(), address);
	ifstream in_file(str, ios::in | ios::binary);
	free(str);
	//��in_file�ļ�д��BPlusnode n��
	in_file.read((char*)&n, sizeof(n));
	n;
	in_file.close();

}

//д��һ�����
inline void	BPlusTree::WriteBPlusNode(FILEP address, BPlusNode &n) {
	char *str;
	str = (char*)malloc(sizeof(char)* BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\tree%d.dat", tablename.c_str(), keyname.c_str(), address);
	ofstream out_file(str, ios::out | ios::trunc | ios::binary);
	free(str);
	//��BPlusnode nд��out_file�ļ�
	out_file.write((char*)&n, sizeof(n));
	out_file.close();

}

inline void BPlusTree::DeleteBplusNode(FILEP address) {
	char *str;
	str = (char*)malloc(sizeof(char)* BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\tree%d.dat", tablename.c_str(), keyname.c_str(), address);
	int err = remove(str);
	if (err == -1)
		cout << "ɾ�������ʧ��";
}

//int BPlusTree::GetAddressIndexCount(FILEP INID)
//{
//	char *str;
//	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
//	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), tresult.Raddress);
//	ifstream in_file(str, ios::in | ios::binary);
//	free(str);
//
//	FILEP _point = 0;
//	while (1) {
//		in_file.read((char*)&_point, sizeof(_point));
//		if (in_file.eof()) break;//������ĩβʱ�ͽ���
//		points_to.push_back(_point);
//
//	}
//
//	in_file.close();
//}

void BPlusTree::GetTheOnlyRaddressIndex(FILEP INDID_ADDRESS, TRecord & trecord)
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

inline void BPlusTree::WriteRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress) {
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), INDID_ADDRESS);
	ofstream out_file(str, ios::out | ios::binary | ios::app);
	free(str);
	//��Raddress׷��д��������
	out_file.write((char*)&raddress, sizeof(raddress));
	out_file.close();
}

int BPlusTree::DeleteSingleRaddressIndex(FILEP INDID_ADDRESS, FILEP raddress)
{
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), INDID_ADDRESS);
	fstream f;
	f.open(str, ios::in | ios::binary);
	vector<FILEP> a;
	FILEP s;
	while (1) {
		f.read((char*)&s, sizeof(s));
		if (f.eof()) break;//������ĩβʱ�ͽ���
		if(s != raddress)
			a.push_back(s);//���·�ɾ���ļ�¼��ַ

	}
	f.close();

	int count = 0;
	f.open(str, ios::out | ios::binary | ios::trunc);
	for (int i = 0; i < a.size(); i++) {
		f.write((char*)&a[i], sizeof(FILEP));
		count++;
	}
	f.close();
	return count;
}

inline void BPlusTree::DeleteRaddressIndex(FILEP INDID_ADDRESS) {
	char *str;
	str = (char *)malloc(sizeof(char) * BPTREE_MAX_FILE_PATH_SIZE);
	sprintf(str, "table\\%s\\index\\bptree_%s\\indid%d.dat", tablename.c_str(), keyname.c_str(), INDID_ADDRESS);
	int err = remove(str);
	if (err == -1)
		cout << "ɾ�������¼��ַ������ʧ��";
}


