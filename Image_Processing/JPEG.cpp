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
	//���Ӧλ����Ӧ��д��ֵ
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			s = i + j;

			if (s < N)//������
			{
				s = i + j;//Ϊ�˿��ĸ�����������������������ԳƵ����    
				index = s * (s + 1) / 2 + ((0 == (i + j) % 2) ? i : j);
				pZigZag[index] = pBlockData[i*N + j];
			}
			else//������
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
	//��������DCϵ����
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


	//����ɫ��DCϵ����
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


	//װ��������ϢACϵ����
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

	//װ��ɫ����ϢACϵ����
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
	//�ȶ�ֱ��ϵ�����б���(ֱ��ϵ����zlen��1)
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
	s.zLen = -1; //ֱ��ϵ��ͳһ��-1
	symbols.push_back(s);


	//Ȼ��Խ���ϵ�����б���
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

			while (zLen > 15) //���ȴ���15,��װһ��ZRL���ٶ�zlen-16���б���
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
	//�����һ����ֹ��(0,0)����ֹ���value��-INT_MAX
	s.zLen = 0;
	s.SSSS = 0;
	s.value = -INT_MAX;
	symbols.push_back(s);
	//�����ｻ��ϵ���ͱ��������
	return symbols;
}

vector<string> CJPEG::getLuminCodesBySymbolSequence(vector<symbol> &symbols)
{
	vector<string> codes;
	for (auto it = symbols.begin(); it != symbols.end(); ++it)
	{
		string s;
		if (it->zLen == -1) //��ʾ��DCϵ��,DCϵ����ı�һ������������
		{
			s = m_TableluminDC[it->SSSS].code;
			codes.push_back(s);
			codes.push_back(getBinaryCode(it->value));
			continue;
		}

		if (it->value == -INT_MAX) //��ʾ�ǽ�ֹ��EOB����ZRL����������
		{
			codes.push_back(searchForLuminCode(it->zLen, it->SSSS));
			continue;
		}

		//cout << searchForCode(it->zLen, it->SSSS) << endl;

		//��װ��code
		codes.push_back(searchForLuminCode(it->zLen, it->SSSS));
		//��װ���Ӧ�Ķ�������
		codes.push_back(getBinaryCode(it->value));
	}
	return codes;
}

//����ָ����zlen��ssss����code(��ǰ��û���Ż�������ٶ���ʵ�����Ż�)
string CJPEG::searchForLuminCode(int zlen, int SSSS)
{
	for (int i = 0; i != m_TableluminAC.size(); ++i)
	{
		if (m_TableluminAC[i].zLen == zlen && m_TableluminAC[i].SSSS == SSSS)
		{
			return m_TableluminAC[i].code;
		}
	}
	cout << zlen << "," << SSSS << "û�ҵ�lumin" << endl;
	return "";
}

//����ָ����zlen��ssss����code(��ǰ��û���Ż�������ٶ���ʵ�����Ż�)
string CJPEG::searchForChrominCode(int zlen, int SSSS)
{
	for (int i = 0; i != m_TablechrominAC.size(); ++i)
	{
		if (m_TablechrominAC[i].zLen == zlen && m_TablechrominAC[i].SSSS == SSSS)
		{
			return m_TablechrominAC[i].code;
		}
	}
	cout << zlen << "," << SSSS << "û�ҵ�chromin" << endl;
	return "";
}


//ȡһ�����Ķ����Ʊ���,�����õ�ԭ�룬�����õ�����,����������0���ͷ���0?��-1�����ظ���(��Ϊ��ʱ�Ĳ�ֵΪ0������ssss=0,Ҳ��������-1��0)
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
	if (s[0] == '0') //����
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
	else //����
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
		if (it->zLen == -1) //��ʾ��DCϵ��,DCϵ����ı�һ������������
		{
			s = m_TablechrominDC[it->SSSS].code;
			codes.push_back(s);
			codes.push_back(getBinaryCode(it->value));
			continue;
		}

		if (it->value == -INT_MAX) //��ʾ�ǽ�ֹ��EOB����ZRL����������
		{
			codes.push_back(searchForChrominCode(it->zLen, it->SSSS));
			continue;
		}

		//cout << searchForCode(it->zLen, it->SSSS) << endl;

		//��װ��code
		codes.push_back(searchForChrominCode(it->zLen, it->SSSS));
		//��װ���Ӧ�Ķ�������
		codes.push_back(getBinaryCode(it->value));
	}
	return codes;
}

//�������ֽ��н��루���ȣ�����ʵ���������룬SSSS�Ѿ�û�д��ڵ������ˣ�
void CJPEG::decodeLuminByCodes(vector<string> &codes, double *pBlockData, int nBlockSize, int preDC)
{
	vector<symbol> symbols; //�Ƚ���õ��������У��ٽ���������еõ�ԭʼֵ
	//�Ƚ���ֱ��ϵ��
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
		if (s.SSSS == 0 && s.zLen == 15)//ZRL�����ø���һ����zlen+16����
		{
			s.value = -INT_MAX;
			symbols.push_back(s);
			ZRLcomplementary += 16;
			continue;
		}

		//������������ģ�����һ��value�ı���
		if (ZRLcomplementary!=0) //���ǰ�������zrl������
		{
			s.zLen += ZRLcomplementary;
			ZRLcomplementary = 0;
		}
		s.value = getNumByBinaryCode(codes[++i]); //��index+1
		symbols.push_back(s);
	}

	//���ڵõ���symbols���ٸ���symbols�ָ�block
	//������ֱ��ϵ��
	pBlockData[0] = symbols[0].value + preDC;
	//Ȼ����zigzag�ָ�����ϵ��
	memset(pBlockData, 0.0, nBlockSize*nBlockSize); //��ȫ����0
	//


	return;
}

void CJPEG::getLuminACSymbolByTable(string &code, int& zlen, int & ssss)
{
	for (int i = 0; i != m_TableluminAC.size(); ++i)
	{
		cout << m_TableluminAC[i].code << " " << code << endl;
		if (!strcmp(m_TableluminAC[i].code.c_str(),code.c_str())) //���ﲻ����==����������
		{
			zlen = m_TableluminAC[i].zLen;
			ssss = m_TableluminAC[i].SSSS;
			return;
		}
	}
	cout << "û�ҵ�" << endl;
}

//�������ֽ��н��루ɫ�
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