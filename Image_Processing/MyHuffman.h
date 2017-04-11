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
	unsigned char *m_pImgTemp;//����ͼ�����ݵ�ָ��  
	int m_nWidth;//ͼ��Ŀ�  
	int m_nHeight;//ͼ��ĸ�
	int m_nHist[256];//���Ҷ�ֵ���ֵĴ���   
	float Feq[256];//���Ҷ�ֵ���ֵ�Ƶ��   
	int NodeNum;    //Huffman���ܽڵ����  
	int m_NodeStart;  //Huffman����ʼ�ڵ�  
	char CodeStr[300][300]; //��¼����ֵ
	int CodeLen[300];       //���볤��  
	bool *m_codes;   //ͼ����Ϣ(�������Ҫ��̬������У�ֱ�ӷ���8000000��ֱ�ӱ���)
	int m_nCodesLen; //ͼ����Ϣ����  
	char m_chCodeTemp[300];

	void HuffmanDecode();
	void HuffmanCodeInit();
	void dfs(int pos, int len);
	int MinNode();
	void HuffmanCode();
public:
	void DoHuffmanCode(const MyImage_ &srcImg, MyImage_ &dstImg);

};

