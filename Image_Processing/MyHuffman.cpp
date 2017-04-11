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

	//读入图像数据到pBmpBuf
	for (int i=0;i!=srcImg.GetHeight();++i)
	{
		for (int j=0;j!=srcImg.GetWidth();++j)
		{
			pBmpBuf[i*srcImg.GetWidth() + j] = srcImg.m_pBits[0][i][j];
		}
	}

	//初始化hist和huffman树
	HuffmanCodeInit();

	//计算每个灰度值出现的次数  (统计直方图) 
	for (int i = 0; i < bmpHeight; i++)
		for (int j = 0; j < bmpWidth; j++)
		{
			lpBuf = (unsigned char *)pBmpBuf + lineByte * i + j;
			hist[*(lpBuf)] += 1;
		}

	HuffmanCode(); //进行编码  结果存在imgInf里面
	HuffmanDecode();//进行霍夫曼解码 解码结果存在pBmpBuf里面

	//再把结果赋值回dstimg就可以了
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


//Huffman编码初始化  
void CMyHuffman::HuffmanCodeInit()
{
	int i;
	for (i = 0; i < 256; i++)//灰度值记录清零  
		hist[i] = 0;
	//初始化哈夫曼树  
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
	//遍历左儿子  
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

	//遍历右儿子  
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

//寻找值最小的节点  
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

//编码主函数  
void CMyHuffman::HuffmanCode()
{
	int i, j, k, a, b;

	for (i = 0; i < 256; i++)
	{//创建初始节点  
		Feq[i] = (float)hist[i] / (float)(bmpHeight * bmpWidth);//计算灰度值频率  
		if (hist[i] > 0)
		{
			node[NodeNum].color = i;
			node[NodeNum].num = hist[i];
			node[NodeNum].lson = node[NodeNum].rson = -1;   //叶子节点无左右儿子  
			NodeNum++;
		}
	}

	while (1)
	{
		//找到两个值最小的节点，合并成为新的节点  
		a = MinNode();
		if (a == -1)
			break;
		b = MinNode();
		if (b == -1)
			break;

		//构建新节点  
		node[NodeNum].color = -1;
		node[NodeNum].num = node[a].num + node[b].num;
		node[NodeNum].lson = a;
		node[NodeNum].rson = b;
		NodeNum++;
		//node[a].mark = node[b].mark = 1;  
	}

	//根据建好的Huffman树编码(深搜实现)  
	dfs(m_NodeStart, 0);

	//计算平均码长,信源熵,编码效率
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
	cout << "信源熵为:" << HX << endl;
	cout << "平均码长为:" << L << endl;
	cout << "编码效率:" << HX / L << endl;

	//屏幕输出编码  
	int sum = 0;
	//  printf("Huffman编码信息如下：\n");  
	for (i = 0; i < 256; i++)
		if (hist[i] > 0)
		{
			sum += CodeLen[i] * hist[i];
			//printf("灰度值：%3d  频率: %f  码长: %2d  编码: %s\n", i, Feq[i], CodeLen[i], CodeStr[i]);
		}
	//printf("原始总码长：%d\n", bmpWidth * bmpHeight * 8);
	//printf("Huffman编码总码长：%d\n", sum);
	//printf("压缩比：%.3f : 1\n", (float)(bmpWidth * bmpHeight * 8) / (float)sum);

	//记录图像信息
	InfLen = 0;
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	ImgInf = new bool[bmpHeight*bmpWidth * 8]; //先动态分配一个最大值,即每个都用8bit编码,其实肯定用不到，因为是变长编码嘛
	for (i = 0; i < bmpHeight; i++)
		for (j = 0; j < bmpWidth; j++)
		{
			lpBuf = (unsigned char *)pBmpBuf + lineByte * i + j;
			for (k = 0; k < CodeLen[*(lpBuf)]; k++)
			{
				ImgInf[InfLen++] = (int)(CodeStr[*(lpBuf)][k] - '0');
			}
		}


	//此时imgInf里面就是编码好的霍夫曼编码数据了
	

	////再编码数据  
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
	//获取编码信息
	int i, j, tmp;
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;

	//解码  
	int p = m_NodeStart;  //遍历指针位置  
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

	//现在pBmpBuf里就是恢复的数据了
	
}