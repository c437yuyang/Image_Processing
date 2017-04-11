#pragma once
class CHuffmanCoding
{
public:
	CHuffmanCoding();
	~CHuffmanCoding();

	struct Node {        //Huffman���ڵ�  
		int color;      //��¼Ҷ�ӽڵ�ĻҶ�ֵ����Ҷ�ӽڵ�Ϊ -1��  
		int lson, rson;  //�ڵ�����Ҷ��ӣ���û����Ϊ -1��  
		int num;        //�ڵ����ֵ���������ݣ�  
		int mark;       //��¼�ڵ��Ƿ��ù�(�ù�Ϊ1��û�ù�Ϊ0)  
	}Node;

	struct Node * Nodes = new struct Node[1000]; //һ����˵������(���2n-1��nΪҶ�ڵ���Ŀ)

	int NodeNum;    //Huffman���ܽڵ����  
	int m_NodeStart;  //Huffman����ʼ�ڵ�  
	//char CodeStr[1000][1000]; //��¼����ֵ
	char **CodeStr;
	//int CodeLen[1000];       //���볤��  
	int *CodeLen;       //���볤��  

	bool *m_codes;   //ͼ����Ϣ(�������Ҫ��̬������У�ֱ�ӷ���8000000��ֱ�ӱ���)
	int m_nCodesLen; //ͼ����Ϣ����  
	char m_chCodeTemp[1000];

	int doHuffmanCoding(int *hist, int len,char **, int *codelen);
	int MinNode();
	void dfs(int pos, int len);
};

