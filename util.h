#ifndef UTIL_T_H
#define UTIL_T_H

#include <string.h>

//gb2312 ���ָ���
#define CC 6778 
#define MAX_WORD_PER_SENTENCE 100
#define gb2312_h( he, lw)  (((he)-176) * 94 + ((lw)-161))

#define isfullchar(he,lw)  ((((he)==161) && ((lw)==161)) || ((he)==163))

#define fulltohalfchar(he,lw)  ((he)==161? ' ':(lw-128))
 
#define MAX_FREQUENCE 1000000

#define TOP_K 50

#define INF 0x3f3f3f3f

typedef unsigned char _uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char _int8_t;
typedef short int16_t;
typedef int int32_t;

#ifdef __GNUG__
typedef unsigned long long uint64_t;
typedef long long int64_t;
#else
typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;
#endif

/************************************************************************/
/*       �ҵ�֮���������������ƥ��                                                    */
/************************************************************************/
typedef struct interval_t {
	int first, last;
	interval_t():first(-1),last(-1){}
	interval_t(int f,int l):first(f),last(l){}
	bool ispoint() { return first == last; }
	void reset(){ last = -1; first = -1; }
	bool isvalid() { return last >= first; }
}*pinterval_t;

//���岨�����ʽ�ṹ�� 
//�洢������͵���
typedef struct bolan_t
{
	int    flag;       //��ʶ�ýڵ��ǵ��ʻ��������, "0":�����.", 1, ȫ�����ֻ���ȫ���ַ��� 2 ��ͨ�ַ���������,   3�� �ʣ� 4 ���ƥ��ʱ��ɵĴʣ�  100"ʱ��ʾ������101 ��ʾ������ʶ���ַ�
	char    word[50];     //�洢�ʵ�ָ��
	int wordlen;
	//int ist ;//�����ھ����е���ʼλ��
	
	bool add(char*s,int size) {
		if(flag == 0) return false;
		int i=0;
		while(i<size) word[wordlen++]=s[i++];
		word[wordlen]='\0';
		return true;
	}

	bolan_t():flag(-1),wordlen(0) { memset(word,0,sizeof(word)); }
	bolan_t(int f,char*s,int len/*,int st=-1*/):flag(f){
		memset(word,0,sizeof(word));
		strncpy(word,s,len);
		wordlen = len;
	//	ist = st;
	}

	bolan_t& operator = (const bolan_t& bolan) {
		flag = bolan.flag;
		memset(word,0,sizeof(word));
		wordlen = bolan.wordlen;
		strncpy(word,bolan.word,wordlen);
		//ist = bolan.ist;
		return *this;
	}

	bool ismerge() {
		return (flag == 1 || flag == 2);
	} 

}*pbolan_t;


struct edge_t {
	double cost;
	int idx;
	edge_t():cost(INF),idx(-1){}
	edge_t(double _c,int _i):cost(_c),idx(_i){}
};


//���������յ�
#define  BYTE_NUMBER_START      0x30
#define  BYTE_NUMBER_END        0x39

//СдӢ���ַ������յ�
#define  BYTE_LOWER_CASE_START      0x61
#define  BYTE_LOWER_CASE_END        0x7A
//��дӢ���ַ������յ�
#define  BYTE_UPPER_CASE_START      0x41
#define  BYTE_UPPER_CASE_END        0x5A


#define EPS (1e-8)

#ifndef ZERO
#define ZERO(a)  (((a)>EPS) - ((a)<-EPS)) 
#endif
#endif
