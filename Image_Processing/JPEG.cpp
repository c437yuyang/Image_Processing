#include "stdafx.h"
#include "JPEG.h"

int getNumOfHex(string s);
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
			m_TableluminDC.push_back(dti);
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
			m_TablechrominDC.push_back(dti);
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
			string temp;
			ss >> temp;
			ati.zLen = getNumOfHex(temp);
			ss >> temp;
			if (temp == "A")
				ati.SSSS = 10;
			else
				ati.SSSS = atoi(temp.c_str());
			ss >> ati.len;
			ss >> ati.code;
			m_TableluminAC.push_back(ati);
		}
	}
	else
		return false;
	ifs2.close();

	//装载色差信息AC系数表
	ifstream ifs3("tb_chromin_ac.txt");
	if (ifs3.is_open())
	{
		string line;
		while (!ifs3.eof())
		{
			getline(ifs3, line);
			stringstream ss(line);
			string temp;
			ss >> temp;
			ati.zLen = getNumOfHex(temp);
			ss >> temp;
			if (temp == "A")
				ati.SSSS = 10;
			else
				ati.SSSS = atoi(temp.c_str());
			ss >> ati.len;
			ss >> ati.code;
			m_TablechrominAC.push_back(ati);
		}
	}
	else
		return false;
	ifs3.close();

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
	for (int i = 1; i != nBlockSize*nBlockSize; ++i)
	{
		if (pZigZag[i] == 0)
		{
			zLen += 1;
		}
		else
		{

			while (zLen > 15) //长度大于15,先装一个ZRL，再对zlen-16进行编码
			{
				s.SSSS = 0;
				s.zLen = 15;
				s.value = -INT_MAX;
				symbols.push_back(s);
				zLen -= 16;
			}

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

vector<string> CJPEG::getLuminCodesBySymbolSequence(vector<symbol> &symbols)
{
	vector<string> codes;
	for (auto it = symbols.begin(); it != symbols.end(); ++it)
	{
		string s;
		if (it->zLen == -1) //表示是DC系数,DC系数查的表不一样，单独处理
		{
			s = m_TableluminDC[it->SSSS].code;
			codes.push_back(s);
			codes.push_back(getBinaryCode(it->value));
			continue;
		}

		if (it->value == -INT_MAX) //表示是截止块EOB或者ZRL，单独处理
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
string CJPEG::searchForLuminCode(int zlen, int SSSS)
{
	for (int i = 0; i != m_TableluminAC.size(); ++i)
	{
		if (m_TableluminAC[i].zLen == zlen && m_TableluminAC[i].SSSS == SSSS)
		{
			return m_TableluminAC[i].code;
		}
	}
	cout << zlen << "," << SSSS << "没找到lumin" << endl;
	return "";
}

//根据指定的zlen和ssss查找code(当前还没做优化，查表速度其实可以优化)
string CJPEG::searchForChrominCode(int zlen, int SSSS)
{
	for (int i = 0; i != m_TablechrominAC.size(); ++i)
	{
		if (m_TablechrominAC[i].zLen == zlen && m_TablechrominAC[i].SSSS == SSSS)
		{
			return m_TablechrominAC[i].code;
		}
	}
	cout << zlen << "," << SSSS << "没找到chromin" << endl;
	return "";
}


//取一个数的二进制编码,正数得到原码，负数得到反码,如果本身就是0，就返回0?和-1岂不是重复了(因为此时的差值为0，根据ssss=0,也可以区别-1和0)
string CJPEG::getBinaryCode(int value)
{
	string code = "";
	if (value > 0)
	{
		while (value != 0)
		{
			if (value % 2 == 1)
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
	else if (value < 0)
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

int CJPEG::getNumByBinaryCode(string s)
{
	int num = 0;
	int n = 0;
	if (s[0] == '0') //负数
	{
		while (s.length() > 0)
		{
			if (s[s.length() - 1] == '0')
				num += pow(2, n);
			n++;
			s.pop_back();
		}
		return -num;
	}
	else //正数
	{
		while (s.length() > 0)
		{
			if (s[s.length() - 1] == '1')
				num += pow(2, n);
			n++;
			s.pop_back();
		}
		return num;
	}
}


vector<string> CJPEG::getChrominCodesBySymbolSequence(vector<symbol> &symbols)
{
	vector<string> codes;
	for (auto it = symbols.begin(); it != symbols.end(); ++it)
	{
		string s;
		if (it->zLen == -1) //表示是DC系数,DC系数查的表不一样，单独处理
		{
			s = m_TablechrominDC[it->SSSS].code;
			codes.push_back(s);
			codes.push_back(getBinaryCode(it->value));
			continue;
		}

		if (it->value == -INT_MAX) //表示是截止块EOB或者ZRL，单独处理
		{
			codes.push_back(searchForChrominCode(it->zLen, it->SSSS));
			continue;
		}

		//cout << searchForCode(it->zLen, it->SSSS) << endl;

		//先装入code
		codes.push_back(searchForChrominCode(it->zLen, it->SSSS));
		//再装入对应的二进制码
		codes.push_back(getBinaryCode(it->value));
	}
	return codes;
}

//根据码字进行解码（亮度）（其实我这样编码，SSSS已经没有存在的意义了）
void CJPEG::decodeLuminByCodes(vector<string> &codes, double *pBlockData, int nBlockSize, int preDC)
{
	vector<symbol> symbols; //先解码得到符号序列，再解码符号序列得到原始值
	//先解码直流系数
	int ZRLcomplementary = 0;
	symbol s;
	s.zLen = -1;
	s.SSSS = getLuminDCSSSS(codes[0]);
	s.value = getNumByBinaryCode(codes[1]);
	symbols.push_back(s); // 
	for (int i = 2; i != codes.size(); ++i)
	{
		getLuminACSymbolByTable(codes[i], s.zLen, s.SSSS);
		if ((s.SSSS == 0 && s.zLen == 0)) //EOB
		{
			s.value = -INT_MAX;
			symbols.push_back(s);
			continue;
		}
		if (s.SSSS == 0 && s.zLen == 15)//ZRL，还得给下一个的zlen+16才行
		{
			s.value = -INT_MAX;
			symbols.push_back(s);
			ZRLcomplementary += 16;
			continue;
		}

		//否则就是正常的，带有一个value的编码
		if (ZRLcomplementary!=0) //如果前面出现了zrl补偿的
		{
			s.zLen += ZRLcomplementary;
			ZRLcomplementary = 0;
		}
		s.value = getNumByBinaryCode(codes[++i]); //让index+1
		symbols.push_back(s);
	}

	//现在得到了symbols，再根据symbols恢复block
	//首先是直流系数
	pBlockData[0] = symbols[0].value + preDC;
	//然后按照zigzag恢复交流系数
	memset(pBlockData, 0.0, nBlockSize*nBlockSize); //先全部置0
	//


	return;
}

void CJPEG::getLuminACSymbolByTable(string &code, int& zlen, int & ssss)
{
	for (int i = 0; i != m_TableluminAC.size(); ++i)
	{
		cout << m_TableluminAC[i].code << " " << code << endl;
		if (!strcmp(m_TableluminAC[i].code.c_str(),code.c_str())) //这里不能用==，引用类型
		{
			zlen = m_TableluminAC[i].zLen;
			ssss = m_TableluminAC[i].SSSS;
			return;
		}
	}
	cout << "没找到" << endl;
}

//根据码字进行解码（色差）
void CJPEG::decodeChrominByCodes(vector<string> &codes, double *pBlockData, int nBlockSize, int preDC)
{

}


int getNumOfHex(string s)
{
	if (s == "A")
		return 10;
	else if (s == "B")
		return 11;
	else if (s == "C")
		return 12;
	else if (s == "D")
		return 13;
	else if (s == "E")
		return 14;
	else if (s == "F")
		return 15;
	else
		return atoi(s.c_str());
}


int CJPEG::getLuminDCSSSS(string code)
{
	for (int i = 0; i != m_TableluminDC.size(); ++i)
	{
		if (m_TableluminDC[i].code == code)
		{
			return m_TableluminDC[i].SSSS;
		}
	}
}