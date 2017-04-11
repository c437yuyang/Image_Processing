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


//������תʮ����  
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

//����Huffman������  
int CHuffman::saveInfo(char *writePath, int lineByte) {
	int i, j, k;

	FILE *fout;
	fout = fopen(writePath, "w");
	fprintf(fout, "%d %d %d\n", m_NodeStart, NodeNum, InfLen);//�����ʼ�ڵ㡢�ڵ�������ͼ����ռ�ռ�  
	for (i = 0; i < NodeNum; i++) {     //���Huffman��  
		fprintf(fout, "%d %d %d\n", node[i].color, node[i].lson, node[i].rson);
	}

	fclose(fout);
	return 0;
}


/*********************

Huffman����ͼ�����

*********************/
//�������ͼ��  
bool CHuffman::readHuffman(char *Name)
{
	int i, j;
	char NameStr[100];
	//��ȡHuffman������Ϣ�ͱ�����  
	strcpy(NameStr, Name);
	strcat(NameStr, ".bpt");
	FILE *fin = fopen(NameStr, "r");
	if (fin == 0) {
		printf("δ�ҵ�ָ���ļ���\n");
		return 0;
	}
	fscanf(fin, "%d %d %d", &m_NodeStart, &NodeNum, &InfLen);
	//printf("%d %d %d\n",NodeStart,NodeNum,InfLen);  
	for (i = 0; i < NodeNum; i++) {
		fscanf(fin, "%d %d %d", &node[i].color, &node[i].lson, &node[i].rson);
		//printf("%d %d %d\n",node[i].color,node[i].lson,node[i].rson);  
	}

	//�����ƶ���ʽ��ָ����ͼ���ļ�  
	strcpy(NameStr, Name);
	strcat(NameStr, ".bhd");
	FILE *fp = fopen(NameStr, "rb");
	if (fp == 0) {
		printf("δ�ҵ�ָ���ļ���\n");
		return 0;
	}

	//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��  
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//��ȡͼ����ߡ�ÿ������ռλ������Ϣ  
	bmpWidth = head.biWidth;
	bmpHeight = head.biHeight;
	biBitCount = head.biBitCount;

	//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����  
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;

	//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256  
	if (biBitCount == 8) {
		//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�  
		pColorTable = new RGBQUAD[256];
		fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	}

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�  
	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, InfLen / 8, fp);

	//�ر��ļ�  
	fclose(fp);

	return 1;
}

void CHuffman::HuffmanDecode()
{
	//��ȡ������Ϣ
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
	//printf("\nj: %d\n",j);  
}


/*********************

Huffman����

*********************/
//Huffman�����ʼ��  
void CHuffman::HuffmanCodeInit()
{
	int i;
	for (i = 0; i <256; i++)//�Ҷ�ֵ��¼����  
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

//���ѱ���Huffman����ȡ����ֵ  

void CHuffman::dfs(int pos, int len)
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

//����������  
void CHuffman::HuffmanCode()
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
	ImgInf = new bool[bmpHeight*bmpWidth*8]; //�ȶ�̬����һ�����ֵ,��ÿ������8bit����,��ʵ�϶��ò�������Ϊ�Ǳ䳤������
	for (i = 0; i < bmpHeight; i++)
		for (j = 0; j < bmpWidth; j++)
		{
			lpBuf = (unsigned char *)pBmpBuf + lineByte * i + j;
			for (k = 0; k < CodeLen[*(lpBuf)]; k++)
			{
				ImgInf[InfLen++] = (int)(CodeStr[*(lpBuf)][k] - '0');
			}
		}

	//�ٱ�������  
	j = 0;
	for (i = 0; i < InfLen;)
	{
		*(pBmpBuf + j) = Change2to10(i);
		i += 8;
		j++;
	}
	//cout << j << endl;
}

//�����ļ�  
bool CHuffman::saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height,
	int biBitCount, RGBQUAD *pColorTable)
{
	//imgBuf = pBmpBuf
	//���λͼ����ָ��Ϊ0,��û�����ݴ���,��������  
	if (!imgBuf)
		return 0;

	//��ɫ���С,���ֽ�Ϊ��λ,�Ҷ�ͼ����ɫ��Ϊ1024�ֽ�,��ɫͼ����ɫ���СΪ0  
	int colorTablesize = 0;
	if (biBitCount == 8)
		colorTablesize = 1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���  
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//�Զ�����д�ķ�ʽ���ļ�  
	FILE *fp = fopen(bmpName, "wb");
	if (fp == 0) return 0;

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ  
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;//bmp����  

							 //bfSize��ͼ���ļ�4����ɲ���֮��  
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ colorTablesize + lineByte*height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;

	//bfOffBits��ͼ���ļ�ǰ������������ռ�֮��  
	fileHead.bfOffBits = 54 + colorTablesize;

	//д�ļ�ͷ���ļ�  
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ  
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
	//дλͼ��Ϣͷ���ڴ�  
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//����Ҷ�ͼ��,����ɫ��,д���ļ�   
	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//дλͼ���ݽ��ļ�  
	//fwrite(imgBuf, height*lineByte, 1, fp);  
	fwrite(imgBuf, InfLen / 8, 1, fp);


	//�ر��ļ�  
	fclose(fp);

	return 1;
}


bool CHuffman::readBmp(char *bmpName)
{
	//�����ƶ���ʽ��ָ����ͼ���ļ�  
	FILE *fp = fopen(bmpName, "rb");
	if (fp == 0)
	{
		printf("δ�ҵ�ָ���ļ���\n");
		return 0;
	}


	//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);


	//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��  
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//��ȡͼ����ߡ�ÿ������ռλ������Ϣ  
	bmpWidth = head.biWidth;
	bmpHeight = head.biHeight;
	biBitCount = head.biBitCount;

	//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı��������ԻҶ�ͼ��˵����ͼ���ȣ�Ȼ����ܲ���4�ı��������䵽4�ı���������409��412
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;

	//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256  
	if (biBitCount == 8)
	{
		//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�  
		pColorTable = new RGBQUAD[256];
		fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	}

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�  
	pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	//�ر��ļ�  
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
	//int nrow = imgTemp.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	//BYTE *psrc = (BYTE *)imgTemp.GetBits();//���m_CImage���һ�е����ص�ַ
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

	//����ÿ���Ҷ�ֵ���ֵĴ���  (ͳ��ֱ��ͼ) 
	for (int i = 0; i < bmpHeight; i++)
		for (int j = 0; j < bmpWidth; j++)
		{
			lpBuf = (unsigned char *)pBmpBuf + lineByte * i + j;
			hist[*(lpBuf)] += 1;
		}

	//���ñ���  
	HuffmanCode(); //
	saveBmp("huffman.bhd", pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
	saveInfo("huffman.bpt", lineByte);
	delete[] pBmpBuf;
	if (biBitCount == 8)
		delete[] pColorTable;

	//��������ʼ��  
	HuffmanCodeInit();
	readHuffman("huffman");
	HuffmanDecode();    //Huffman����  
	InfLen = bmpWidth * bmpHeight * 8;
	saveBmp("huffman_decode.bmp", pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);
	//�ٶ��뱣���ͼ
	dstImg.Load(L"huffman_decode.bmp");

	delete[] ImgInf;
}
