// 2016-12-18, jjuiddong
//
// parse tree
// name: id=value, id=value ~~~ 
//
//
// build body, joint tree
// body: ~~
//		joint: ~~
//		joint: ~~
// body: ~~
//		joint: ~~
//
//
#pragma once

#include "genotype.h"


namespace genotype
{
	struct sParseTree
	{
		string name;
		map<string, string> attrs;
	};
	

	class cParser
	{
	public:
		cParser();
		virtual ~cParser();

		bool Read(const string &fileName);
		void Clear();


	protected:
		bool ScanLine();
		void UnScanLine();
		sParseTree* ParseAttributes();
		bool IsBlank(const char *str, const int size);
		char* passBlank(const char *str);
		const char* id();
		const char* number();
		char op();
		char comma();
		bool match(const char c);
		void attr_list(sParseTree *current);
		int attrs(const string &str, OUT string &out);
		string parse_attrs_symbol(const string &values);
		void Build();
		sBody* BuildBody(sParseTree *tree);
		sJoint* BuildJoint(sParseTree *tree);
		Vector3 String2Vector3(const string &str);
		Vector4 String2Vector4(const string &str);


	public:
		vector<sParseTree*> m_parseTrees;
		map<string, sBody*> m_bodies;
		sBody *m_root;

		std::ifstream m_ifs;
		char *m_lineStr;
		char m_scanLine[256];
		char m_tmpBuffer[256];
		bool m_isReverseScan; // 한 라인을 다시 읽는다.
		bool m_isError;
		int m_lineNum;
	};

}
