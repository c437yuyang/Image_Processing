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

