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
	
	struct table_attr {			//属性名或者'*'的话，attrname为空
		string tablename;
		string attrname;
		table_attr(){}
		table_attr(string a, string b);
		int parse(string str);//将字符串解析成table和attr的形式，失败则返回false;
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
	//不能嵌套查询的表达式————搞不动不搞了
	struct easy_expression{
		int type;	//0.attr op attr 1.attr op num 2.attr op str
		table_attr lef_attr;
		int op;					// 0/=;	1/<; 2/>; 3/<=; 4/>=; 5/<>
		table_attr rig_attr;
		int nval;		//number
		string sval;		//string_const
		int parse(string str);//未完成
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

	


	//查询结果保存在RESULT中
	struct RESULT{
		vector<table_attr> attr_vec;
		vector<vector<void *> > data_vec;//什么鬼畜操作
		vector<int> id;
		vector<int> rec_addr;//只记录最左边的地址
		RESULT(){}
		RESULT(string tablename,vector<FILEP> points_to);//根据指针集合读取出数据________________实现了吗？
		void clear();
		void init(string tablename, vector<FILEP> points_to);
		//RESULT(string tablename, expression cd);
		RESULT Projection(vector<table_attr> ta);//投影
		RESULT Select(expression cd);//选择
		void ReNumber();//重新编号id
		//RESULT Cartesian_Product(RESULT A, RESULT B);//笛卡尔积
		void print();
	};
	

	struct MID_OP{
		static error mid_create_table(string tablename, vector<Base_attribute_sega> baseattr);	//chartype字符串属性，doubletype浮点数属性
		static error mid_create_index(table_attr ta);
		//------------drop--------------
		static error mid_drop_table(string tablename);
		static error mid_drop_index(table_attr ta);
		//------------insert-------------//要插就插完全的？
		static error mid_insert(vector<string>tablename, vector<expression>cd);//charattr存放字符串的属性值，doubleattr存放浮点数的属性值


		//传入多个表名也是错的
		static error mid_delete_data(vector<string>tablename, vector<expression> cd);

		//-----------update------------假定字符串只能赋值，出现+-*/一定是浮点数类型,并且只能自+-*/

		//expression 和 condition 合并了

		static error mid_update_data(vector<string>tablename, vector<expression> exp, vector<expression> cd);
		//-----------select-------------假定字符串只能赋值
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