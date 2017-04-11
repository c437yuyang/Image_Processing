#pragma once
#include "math.h"  
#include <time.h>  
#include <string>
#include "MyImage_.h"
#include <iostream>
using namespace std;

class CMyHuffman
{
public:
	CMyHuffman();
	~CMyHuffman();

private:
	struct Node {        //Huffman���ڵ�  
		int color;      //��¼Ҷ�ӽڵ�ĻҶ�ֵ����Ҷ�ӽڵ�Ϊ -1��  
		int lson, rson;  //�ڵ�����Ҷ��ӣ���û����Ϊ -1��  
		int num;        //�ڵ����ֵ���������ݣ�  
		int mark;       //��¼�ڵ��Ƿ��ù�(�ù�Ϊ1��û�ù�Ϊ0)  
	}node[600];
	unsigned char *pBmpBuf;//����ͼ�����ݵ�ָ��  
	int bmpWidth;//ͼ��Ŀ�  
	int bmpHeight;//ͼ��ĸ�  
	RGBQUAD *pColorTable;//��ɫ��ָ��  
	int biBitCount;//ͼ������  
	char str[100];//�ļ�����   
	int hist[300];//���Ҷ�ֵ���ֵĴ���   
	float Feq[300];//���Ҷ�ֵ���ֵ�Ƶ��   
	unsigned char *lpBuf;//ָ��ͼ�����ص�ָ��  
	unsigned char *m_pDib;//��Ŵ��ļ���DIB  

	int NodeNum;    //Huffman���ܽڵ����  
	int m_NodeStart;  //Huffman����ʼ�ڵ�  

	char CodeStr[300][300]; //��¼����ֵ  
	int CodeLen[300];       //���볤��  
							//bool ImgInf[8000000];   //ͼ����Ϣ
	bool *ImgInf;   //ͼ����Ϣ(�������Ҫ��̬������У�ֱ�ӷ���8000000��ֱ�ӱ���)
	int InfLen;             //ͼ����Ϣ����  
	char CodeTmp[300];
	void HuffmanDecode();
	void HuffmanCodeInit();
	void dfs(int pos, int len);
	int MinNode();
	void HuffmanCode();
public:
	void DoHuffmanCode(const MyImage_ &srcImg, MyImage_ &dstImg);
};

