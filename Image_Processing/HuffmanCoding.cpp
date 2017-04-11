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
	//先统计频率
	double *frep = new double[histLen];
	int sum = 0;
	for (int i=0;i!=histLen;++i)
		sum += hist[i];
	for (int i = 0; i != histLen; ++i)
		frep[i] = hist[i] / sum;

	//初始化哈夫曼树
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
	{//创建初始节点  
		if (hist[i] > 0)
		{
			Nodes[NodeNum].color = i;
			Nodes[NodeNum].num = hist[i];
			Nodes[NodeNum].lson = Nodes[NodeNum].rson = -1;   //叶子节点无左右儿子  
			NodeNum++;
		}
	}

	while (1)
	{
		//找到两个值最小的节点，合并成为新的节点  
		a = MinNode();
		//if (a == -1) //第一个节点肯定是能够找到的
		//	break;
		b = MinNode(); //这里面将最后合成的为1概率的节点赋值为nodeStart
		if (b == -1)
			break;

		//构建新节点
		Nodes[NodeNum].color = -1;
		Nodes[NodeNum].num = Nodes[a].num + Nodes[b].num;
		Nodes[NodeNum].lson = a;
		Nodes[NodeNum].rson = b;
		NodeNum++;
		//node[a].mark = node[b].mark = 1;  
	}

	//根据建好的Huffman树编码(深搜实现)  
	dfs(m_NodeStart, 0);
	return m_NodeStart;
	//编好的码表在CodeStr里面了，起始节点也注明了是nodestart

	////计算平均码长,信源熵,编码效率
	//double HX = 0.0, L = 0.0;
	//for (int i = 0; i != 256; ++i)
	//{
	//	if (frep[i] != 0)
	//		HX += (frep[i] * -log2(frep[i]));
	//	if (frep[i] != 0)
	//		L += frep[i] * CodeLen[i];
	//}
	//cout << "信源熵为:" << HX << endl;
	//cout << "平均码长为:" << L << endl;
	//cout << "编码效率:" << HX / L << endl;


	//记录图像信息
	//m_nCodesLen = 0;
	//m_codes = new bool[histLen * 8]; //先动态分配一个最大值,即每个都用8bit编码,其实肯定用不到，因为是变长编码嘛
	//unsigned char * lpBuf;
	//for (i = 0; i < m_nHeight; i++)
	//	for (j = 0; j < m_nWidth; j++)
	//	{
	//		lpBuf = (unsigned char *)m_pImgTemp + m_nWidth * i + j;
	//		for (k = 0; k < CodeLen[*(lpBuf)]; k++)
	//			m_codes[m_nCodesLen++] = (int)(CodeStr[*(lpBuf)][k] - '0'); //现在存的就都是1010的数字了
	//	}
	//此时m_codes里面就是编码好的霍夫曼编码数据了

}


//寻找值最小的节点  
int CHuffmanCoding::MinNode()
{
	int i, j = -1;
	for (i = 0; i < NodeNum; i++)
		if (!Nodes[i].mark) //找到未被标记的最小num值（hist）的节点
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

	//遍历左儿子  
	if (Nodes[pos].lson != -1)
	{
		m_chCodeTemp[len] = '1';
		dfs(Nodes[pos].lson, len + 1);
	}
	else //到了叶子节点，其实最终就是求所有叶子节点的编码
	{
		if (Nodes[pos].color != -1)
		{
			CodeLen[Nodes[pos].color] = len;
			m_chCodeTemp[len] = '\0'; //最后一个地方赋值为\0才能strcpy
			strcpy(CodeStr[Nodes[pos].color], m_chCodeTemp);
		}
	}

	//遍历右儿子  
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