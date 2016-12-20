
#include "stdafx.h"
#include "GenotypeParser.h"

namespace genotype
{
	static const char *g_numStr = "1234567890.";
	static const int g_numLen = 11;
	static const char *g_strStr = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@$_/.";
	static const int g_strLen = 57;
	static const char *g_strStr2 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@$_/.1234567890";
	static const int g_strLen2 = 67;
	static const char *g_strOp = "+-*/=:()";
	static const int g_opLen = 7;
}

using namespace genotype;


cParser::cParser()
: m_isReverseScan(false)
, m_isError(false)
, m_lineNum(0)
{
}

cParser::~cParser()
{
	Clear();
}


bool cParser::Read(const string &fileName)
{
	Clear();

	using namespace std;
	m_ifs.open(fileName);
	RETV(!m_ifs.is_open(), false);

	while (ScanLine())
	{
		string name = id();
		if (name.empty())
			continue;

		const char c = op();
		if (c == ':') // tree head
		{
			if (sParseTree *tree = ParseAttributes())
			{
				tree->name = name;
				m_parseTrees.push_back(tree);
			}
		}
	}

	Build();

	m_ifs.close();
	return true;
}


void cParser::Clear()
{
	m_isReverseScan = false;
	m_isError = false;
	m_lineNum = 0;

	for (auto tree : m_parseTrees)
		delete tree;
	m_parseTrees.clear();

	for (auto kv : m_bodies)
	{
		for (auto j : kv.second->joints)
			delete j;
		delete kv.second;
	}
	m_bodies.clear();
}


sParseTree* cParser::ParseAttributes()
{
	RETV(!m_lineStr, NULL);

	sParseTree *tree = new sParseTree;

	while (1)
	{
		attr_list(tree);

		if (!ScanLine())
			break;

		// check new tree node or attributes
		const string name = id();
		if (name.empty())
			continue;
		const char c = op();
		if (c == ':')
		{
			// new tree node
			UnScanLine();
			break;
		}
		else
		{
			// continue attribute parsing
			m_lineStr = m_scanLine;
		}
	}

	return tree;
}


void cParser::Build() 
{
	RET(m_parseTrees.empty());

	sParseTree *mainNode = NULL;
	for (auto &tree : m_parseTrees)
	{
		if (tree->name == "main")
		{
			mainNode = tree;
			break;
		}
	}

	RET(!mainNode);

	sBody *body = NULL;
	for (auto &tree : m_parseTrees)
	{
		if (body)
		{
			if (tree->name == "joint")
			{
				if (sJoint *j = BuildJoint(tree))
					body->joints.push_back(j);
			}
			else
			{
				body = BuildBody(tree);
				m_bodies[body->name] = body;
			}
		}
		else
		{
			if (tree->name == "joint")
			{
				// error occur
				// not create body
			}
			else
			{
				body = BuildBody(tree);
				m_bodies[body->name] = body;
			}
		}
	}

	// Build Joint Link
	for (auto kv : m_bodies)
	{
		if (!kv.second)
			continue;
		for (auto joint : kv.second->joints)
			joint->link = m_bodies[joint->linkName];
	}

	m_root = m_bodies["main"];
}


sBody* cParser::BuildBody(sParseTree *tree)
{
	RETV(!tree, NULL);
	sBody *body = new sBody;
	body->name = tree->name;

	if (tree->attrs["type"] == "dynamic")
		body->type = eBodyType::DYNAMIC;
	else if (tree->attrs["type"] == "kinematic")
		body->type = eBodyType::KINEMATIC;
	else
		body->type = eBodyType::DYNAMIC;

	if (tree->attrs["shape"] == "box")
		body->shape = eShapeType::BOX;
	else if (tree->attrs["shape"] == "sphere")
		body->shape = eShapeType::SPHERE;
	else
		body->shape = eShapeType::BOX;

	body->dim = String2Vector3(tree->attrs["dim"]);
	body->mtrl = String2Vector3(tree->attrs["material"]);
	body->mass = (float)atof(tree->attrs["mass"].c_str());
	body->depth = atoi(tree->attrs["depth"].c_str());
	return body;
}


sJoint* cParser::BuildJoint(sParseTree *tree)
{
	RETV(!tree, NULL);
	sJoint *joint = new sJoint;

	if (tree->attrs["type"] == "none")
		joint->type = eJointType::NONE;
	else if (tree->attrs["type"] == "fixed")
		joint->type = eJointType::FIXED;
	else if (tree->attrs["type"] == "revolute")
		joint->type = eJointType::REVOLUTE;
	else if (tree->attrs["type"] == "spherical")
		joint->type = eJointType::SPHERICAL;
	else
		joint->type = eJointType::NONE;

	joint->pos = String2Vector3(tree->attrs["pos"]);
	joint->rot = String2Vector4(tree->attrs["rot"]);
	joint->limit = String2Vector3(tree->attrs["limit"]);
	joint->velocity = (float)atof(tree->attrs["velocity"].c_str());
	joint->period = (float)atof(tree->attrs["period"].c_str());
	joint->linkName = tree->attrs["link"];
	return joint;
}


Vector3 cParser::String2Vector3(const string &str) 
{
	vector<string> out;
	out.reserve(4);
	tokenizer2(str, ",", out);

	Vector3 v(0,0,0);
	if (out.size() > 0)
		v.x = (float)atof(out[0].c_str());
	if (out.size() > 1)
		v.y = (float)atof(out[1].c_str());
	if (out.size() > 2)
		v.z = (float)atof(out[2].c_str());

	return v;
}


Vector4 cParser::String2Vector4(const string &str)
{
	vector<string> out;
	out.reserve(4);
	tokenizer2(str, ",", out);

	Vector4 v(0, 0, 0, 0);
	if (out.size() > 0)
		v.x = (float)atof(out[0].c_str());
	if (out.size() > 1)
		v.y = (float)atof(out[1].c_str());
	if (out.size() > 2)
		v.z = (float)atof(out[2].c_str());
	if (out.size() > 3)
		v.w = (float)atof(out[3].c_str());

	return v;
}



bool cParser::IsBlank(const char *str, const int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (!*str)
			break;
		if (('\n' == *str) || ('\r' == *str) || ('\t' == *str) || (' ' == *str))
			++str;
		else
			return false;
	}
	return true;
}


// pass through blank character
char* cParser::passBlank(const char *str)
{
	RETV(!str, NULL);

	while (1)
	{
		if (!*str)
			return NULL;
		if ((*str == ' ') || (*str == '\t'))
			++str;
		else
			break;
	}
	return (char*)str;
}

// id -> alphabet + {alphabet | number}
const char* cParser::id()
{
	// comma check
	m_lineStr = passBlank(m_lineStr);
	if (!m_lineStr)
		return "";

	// 쌍따옴표로 id를 설정할 때
	if (*m_lineStr == '\"')
	{
		++m_lineStr;

		int cnt = 0;
		char *dst = m_tmpBuffer;
		while ((*m_lineStr != '\"') && (cnt < 256) && (*m_lineStr))
		{
			*dst++ = *m_lineStr++;
			++cnt;
		}
		*dst = NULL;
		++m_lineStr;
		return m_tmpBuffer;
	}

	// find first char
	const char *n = strchr(g_strStr, *m_lineStr);
	if (!n)
		return "";

	char *dst = m_tmpBuffer;
	*dst++ = *m_lineStr++;

	while (1)
	{
		const char *n = strchr(g_strStr2, *m_lineStr);
		if (!n || !*n)
			break;
		*dst++ = *m_lineStr++;
	}

	*dst = NULL;
	return m_tmpBuffer;
}

const char* cParser::number()
{
	// comma check
	m_lineStr = passBlank(m_lineStr);
	if (!m_lineStr)
		return "";

	char *dst = m_tmpBuffer;
	if ('+' == *m_lineStr)
		*dst++ = *m_lineStr++;
	else if ('-' == *m_lineStr)
		*dst++ = *m_lineStr++;

	while (1)
	{
		const char *n = strchr(g_numStr, *m_lineStr);
		if (!n || !*n)
			break;
		*dst++ = *m_lineStr++;
	}

	*dst = NULL;
	return m_tmpBuffer;
}

char cParser::op()
{
	m_lineStr = passBlank(m_lineStr);
	if (!m_lineStr)
		return NULL;
	const char *n = strchr(g_strOp, *m_lineStr);
	if (!n)
		return NULL;
	return *m_lineStr++;
}

char cParser::comma()
{
	m_lineStr = passBlank(m_lineStr);
	if (!m_lineStr)
		return NULL;
	const char *n = strchr(",", *m_lineStr);
	if (!n)
		return NULL;
	return *m_lineStr++;
}

bool cParser::match(const char c)
{
	m_lineStr = passBlank(m_lineStr);
	if (!m_lineStr)
		return NULL;
	if (c == *m_lineStr)
	{
		m_lineStr++;
		return true;
	}
	return false;
}



// 한 라인을 읽는다.
// 첫 번째 문자가 # 이면 주석 처리된다. 
// 문자열 중간에 # 이 있을 때는 처리 하지 않는다.
bool cParser::ScanLine()
{
	if (m_isReverseScan)
	{
		--m_lineNum;
		m_lineStr = m_scanLine;
		m_isReverseScan = false;
		return true;
	}

	do
	{
		++m_lineNum;
		if (!m_ifs.getline(m_scanLine, sizeof(m_scanLine)))
			return false;
	} while (IsBlank(m_scanLine, sizeof(m_scanLine)) || (m_scanLine[0] == '#'));

	m_lineStr = m_scanLine;
	return true;
}


// 읽는 위치를 한 라인위로 옮긴다.
void cParser::UnScanLine()
{
	m_isReverseScan = true;
}



// aaa, bb, cc, "dd ee"  ff -> aaa, bb, cc, dd ee
int cParser::attrs(const string &str, OUT string &out)
{
	int i = 0;

	out.reserve(64);

	bool isLoop = true;
	bool isComma = false;
	bool isString = false;
	bool isFirst = true;
	while (isLoop && str[i])
	{
		switch (str[i])
		{
		case '"':
			isFirst = false;
			isString = !isString;
			break;

		case ',': // comma
			if (isString)
			{
				out += ',';
			}
			else
			{
				isComma = true;
				out += ',';
			}
			break;

		case '\r':
		case '\n':
		case ' ': // space
			if (isFirst)
			{
				// nothing~
			}
			else if (isString)
			{
				out += str[i];
			}
			else
			{
				if (!isComma)
					isLoop = false;
			}
			break;

		default:
			isFirst = false;
			isComma = false;
			out += str[i];
			break;
		}
		++i;
	}

	return i;
}


// attr - list ->  { id=value  }
void cParser::attr_list(sParseTree *current)
{
	while (1)
	{
		const char *pid = id();
		if (!*pid)
			break;

		const string key = pid;
		if (!match('='))
		{
			if (!key.empty())
				current->attrs["id"] = parse_attrs_symbol(key);
			continue;
		}

		// aaa, bb, cc, "dd ee"  ff -> aaa,bb,cc,dd ee
		string data;
		m_lineStr += attrs(m_lineStr, data);

		current->attrs[key] = parse_attrs_symbol(data);
	}
}


// ex) aaa; bbb ccc ddd; eee
//		out : aaa,bbb,ccc,ddd,eee
string cParser::parse_attrs_symbol(const string &values)
{
	vector<string> out;
	tokenizer2(values, ",", out);

	string retVar;
	for each (auto str in out)
	{
		retVar += str + ",";
	}

	if (retVar.back() == ',')
		retVar.pop_back();

	return retVar;
}
