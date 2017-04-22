#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class CJPEG
{

public:
	//只是把查询霍夫曼的码表装载了，根据diff查表的信息程序判断系数大小来实现
	typedef struct {
		int zLen; //前面的0行程长度
		int SSSS; //前缀码
		int value; //自己的值
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

