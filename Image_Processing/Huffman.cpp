#include "stdafx.h"
#include "Huffman.h"
#include <iostream>
using namespace std;

CHuffman::CHuffman()
{
	//pBmpBuf = NULL;
	//pColorTable = NULL;
}


CHuffman::~CHuffman()
{
}


//二进制转十进制  
int CHuffman::Change2to10(int pos)
{
	int i, j, two = 1;
	j = 0;
	for (i = pos + 7; i >= pos; i--) {
		j += two * ImgInf[i];
		two *= 2;
	}
	return j;
}

//保存Huffman编码树  
int CHuffman::saveInfo(char *writePath, int lineByte) {
	int i, j, k;

	FILE *fout;
	fout = fopen(writePath, "w");
	fprintf(fout, "%d %d %d\n", m_NodeStart, NodeNum, InfLen);//输出起始节点、节点总数、图像所占空间  
	for (i = 0; i < NodeNum; i++) {     //输出Huffman树  
		fprintf(fout, "%d %d %d\n", node[i].color, node[i].lson, node[i].rson);
	}

	fclose(fout);
	return 0;
}


/*********************

Huffman编码图像解码

*********************/
//读入编码图像  
bool CHuffman::readHuffman(char *Name)
{
	int i, j;
	char NameStr[100];
	//读取Huffman编码信息和编码树  
	strcpy(NameStr, Name);
	strcat(NameStr, ".bpt");
	FILE *fin = fopen(NameStr, "r");
	if (fin == 0) {
		printf("未找到指定文件！\n");
		return 0;
	}
	fscanf(fin, "%d %d %d", &m_NodeStart, &NodeNum, &InfLen);
	//printf("%d %d %d\n",NodeStart,NodeNum,InfLen);  
	for (i = 0; i < NodeNum; i++) {
		fscanf(fin, "%d %d %d", &node[i].color, &node[i].lson, &node[i].rson);
		//printf("%d %d %d\n",node[i].color,node[i].lson,node[i].rson);  
	}

	//二进制读方式打开指定的图像文件  
	strcpy(NameStr, Name);
	strcat(NameStr, ".bhd");
	FILE *fp = fopen(NameStr, "rb");
	if (fp == 0) {
		printf("未找到指定文件！\n");
		return 0;
	}

	//跳过位图文件头结构BITMAPFILEHEADER  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中  
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//获取图像宽、高、每像素所占位数等信息  
	bmpWidth = head.biWidth;
	bmpHeight = head.biHeight;
	biBitCount = head.biBitCount;

	//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）  
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;

	//灰度图像有颜色表，且颜色表表项为256  
	if (biBitCount == 8) {
		//申请颜色表所需要的空间，读颜色表进内存  
		pColorTable = new RGBQUAD[256];
		fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	}

	//申请位图数据所需要的空间，读位图数据进内存  
	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, InfLen / 8, fp);

	//关闭文件  
	fclose(fp);

	return 1;
}

void CHuffman::HuffmanDecode()
{
	//获取编码信息
	int i, j, tmp;
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	for (i = 0; i < InfLen / 8; i++)
	{
		j = i * 8 + 7;
		tmp = *(pBmpBuf + i);
		while (tmp > 0)
		{
			ImgInf[j] = tmp % 2;
			tmp /= 2;
			j--;
		}
	}
	/*for(i = 0;i < InfLen;i ++)
	printf("%d",ImgInf[i]);
	printf("\n");*/

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
	//printf("\nj: %d\n",j);  
}


/*********************

Huffman编码

*********************/
//Huffman编码初始化  
void CHuffman::HuffmanCodeInit()
{
	int i;
	for (i = 0; i <256; i++)//灰度值记录清零  
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

//深搜遍历Huffman树获取编码值  

void CHuffman::dfs(int pos, int len)
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
int CHuffman::MinNode()
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
void CHuffman::HuffmanCode()
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
	ImgInf = new bool[bmpHeight*bmpWidth*8]; //先动态分配一个最大值,即每个都用8bit编码,其实肯定用不到，因为是变长编码嘛
	for (i = 0; i < bmpHeight; i++)
		for (j = 0; j < bmpWidth; j++)
		{
			lpBuf = (unsigned char *)pBmpBuf + lineByte * i + j;
			for (k = 0; k < CodeLen[*(lpBuf)]; k++)
			{
				ImgInf[InfLen++] = (int)(CodeStr[*(lpBuf)][k] - '0');
			}
		}

	//再编码数据  
	j = 0;
	for (i = 0; i < InfLen;)
	{
		*(pBmpBuf + j) = Change2to10(i);
		i += 8;
		j++;
	}
	//cout << j << endl;
}

//保存文件  
bool CHuffman::saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height,
	int biBitCount, RGBQUAD *pColorTable)
{
	//imgBuf = pBmpBuf
	//如果位图数据指针为0,则没有数据传入,函数返回  
	if (!imgBuf)
		return 0;

	//颜色表大小,以字节为单位,灰度图像颜色表为1024字节,彩色图像颜色表大小为0  
	int colorTablesize = 0;
	if (biBitCount == 8)
		colorTablesize = 1024;

	//待存储图像数据每行字节数为4的倍数  
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//以二进制写的方式打开文件  
	FILE *fp = fopen(bmpName, "wb");
	if (fp == 0) return 0;

	//申请位图文件头结构变量，填写文件头信息  
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;//bmp类型  

							 //bfSize是图像文件4个组成部分之和  
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ colorTablesize + lineByte*height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;

	//bfOffBits是图像文件前三个部分所需空间之和  
	fileHead.bfOffBits = 54 + colorTablesize;

	//写文件头进文件  
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//申请位图信息头结构变量，填写信息头信息  
	BITMAPINFOHEADER head;
	head.biBitCount = biBitCount;
	head.biClrImportant = 0;
	head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = lineByte*height;
	head.biWidth = width;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;
	//写位图信息头进内存  
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//如果灰度图像,有颜色表,写入文件   
	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//写位图数据进文件  
	//fwrite(imgBuf, height*lineByte, 1, fp);  
	fwrite(imgBuf, InfLen / 8, 1, fp);


	//关闭文件  
	fclose(fp);

	return 1;
}


bool CHuffman::readBmp(char *bmpName)
{
	//二进制读方式打开指定的图像文件  
	FILE *fp = fopen(bmpName, "rb");
	if (fp == 0)
	{
		printf("未找到指定文件！\n");
		return 0;
	}


	//跳过位图文件头结构BITMAPFILEHEADER  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);


	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中  
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//获取图像宽、高、每像素所占位数等信息  
	bmpWidth = head.biWidth;
	bmpHeight = head.biHeight;
	biBitCount = head.biBitCount;

	//定义变量，计算图像每行像素所占的字节数（必须是4的倍数），对灰度图来说就是图像宽度，然后可能不是4的倍数就扩充到4的倍数，比如409到412
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;

	//灰度图像有颜色表，且颜色表表项为256  
	if (biBitCount == 8)
	{
		//申请颜色表所需要的空间，读颜色表进内存  
		pColorTable = new RGBQUAD[256];
		fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	}

	//申请位图数据所需要的空间，读位图数据进内存  
	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	//关闭文件  
	fclose(fp);

	return 1;
}


void CHuffman::DoHuffmanCode(const MyImage_ &srcImg,MyImage_ & dstImg)
{

	int lineByte = srcImg.GetWidth();
	bmpHeight = srcImg.GetHeight();
	bmpWidth = srcImg.GetWidth();
	biBitCount = 8;
	pBmpBuf = new unsigned char[lineByte * bmpHeight];

	//srcImg.Save(L"huffman.bmp");

	//CImage imgTemp;
	//imgTemp.Create(bmpWidth, bmpHeight, 8);
	//int w = srcImg.GetWidth();
	//int h = srcImg.GetHeight();
	//int nrow = imgTemp.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	//BYTE *psrc = (BYTE *)imgTemp.GetBits();//获得m_CImage最后一行的像素地址
	//for (int j = 0; j < h; j++)
	//{
	//	for (int k = 0; k < w; k++)
	//	{
	//		psrc[j*nrow + k] = srcImg.m_pBits[0][j][k];//B
	//		//cout << (int)psrc[j*nrow + k] << endl;
	//	}
	//}
	//imgTemp.Save(L"huffman.BMP", GUID_NULL);


	readBmp("huffman_gray.bmp");

	HuffmanCodeInit();

	//计算每个灰度值出现的次数  (统计直方图) 
	for (int i = 0; i < bmpHeight; i++)
		for (int j = 0; j < bmpWidth; j++)
		{
			lpBuf = (unsigned char *)pBmpBuf + lineByte * i + j;
			hist[*(lpBuf)] += 1;
		}

	//调用编码  
	HuffmanCode(); //
	saveBmp("huffman.bhd", pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
	saveInfo("huffman.bpt", lineByte);
	delete[] pBmpBuf;
	if (biBitCount == 8)
		delete[] pColorTable;

	//编码解码初始化  
	HuffmanCodeInit();
	readHuffman("huffman");
	HuffmanDecode();    //Huffman解码  
	InfLen = bmpWidth * bmpHeight * 8;
	saveBmp("huffman_decode.bmp", pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
	//再读入保存的图
	dstImg.Load(L"huffman_decode.bmp");

	delete[] ImgInf;
}
