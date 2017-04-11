#include "stdafx.h"
#include "MyHuffman.h"


CMyHuffman::CMyHuffman()
{
}


CMyHuffman::~CMyHuffman()
{
}

void CMyHuffman::DoHuffmanCode(const MyImage_ & srcImg, MyImage_ & dstImg)
{
	int lineByte = srcImg.GetWidth();
	bmpHeight = srcImg.GetHeight();
	bmpWidth = srcImg.GetWidth();
	biBitCount = 8;
	pBmpBuf = new unsigned char[lineByte * bmpHeight];

	//����ͼ�����ݵ�pBmpBuf
	for (int i=0;i!=srcImg.GetHeight();++i)
	{
		for (int j=0;j!=srcImg.GetWidth();++j)
		{
			pBmpBuf[i*srcImg.GetWidth() + j] = srcImg.m_pBits[0][i][j];
		}
	}

	//��ʼ��hist��huffman��
	HuffmanCodeInit();

	//����ÿ���Ҷ�ֵ���ֵĴ���  (ͳ��ֱ��ͼ) 
	for (int i = 0; i < bmpHeight; i++)
		for (int j = 0; j < bmpWidth; j++)
		{
			lpBuf = (unsigned char *)pBmpBuf + lineByte * i + j;
			hist[*(lpBuf)] += 1;
		}

	HuffmanCode(); //���б���  �������imgInf����
	HuffmanDecode();//���л��������� ����������pBmpBuf����

	//�ٰѽ����ֵ��dstimg�Ϳ�����
	for (int i = 0; i != srcImg.GetHeight(); ++i)
	{
		for (int j = 0; j != srcImg.GetWidth(); ++j)
		{
			dstImg.m_pBits[0][i][j] = pBmpBuf[i*srcImg.GetWidth() + j];
			dstImg.m_pBits[1][i][j] = pBmpBuf[i*srcImg.GetWidth() + j];
			dstImg.m_pBits[2][i][j] = pBmpBuf[i*srcImg.GetWidth() + j];
		}
	}
}


//Huffman�����ʼ��  
void CMyHuffman::HuffmanCodeInit()
{
	int i;
	for (i = 0; i < 256; i++)//�Ҷ�ֵ��¼����  
		hist[i] = 0;
	//��ʼ����������  
	for (i = 0; i < 600; i++)
	{
		node[i].color = -1;
		node[i].lson = node[i].rson = -1;
		node[i].num = -1;
		node[i].mark = 0;
	}
	NodeNum = 0;
}

void CMyHuffman::dfs(int pos, int len)
{
	//���������  
	if (node[pos].lson != -1)
	{
		CodeTmp[len] = '1';
		dfs(node[pos].lson, len + 1);
	}
	else
	{
		if (node[pos].color != -1)
		{
			CodeLen[node[pos].color] = len;
			CodeTmp[len] = '\0';
			strcpy(CodeStr[node[pos].color], CodeTmp);
		}
	}

	//�����Ҷ���  
	if (node[pos].lson != -1)
	{
		CodeTmp[len] = '0';
		dfs(node[pos].rson, len + 1);
	}
	else {
		if (node[pos].color != -1)
		{
			CodeLen[node[pos].color] = len;
			CodeTmp[len] = '\0';
			strcpy(CodeStr[node[pos].color], CodeTmp);
		}
	}
}

//Ѱ��ֵ��С�Ľڵ�  
int CMyHuffman::MinNode()
{
	int i, j = -1;
	for (i = 0; i < NodeNum; i++)
		if (!node[i].mark)
			if (j == -1 || node[i].num < node[j].num)
				j = i;
	if (j != -1)
	{
		m_NodeStart = j;
		node[j].mark = 1;
	}
	return j;
}

//����������  
void CMyHuffman::HuffmanCode()
{
	int i, j, k, a, b;

	for (i = 0; i < 256; i++)
	{//������ʼ�ڵ�  
		Feq[i] = (float)hist[i] / (float)(bmpHeight * bmpWidth);//����Ҷ�ֵƵ��  
		if (hist[i] > 0)
		{
			node[NodeNum].color = i;
			node[NodeNum].num = hist[i];
			node[NodeNum].lson = node[NodeNum].rson = -1;   //Ҷ�ӽڵ������Ҷ���  
			NodeNum++;
		}
	}

	while (1)
	{
		//�ҵ�����ֵ��С�Ľڵ㣬�ϲ���Ϊ�µĽڵ�  
		a = MinNode();
		if (a == -1)
			break;
		b = MinNode();
		if (b == -1)
			break;

		//�����½ڵ�  
		node[NodeNum].color = -1;
		node[NodeNum].num = node[a].num + node[b].num;
		node[NodeNum].lson = a;
		node[NodeNum].rson = b;
		NodeNum++;
		//node[a].mark = node[b].mark = 1;  
	}

	//���ݽ��õ�Huffman������(����ʵ��)  
	dfs(m_NodeStart, 0);

	//����ƽ���볤,��Դ��,����Ч��
	double HX = 0.0, L = 0.0;
	for (int i = 0; i != 256; ++i)
	{
		if (Feq[i] != 0)
			HX += (Feq[i] * -log2(Feq[i]));
		if (Feq[i] != 0)
		{
			L += Feq[i] * CodeLen[i];
		}
	}
	cout << "��Դ��Ϊ:" << HX << endl;
	cout << "ƽ���볤Ϊ:" << L << endl;
	cout << "����Ч��:" << HX / L << endl;

	//��Ļ�������  
	int sum = 0;
	//  printf("Huffman������Ϣ���£�\n");  
	for (i = 0; i < 256; i++)
		if (hist[i] > 0)
		{
			sum += CodeLen[i] * hist[i];
			//printf("�Ҷ�ֵ��%3d  Ƶ��: %f  �볤: %2d  ����: %s\n", i, Feq[i], CodeLen[i], CodeStr[i]);
		}
	//printf("ԭʼ���볤��%d\n", bmpWidth * bmpHeight * 8);
	//printf("Huffman�������볤��%d\n", sum);
	//printf("ѹ���ȣ�%.3f : 1\n", (float)(bmpWidth * bmpHeight * 8) / (float)sum);

	//��¼ͼ����Ϣ
	InfLen = 0;
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	ImgInf = new bool[bmpHeight*bmpWidth * 8]; //�ȶ�̬����һ�����ֵ,��ÿ������8bit����,��ʵ�϶��ò�������Ϊ�Ǳ䳤������
	for (i = 0; i < bmpHeight; i++)
		for (j = 0; j < bmpWidth; j++)
		{
			lpBuf = (unsigned char *)pBmpBuf + lineByte * i + j;
			for (k = 0; k < CodeLen[*(lpBuf)]; k++)
			{
				ImgInf[InfLen++] = (int)(CodeStr[*(lpBuf)][k] - '0');
			}
		}


	//��ʱimgInf������Ǳ���õĻ���������������
	

	////�ٱ�������  
	//j = 0;
	//for (i = 0; i < InfLen;)
	//{
	//	*(pBmpBuf + j) = Change2to10(i);
	//	i += 8;
	//	j++;
	//}
	//cout << j << endl;
}


void CMyHuffman::HuffmanDecode()
{
	//��ȡ������Ϣ
	int i, j, tmp;
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;

	//����  
	int p = m_NodeStart;  //����ָ��λ��  
	j = 0;
	i = 0;
	do
	{
		if (node[p].color >= 0)
		{
			*(pBmpBuf + j) = node[p].color;
			//printf("%d ",*(pBmpBuf + j));  
			j++;
			p = m_NodeStart;
		}
		if (ImgInf[i] == 1)
			p = node[p].lson;
		else if (ImgInf[i] == 0)
			p = node[p].rson;
		i++;
	} while (i <= InfLen);

	//����pBmpBuf����ǻָ���������
	
}