#pragma once
#include "headerer.h"
using namespace std;

namespace blue_base{

	/*class BlueString {
	public:
		static const int maxlen = 100;
		char s[maxlen];
		BlueString() {
			memset(s, 0, sizeof(s));
		}
		BlueString(const char *ss) {
			set(ss);
		}
		BlueString(double a) {
			sprintf_s(s, "%f", a);
		}
		void operator=(const char* ss) {
			set(ss);
		}
		friend ostream & operator << (ostream &out, BlueString & obj) {
			out << obj.s;
			return out;
		}
		friend istream & operator >> (istream &in, BlueString & obj) {
			in >> obj.s;
			return in;
		}
		void set(const char *ss) {
			for (int i = 0; i < maxlen; i++) {
				s[i] = ss[i];
				if (ss[i] == '\0')return;
			}
		}
	};
*/

	//inline bool is_alpha(char cc);

	//inline bool is_num_bit(char cc);

	//inline bool is_num(string str, int &num);

	//inline bool is_string_const(string str);

	//inline bool is_keep_word(string str);

	//inline bool is_legal_user_id(string str);

	//inline vector<string> space_extraction(string str); 


	////0 /= ;1 / <; 2 / >; 3 / <= ; 4 / >= ; 5 / <>;
	//inline int get_op_id(string str);


	//inline int checkpoint(char ch);
	
	struct table_attr {			//����������'*'�Ļ���attrnameΪ��
		string tablename;
		string attrname;
		table_attr(){}
		table_attr(string a, string b);
		int parse(string str);//���ַ���������table��attr����ʽ��ʧ���򷵻�false;
		bool operator==(const table_attr b)const;
	};


	class Base_attribute_sega {
	public:
		string name;
		int type;					//1 double | 2 char
		int key;				//0 normal 1 primary 2 references 3 primary&references
		table_attr outer_code;
		int parse(string str);
	
	};


	struct error {
		int error_type;
		error() { error_type = 0; }
		error(int i){ error_type = i; }
		string geterror();
	};

	/*
	//����Ƕ�ײ�ѯ�ı��ʽ���������㲻��������
	struct easy_expression{
		int type;	//0.attr op attr 1.attr op num 2.attr op str
		table_attr lef_attr;
		int op;					// 0/=;	1/<; 2/>; 3/<=; 4/>=; 5/<>
		table_attr rig_attr;
		int nval;		//number
		string sval;		//string_const
		int parse(string str);//δ���
	};


	struct EASY_QUERY{
		vector<table_attr> ta;
		vector<string> tablename;
		vector<easy_expression> cd;
	};
	*/
	struct expression {
		int type;	//0.attr op attr 1.attr op num 2.attr op str 
		table_attr lef_attr;
		int op;					// 0/=;	1/<; 2/>; 3/<=; 4/>=; 5/<> 
		table_attr rig_attr;
		double nval;		//number
		string sval;		//string_const
		int parse(string str);
	};

	


	//��ѯ���������RESULT��
	struct RESULT{
		vector<table_attr> attr_vec;
		vector<vector<void *> > data_vec;//ʲô�������
		vector<int> id;
		vector<int> rec_addr;//ֻ��¼����ߵĵ�ַ
		RESULT(){}
		RESULT(string tablename,vector<FILEP> points_to);//����ָ�뼯�϶�ȡ������________________ʵ������
		void clear();
		void init(string tablename, vector<FILEP> points_to);
		//RESULT(string tablename, expression cd);
		RESULT Projection(vector<table_attr> ta);//ͶӰ
		RESULT Select(expression cd);//ѡ��
		void ReNumber();//���±��id
		//RESULT Cartesian_Product(RESULT A, RESULT B);//�ѿ�����
		void print();
	};
	

	struct MID_OP{
		static error mid_create_table(string tablename, vector<Base_attribute_sega> baseattr);	//chartype�ַ������ԣ�doubletype����������
		static error mid_create_index(table_attr ta);
		//------------drop--------------
		static error mid_drop_table(string tablename);
		static error mid_drop_index(table_attr ta);
		//------------insert-------------//Ҫ��Ͳ���ȫ�ģ�
		static error mid_insert(vector<string>tablename, vector<expression>cd);//charattr����ַ���������ֵ��doubleattr��Ÿ�����������ֵ


		//����������Ҳ�Ǵ��
		static error mid_delete_data(vector<string>tablename, vector<expression> cd);

		//-----------update------------�ٶ��ַ���ֻ�ܸ�ֵ������+-*/һ���Ǹ���������,����ֻ����+-*/

		//expression �� condition �ϲ���

		static error mid_update_data(vector<string>tablename, vector<expression> exp, vector<expression> cd);
		//-----------select-------------�ٶ��ַ���ֻ�ܸ�ֵ
		static RESULT pre_table_select_attr( vector<expression> cd);

		static error mid_select_attr(vector<table_attr> ta, vector<string> tablename, vector<expression> cd);

		//static RESULT mid_query_middle(vector<table_attr> &ta, vector<string> &tbn, vector<string> &word, vector<int> jmp, int lef, int rig);
		static error mid_query_outer(vector<table_attr> ta, vector<string> tbn, vector<string> word, RESULT & return_ret);
		
		static RESULT  mid_query_middle(vector<string> &tbn, vector<string> &word, vector<int> jmp, int lef, int rig);
		
		static RESULT brute_force_union(RESULT A, RESULT B,bool &f);
		
		static RESULT brute_force_intersect(RESULT A, RESULT B,bool &f);

		static error mid_query_data_dic(string tbn);
	};

}