#include "stdafx.h"
#include "JPEG.h"


CJPEG::CJPEG()
{
	loadCodeTable();
}


CJPEG::~CJPEG()
{
}

void CJPEG::ZigZag(double * pBlockData, double * pZigZag, int nBlockSize)
{
	int index = 0;
	int N = nBlockSize;

	int squa = N * N;
	int s = 0;
	//求对应位置上应填写的值
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			s = i + j;

			if (s < N)//上三角
			{
				s = i + j;//为了看的更清楚，这里加了这条与下面对称的语句    
				index = s * (s + 1) / 2 + ((0 == (i + j) % 2) ? i : j);
				pZigZag[index] = pBlockData[i*N + j];
			}
			else//下三角
			{
				s = (N - 1 - i) + (N - 1 - j);
				index = squa - s * (s + 1) / 2 - (N - ((0 == (i + j) % 2) ? i : j));
				pZigZag[index] = pBlockData[i*N + j];
			}
		}
	}

}

bool CJPEG::loadCodeTable()
{
	//加载亮度DC系数表
	ifstream ifs("tb_lumin_dc.txt");
	DCTableItem dti;
	if (ifs.is_open())
	{
		string line;
		while (!ifs.eof())
		{
			getline(ifs, line);
			stringstream ss(line);
			ss >> dti.SSSS;
			ss >> dti.len;
			ss >> dti.code;
			m_luminDCTable.push_back(dti);
		}
	}
	else
		return false;
	ifs.close();


	//加载色度DC系数表
	ifstream ifs1("tb_chromin_dc.txt");
	if (ifs1.is_open())
	{
		string line;
		while (!ifs1.eof())
		{
			getline(ifs1, line);
			stringstream ss(line);
			ss >> dti.SSSS;
			ss >> dti.len;
			ss >> dti.code;
			m_chrominDCTable.push_back(dti);
		}
	}
	else
		return false;
	ifs1.close();


	//装载亮度信息AC系数表
	ACTableItem ati;
	ifstream ifs2("tb_lumin_ac.txt");
	if (ifs2.is_open())
	{
		string line;
		while (!ifs2.eof())
		{
			getline(ifs2, line);
			stringstream ss(line);
			ss >> ati.zLen;
			string temp;
			ss >> temp;
			if (temp == "A")
				ati.SSSS = 10;
			else
				ati.SSSS = atoi(temp.c_str());
			ss >> ati.len;
			ss >> ati.code;
			m_luminACTable.push_back(ati);
		}
	}
	else
		return false;
	ifs2.close();

	return true;
}

vector<CJPEG::symbol> CJPEG::getLuminSymbolSequence(double * pZigZag, int nBlockSize, int preDC)
{
	vector<symbol> symbols;
	symbol s;
	//先对直流系数进行编码(直流系数的zlen给1)
	int diff = pZigZag[0] - preDC;
	if (diff == 0)
	{
		s.SSSS = 0;
	}
	else if (abs(diff) == 1)
	{
		s.SSSS = 1;
	}
	else
	{
		s.SSSS = (int)floor(log2(abs(diff))) + 1;
	}
	s.value = diff;
	s.zLen = -1; //直流系数统一给-1
	symbols.push_back(s);


	//然后对交流系数进行编码
	int zLen = 0;
	int indexEnd = 0;
	for (int i=1;i!=nBlockSize*nBlockSize;++i)
	{
		if (pZigZag[i]==0)
		{
			zLen += 1;
		}
		else
		{
			if (pZigZag[i]==1 || pZigZag[i] == -1)
			{
				s.SSSS = 1;
			}
			else
			{
				s.SSSS = (int)floor(log2(abs(pZigZag[i]))) + 1;
			}
			s.zLen = zLen;
			s.value = pZigZag[i];
			symbols.push_back(s);
			zLen = 0;
			indexEnd = i;
		}
	}
	//最后有一个截止块(0,0)，截止块的value给-INT_MAX
	s.zLen = 0;
	s.SSSS = 0;
	s.value = -INT_MAX;
	symbols.push_back(s);
	//到这里交流系数就编码完成了
	return symbols;
}

vector<string> CJPEG::getLuminCodesBySymbolSequence(vector<symbol> &symbols)
{
	vector<string> codes;
	for (auto it=symbols.begin();it!=symbols.end();++it)
	{
		string s;
		if (it->zLen==-1) //表示是DC系数,DC系数查的表不一样，单独处理
		{
			s = m_luminDCTable[it->SSSS].code;
			codes.push_back(s);
			codes.push_back(getBinaryCode(it->value));
			continue;
		}

		if (it->value==-INT_MAX) //表示是截止块，单独处理
		{
			codes.push_back(searchForLuminCode(it->zLen, it->SSSS));
			continue;
		}

		//cout << searchForCode(it->zLen, it->SSSS) << endl;

		//先装入code
		codes.push_back(searchForLuminCode(it->zLen, it->SSSS));
		//再装入对应的二进制码
		codes.push_back(getBinaryCode(it->value));
	}
	return codes;
}

//根据指定的zlen和ssss查找code(当前还没做优化，查表速度其实可以优化)
string CJPEG::searchForLuminCode(int zlen,int SSSS)
{
	for (int i=0;i!=m_luminACTable.size();++i)
	{
		if (m_luminACTable[i].zLen == zlen && m_luminACTable[i].SSSS == SSSS)
		{
			return m_luminACTable[i].code;
		}
	}
	return "";
}

//取一个数的二进制编码,正数得到原码，负数得到反码,如果本身就是0，就返回0?和-1岂不是重复了(因为此时的差值为0，根据ssss=0,也可以区别-1和0)
string CJPEG::getBinaryCode(int value)
{
	string code="";
	if (value>0)
	{
		while (value!=0)
		{
			if (value%2==1)
			{
				code = "1" + code;
				//code.insert(0, "1");
			}
			else
			{
				code = "0" + code;

				//code.insert(0, "0");
			}
			value = value / 2;
		}
	}
	else if(value<0)
	{
		value = abs(value);
		while (value != 0)
		{
			if (value % 2 == 1)
			{
				code.insert(0, "0");
			}
			else
			{
				code.insert(0, "1");
			}
			value = value / 2;
		}
	}
	else
	{
		code = "0" + code;

		//code.insert(0, "0");
	}
	return code;
}




vector<CJPEG::symbol> CJPEG::getChrominSymbolSequence(double * pZigZag, int nBlockSize, int preDC)
{
	vector<symbol> symbols;
	symbol s;
	//先对直流系数进行编码(直流系数的zlen给1)
	int diff = pZigZag[0] - preDC;
	if (diff == 0)
	{
		s.SSSS = 0;
	}
	else if (abs(diff) == 1)
	{
		s.SSSS = 1;
	}
	else
	{
		s.SSSS = (int)floor(log2(abs(diff))) + 1;
	}
	s.value = diff;
	s.zLen = -1; //直流系数统一给-1
	symbols.push_back(s);


	//然后对交流系数进行编码
	int zLen = 0;
	int indexEnd = 0;
	for (int i = 1; i != nBlockSize*nBlockSize; ++i)
	{
		if (pZigZag[i] == 0)
		{
			zLen += 1;
		}
		else
		{
			if (pZigZag[i] == 1 || pZigZag[i] == -1)
			{
				s.SSSS = 1;
			}
			else
			{
				s.SSSS = (int)floor(log2(abs(pZigZag[i]))) + 1;
			}
			s.zLen = zLen;
			s.value = pZigZag[i];
			symbols.push_back(s);
			zLen = 0;
			indexEnd = i;
		}
	}
	//最后有一个截止块(0,0)，截止块的value给-INT_MAX
	s.zLen = 0;
	s.SSSS = 0;
	s.value = -INT_MAX;
	symbols.push_back(s);
	//到这里交流系数就编码完成了
	return symbols;
}

vector<string> CJPEG::getChrominCodesBySymbolSequence(vector<symbol> &symbols)
{
	vector<string> codes;
	for (auto it = symbols.begin(); it != symbols.end(); ++it)
	{
		string s;
		if (it->zLen == -1) //表示是DC系数,DC系数查的表不一样，单独处理
		{
			s = m_chrominDCTable[it->SSSS].code;
			codes.push_back(s);
			codes.push_back(getBinaryCode(it->value));
			continue;
		}

		if (it->value == -INT_MAX) //表示是截止块，单独处理
		{
			codes.push_back(searchForLuminCode(it->zLen, it->SSSS)); //没找到色差的huffman表，就先用亮度的
			continue;
		}

		//cout << searchForCode(it->zLen, it->SSSS) << endl;

		//先装入code
		codes.push_back(searchForLuminCode(it->zLen, it->SSSS));
		//再装入对应的二进制码
		codes.push_back(getBinaryCode(it->value));
	}
	return codes;
}