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
	struct Node {        //Huffman树节点  
		int color;      //记录叶子节点的灰度值（非叶子节点为 -1）  
		int lson, rson;  //节点的左右儿子（若没有则为 -1）  
		int num;        //节点的数值（编码依据）  
		int mark;       //记录节点是否被用过(用过为1，没用过为0)  
	}node[600];
	unsigned char *m_pImgTemp;//读入图像数据的指针  
	int m_nWidth;//图像的宽  
	int m_nHeight;//图像的高
	int m_nHist[256];//各灰度值出现的次数   
	float Feq[256];//各灰度值出现的频率   
	int NodeNum;    //Huffman树总节点个数  
	int m_NodeStart;  //Huffman树起始节点  
	char CodeStr[300][300]; //记录编码值
	int CodeLen[300];       //编码长度  
	bool *m_codes;   //图像信息(这里必须要动态分配才行，直接分配8000000会直接报错)
	int m_nCodesLen; //图像信息长度  
	char m_chCodeTemp[300];

	void HuffmanDecode();
	void HuffmanCodeInit();
	void dfs(int pos, int len);
	int MinNode();
	void HuffmanCode();
public:
	void DoHuffmanCode(const MyImage_ &srcImg, MyImage_ &dstImg);

};

