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

	//���浽FileContainer
	WriteFileString(Section, Item, strtemp);
	//���浽ini�ļ�
	WriteIniFile();
}

void CIniFile::SetString(string Section, string Item, string Value)
{
	//���浽FileContainer
	WriteFileString(Section, Item, Value);
	//���浽ini�ļ�
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

	//��IniFile�ļ����ݶ���FileContainer
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

	//��FileContainerд��IniFile�ļ�
	for(int i = 0; i< (int)m_vecFileContainer.size(); i++){

		f << m_vecFileContainer[i] << endl;
	}

	f.close();

	return true;
}

string CIniFile::GetFileString(string Section, string Item, string Value)
{
	ReadIniFile();//���ļ�


	if(bFileExist == false || m_vecFileContainer.size() <= 0)
		return Value;//�ļ��򿪳�����ļ�Ϊ�գ�����Ĭ��ֵ

	int i = 0;
	int iFileLines = (int)m_vecFileContainer.size();
	string strline,str;

	i = 0;
	while(i<iFileLines){

		//ɾ�����׿ո�
		strline=TrimLeft(m_vecFileContainer[i++]);

		if(strline.substr(0,1)=="["){//����Section����һ������Ϊ[

			//�õ���Ŀ,ɾ�����ҿո�
			str = TrimAll(strline.substr(1,strline.find_first_of("]")-1));

			if(Section == str){//�ҵ�Section

				while(i<iFileLines){

					strline = TrimLeft(m_vecFileContainer[i++]);

					if(strline.substr(0, 1) == "[")
						return Value;//���������һ��[]�����Ҳ���,����Ĭ��ֵ

					//�õ���ֵ
					str = TrimAll(strline.substr(0, strline.find_first_of("=")));

					if(Item == str){//�ҵ�Item
						str=TrimAll(strline.substr(strline.find_first_of("=")+1));//ȥ��=���;
						return str;
					}
				}
				return Value;//�Ҳ���,����Ĭ��ֵ
			}

		}

	}
	return Value;//�Ҳ���,����Ĭ��ֵ
}

void CIniFile::WriteFileString(string Section, string Item, string Value)
{
	int i = 0;
	int iFileLines = (int)m_vecFileContainer.size();
	string strline,str;

	while(i<iFileLines){

		strline = TrimLeft(m_vecFileContainer[i++]);

		if(strline.substr(0, 1) == "["){//����Section����һ������Ϊ[

			str = TrimAll(strline.substr(1, strline.find_first_of("]") - 1));

			if(Section == str){//�ҵ�Section
			
				while(i<iFileLines){

					strline = TrimLeft(m_vecFileContainer[i++]);

					//���������һ��[]�����Ҳ���Item
					if (strline.substr(0, 1) == "[") break;
					
					str = TrimAll(strline.substr(0, strline.find_first_of("=")));

					if(Item == str){//�ҵ�Item

						strline = Item + "=" + Value;
						//ֱ�Ӹ���
						m_vecFileContainer[i-1] = strline;
						return;
					}
				}
				//�Ҳ���Item����ָ��λ�ò���
				strline = Item + "=" + Value;
				m_vecFileContainer.insert(m_vecFileContainer.begin()+i-1, strline);
				return;

			}

		}

	}
	//�Ҳ���Section
	//ֱ����������Section��Item��Value
	m_vecFileContainer.push_back("[" + Section + "]");
	m_vecFileContainer.push_back(Item + "=" + Value);
}
