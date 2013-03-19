#ifndef MAX_PROB_SEG_H
#define MAX_PROB_SEG_H

#include "util.h"
#include "dict.h"
#include "max_match_seg.h"
#include <vector>
using namespace std;

class max_prob_seg_t
{
public:
	max_prob_seg_t();
	~max_prob_seg_t();

	bolan_t m_bolan[MAX_WORD_PER_SENTENCE];
	int m_nbolan;
	int m_row,m_col;

	//邻接表
	edge_t m_adj[MAX_WORD_PER_SENTENCE][MAX_WORD_PER_SENTENCE];
	
	dict_t m_dict;
	char m_sentence[260];

	bool build_graph(char* sentence);

	/************************************************************************/
	/* 获得前k短的path                                                                     */
	/*********************************************************************
***/
	void kpath() ;
	//void generate_seg() ;
protected:
private:
	//max_match_seg_t m_match_seg;
	bool match(int innercode,char*s,int step,pinterval_t interval);
	void add_edge(int u,int v,double cost,int );
};


#endif