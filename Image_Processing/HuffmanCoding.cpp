#include "stdafx.h"
#include "HuffmanCoding.h"


CHuffmanCoding::CHuffmanCoding()
{
}


CHuffmanCoding::~CHuffmanCoding()
{
}

int CHuffmanCoding::doHuffmanCoding(int *hist, int histLen,char ** dict,int *codelen)
{
	CodeLen = codelen;
	CodeStr = dict;
	//��ͳ��Ƶ��
	double *frep = new double[histLen];
	int sum = 0;
	for (int i=0;i!=histLen;++i)
		sum += hist[i];
	for (int i = 0; i != histLen; ++i)
		frep[i] = hist[i] / sum;

	//��ʼ����������
	for (int i = 0; i < 1000; i++)
	{
		Nodes[i].color = -1;
		Nodes[i].lson = Nodes[i].rson = -1;
		Nodes[i].num = -1;
		Nodes[i].mark = 0;
	}
	NodeNum = 0;

	int i, j, k, a, b;

	for (i = 0; i < histLen; i++)
	{//������ʼ�ڵ�  
		if (hist[i] > 0)
		{
			Nodes[NodeNum].color = i;
			Nodes[NodeNum].num = hist[i];
			Nodes[NodeNum].lson = Nodes[NodeNum].rson = -1;   //Ҷ�ӽڵ������Ҷ���  
			NodeNum++;
		}
	}

	while (1)
	{
		//�ҵ�����ֵ��С�Ľڵ㣬�ϲ���Ϊ�µĽڵ�  
		a = MinNode();
		//if (a == -1) //��һ���ڵ�϶����ܹ��ҵ���
		//	break;
		b = MinNode(); //�����潫���ϳɵ�Ϊ1���ʵĽڵ㸳ֵΪnodeStart
		if (b == -1)
			break;

		//�����½ڵ�
		Nodes[NodeNum].color = -1;
		Nodes[NodeNum].num = Nodes[a].num + Nodes[b].num;
		Nodes[NodeNum].lson = a;
		Nodes[NodeNum].rson = b;
		NodeNum++;
		//node[a].mark = node[b].mark = 1;  
	}

	//���ݽ��õ�Huffman������(����ʵ��)  
	dfs(m_NodeStart, 0);
	return m_NodeStart;
	//��õ������CodeStr�����ˣ���ʼ�ڵ�Ҳע������nodestart

	////����ƽ���볤,��Դ��,����Ч��
	//double HX = 0.0, L = 0.0;
	//for (int i = 0; i != 256; ++i)
	//{
	//	if (frep[i] != 0)
	//		HX += (frep[i] * -log2(frep[i]));
	//	if (frep[i] != 0)
	//		L += frep[i] * CodeLen[i];
	//}
	//cout << "��Դ��Ϊ:" << HX << endl;
	//cout << "ƽ���볤Ϊ:" << L << endl;
	//cout << "����Ч��:" << HX / L << endl;


	//��¼ͼ����Ϣ
	//m_nCodesLen = 0;
	//m_codes = new bool[histLen * 8]; //�ȶ�̬����һ�����ֵ,��ÿ������8bit����,��ʵ�϶��ò�������Ϊ�Ǳ䳤������
	//unsigned char * lpBuf;
	//for (i = 0; i < m_nHeight; i++)
	//	for (j = 0; j < m_nWidth; j++)
	//	{
	//		lpBuf = (unsigned char *)m_pImgTemp + m_nWidth * i + j;
	//		for (k = 0; k < CodeLen[*(lpBuf)]; k++)
	//			m_codes[m_nCodesLen++] = (int)(CodeStr[*(lpBuf)][k] - '0'); //���ڴ�ľͶ���1010��������
	//	}
	//��ʱm_codes������Ǳ���õĻ���������������

}


//Ѱ��ֵ��С�Ľڵ�  
int CHuffmanCoding::MinNode()
{
	int i, j = -1;
	for (i = 0; i < NodeNum; i++)
		if (!Nodes[i].mark) //�ҵ�δ����ǵ���Сnumֵ��hist���Ľڵ�
			if (j == -1 || Nodes[i].num < Nodes[j].num)
				j = i;
	if (j != -1)
	{
		m_NodeStart = j;
		Nodes[j].mark = 1;
	}
	return j;
}



void CHuffmanCoding::dfs(int pos, int len)
{

	//���������  
	if (Nodes[pos].lson != -1)
	{
		m_chCodeTemp[len] = '1';
		dfs(Nodes[pos].lson, len + 1);
	}
	else //����Ҷ�ӽڵ㣬��ʵ���վ���������Ҷ�ӽڵ�ı���
	{
		if (Nodes[pos].color != -1)
		{
			CodeLen[Nodes[pos].color] = len;
			m_chCodeTemp[len] = '\0'; //���һ���ط���ֵΪ\0����strcpy
			strcpy(CodeStr[Nodes[pos].color], m_chCodeTemp);
		}
	}

	//�����Ҷ���  
	if (Nodes[pos].lson != -1)
	{
		m_chCodeTemp[len] = '0';
		dfs(Nodes[pos].rson, len + 1);
	}
	else
	{
		if (Nodes[pos].color != -1)
		{
			CodeLen[Nodes[pos].color] = len;
			m_chCodeTemp[len] = '\0';
			strcpy(CodeStr[Nodes[pos].color], m_chCodeTemp);
		}
	}
}