#pragma once
#include "math.h"  
#include <time.h>  
#include <string>
#include "MyImage_.h"

class CHuffman
{
private:
	struct Node {        //Huffman树节点  
		int color;      //记录叶子节点的灰度值（非叶子节点为 -1）  
		int lson, rson;  //节点的左右儿子（若没有则为 -1）  
		int num;        //节点的数值（编码依据）  
		int mark;       //记录节点是否被用过(用过为1，没用过为0)  
	}node[600];

	unsigned char *pBmpBuf;//读入图像数据的指针  
	int bmpWidth;//图像的宽  
	int bmpHeight;//图像的高  
	RGBQUAD *pColorTable;//颜色表指针  
	int biBitCount;//图像类型  
	char str[100];//文件名称   
	int Num[300];//各灰度值出现的次数   
	float Feq[300];//各灰度值出现的频率   
	unsigned char *lpBuf;//指向图像像素的指针  
	unsigned char *m_pDib;//存放打开文件的DIB  

	int NodeNum;    //Huffman树总节点个数  
	int NodeStart;  //Huffman树起始节点  

	char CodeStr[300][300]; //记录编码值  
	int CodeLen[300];       //编码长度  
	//bool ImgInf[8000000];   //图像信息
	bool *ImgInf;   //图像信息(这里必须要动态分配才行，直接分配8000000会直接报错)
	int InfLen;             //图像信息长度  
	char CodeTmp[300];

public:
	CHuffman();
	~CHuffman();

	int Change2to10(int pos);
	int saveInfo(char * writePath, int lineByte);
	bool readHuffman(char *Name);
	void HuffmanDecode();
	void HuffmanCodeInit();
	void dfs(int pos, int len);
	int MinNode();
	void HuffmanCode();
	bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable);
	bool readBmp(char * bmpName);
public:
	void DoHuffmanCode(const MyImage_ &srcImg,MyImage_ &dstImg);
};
