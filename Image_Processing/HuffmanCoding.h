#pragma once
class CHuffmanCoding
{
public:
	CHuffmanCoding();
	~CHuffmanCoding();

	struct Node {        //Huffman树节点  
		int color;      //记录叶子节点的灰度值（非叶子节点为 -1）  
		int lson, rson;  //节点的左右儿子（若没有则为 -1）  
		int num;        //节点的数值（编码依据）  
		int mark;       //记录节点是否被用过(用过为1，没用过为0)  
	}Node;

	struct Node * Nodes = new struct Node[1000]; //一般来说够用了(最大2n-1，n为叶节点数目)

	int NodeNum;    //Huffman树总节点个数  
	int m_NodeStart;  //Huffman树起始节点  
	//char CodeStr[1000][1000]; //记录编码值
	char **CodeStr;
	//int CodeLen[1000];       //编码长度  
	int *CodeLen;       //编码长度  

	bool *m_codes;   //图像信息(这里必须要动态分配才行，直接分配8000000会直接报错)
	int m_nCodesLen; //图像信息长度  
	char m_chCodeTemp[1000];

	int doHuffmanCoding(int *hist, int len,char **, int *codelen);
	int MinNode();
	void dfs(int pos, int len);
};

