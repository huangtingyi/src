// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "wwtiny.h"
#include "inifile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile()
{
	m_sFileName = "ini.ini";
}

CIniFile::~CIniFile()
{
	ClearFileContainer();
}
void CIniFile::ClearFileContainer()
{
/*	for (auto it = m_vecFileContainer.begin(); it != m_vecFileContainer.end(); it++) {
		if (*it != NULL) {
			delete *it;
			*it = NULL;
		}
	}
*/
	if (m_vecFileContainer.size() > 0)
		m_vecFileContainer.clear();

}
void CIniFile::SetFileName(string FileName)
{
	m_sFileName = FileName;
	ReadIniFile();
}

void CIniFile::SetInt(string Section, string Item, int Value)
{
	string strtemp= to_string(Value);

	//保存到FileContainer
	WriteFileString(Section, Item, strtemp);
	//保存到ini文件
	WriteIniFile();
}

void CIniFile::SetString(string Section, string Item, string Value)
{
	//保存到FileContainer
	WriteFileString(Section, Item, Value);
	//保存到ini文件
	WriteIniFile();
}

string CIniFile::GetString(string Section, string Item, string Value)
{
	return GetFileString(Section, Item, Value);
}

int CIniFile::GetInt(string Section, string Item, int Value)
{
	string strtemp= to_string(Value);

	return atoi(GetFileString(Section, Item, strtemp).c_str());
}

bool CIniFile::ReadIniFile()
{
	char	sLine[512];

	ifstream f(m_sFileName);

	bFileExist = true;
	if (!f.is_open()){
		printf("Error opening file %s for read\n", m_sFileName.c_str());
		bFileExist = false;
		return false;
	}

	ClearFileContainer();

	//将IniFile文件数据读到FileContainer
	while (!f.eof()){

		f.getline(sLine, sizeof(sLine));

		m_vecFileContainer.push_back(string(sLine));
	}

	f.close();

	return true;
}

bool CIniFile::WriteIniFile()
{
	ofstream f(m_sFileName);

	bFileExist = true;

	if (!f.is_open()) {
		printf("Error opening file %s for write\n",m_sFileName.c_str());
		bFileExist = false;
		return false;
	}

	//将FileContainer写到IniFile文件
	for(int i = 0; i< (int)m_vecFileContainer.size(); i++){

		f << m_vecFileContainer[i] << endl;
	}

	f.close();

	return true;
}

string CIniFile::GetFileString(string Section, string Item, string Value)
{
	ReadIniFile();//打开文件


	if(bFileExist == false || m_vecFileContainer.size() <= 0)
		return Value;//文件打开出错或文件为空，返回默认值

	int i = 0;
	int iFileLines = (int)m_vecFileContainer.size();
	string strline,str;

	i = 0;
	while(i<iFileLines){

		//删除行首空格
		strline=TrimLeft(m_vecFileContainer[i++]);

		if(strline.substr(0,1)=="["){//查找Section，第一个必须为[

			//拿到项目,删除左右空格
			str = TrimAll(strline.substr(1,strline.find_first_of("]")-1));

			if(Section == str){//找到Section

				while(i<iFileLines){

					strline = TrimLeft(m_vecFileContainer[i++]);

					if(strline.substr(0, 1) == "[")
						return Value;//如果到达下一个[]，即找不到,返回默认值

					//拿到键值
					str = TrimAll(strline.substr(0, strline.find_first_of("=")));

					if(Item == str){//找到Item
						str=TrimAll(strline.substr(strline.find_first_of("=")+1));//去掉=左边;
						return str;
					}
				}
				return Value;//找不到,返回默认值
			}

		}

	}
	return Value;//找不到,返回默认值
}

void CIniFile::WriteFileString(string Section, string Item, string Value)
{
	int i = 0;
	int iFileLines = (int)m_vecFileContainer.size();
	string strline,str;

	while(i<iFileLines){

		strline = TrimLeft(m_vecFileContainer[i++]);

		if(strline.substr(0, 1) == "["){//查找Section，第一个必须为[

			str = TrimAll(strline.substr(1, strline.find_first_of("]") - 1));

			if(Section == str){//找到Section
			
				while(i<iFileLines){

					strline = TrimLeft(m_vecFileContainer[i++]);

					//如果到达下一个[]，即找不到Item
					if (strline.substr(0, 1) == "[") break;
					
					str = TrimAll(strline.substr(0, strline.find_first_of("=")));

					if(Item == str){//找到Item

						strline = Item + "=" + Value;
						//直接覆盖
						m_vecFileContainer[i-1] = strline;
						return;
					}
				}
				//找不到Item，在指定位置插入
				strline = Item + "=" + Value;
				m_vecFileContainer.insert(m_vecFileContainer.begin()+i-1, strline);
				return;

			}

		}

	}
	//找不到Section
	//直接在最后加入Section，Item，Value
	m_vecFileContainer.push_back("[" + Section + "]");
	m_vecFileContainer.push_back(Item + "=" + Value);
}
