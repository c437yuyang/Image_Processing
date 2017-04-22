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
	vector<symbol> getLuminSymbolSequence(double *pZigZag, int nBlockSize, int preDC);
	vector<string> getCodesBySymbolSequence(vector<symbol> &symbols);
	string searchForCode(int zlen, int SSSS);
	string getBinaryCode(int value);
	bool loadCodeTable();

	vector<DCTableItem> m_luminDCTable;
	vector<DCTableItem> m_chrominDCTable;
	vector<ACTableItem> m_luminACTable;
};

