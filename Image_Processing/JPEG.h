#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class CJPEG
{

public:
	//ֻ�ǰѲ�ѯ�����������װ���ˣ�����diff������Ϣ�����ж�ϵ����С��ʵ��
	typedef struct {
		int zLen; //ǰ���0�г̳���
		int SSSS; //ǰ׺��
		int value; //�Լ���ֵ
	}symbol;

	typedef struct 
	{
		int SSSS;
		int len;
		string code;
	}DCTableItem;

	typedef struct
	{
		int zLen;
		int SSSS;
		int len;
		string code;
	}ACTableItem;


public:
	CJPEG();
	~CJPEG();
	void ZigZag(double *pBlockData, double *pZigZag, int nBlockSize);
	void IZigZag(double * pBlockData, double * pZigZag, int nBlockSize);
	vector<symbol> getLuminSymbolSequence(double *pZigZag, int nBlockSize, int preDC);
	vector<string> getLuminCodesBySymbolSequence(vector<symbol> &symbols);
	string searchForLuminCode(int zlen, int SSSS);
	string searchForChrominCode(int zlen, int SSSS);
	string getBinaryCode(int value);
	int getNumByBinaryCode(string s);
	vector<string> getChrominCodesBySymbolSequence(vector<symbol>& symbols);
	void decodeLuminByCodes(vector<string> &codes, double *pBlockData, int nBlockSize, int preDC);
	void decodeChrominByCodes(vector<string> &codes, double * pBlockData, int nBlockSize, int preDC);
	bool loadCodeTable();
	int getLuminDCSSSS(string code);
	int getChrominDCSSSS(string code);
	void getLuminACSymbolByTable(string &code, int& zlen, int & ssss);
	void getChrominACSymbolByTable(string & code, int & zlen, int & ssss);
	vector<DCTableItem> m_TableluminDC;
	vector<ACTableItem> m_TableluminAC;
	vector<DCTableItem> m_TablechrominDC;
	vector<ACTableItem> m_TablechrominAC;
};

