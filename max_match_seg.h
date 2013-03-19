#ifndef MAX_MATCH_SEG_H
#define MAX_MATCH_SEG_H

#include "dict.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>


class max_match_seg_t {
public:
	dict_t m_dict;
public:
	max_match_seg_t();
	~max_match_seg_t();
	/************************************************************************
	  从 interval里面的每个poi的第start_pos开始查找s，
	  找到之后将范围在interval返回  
	************************************************************************/
	bool match(int innercode,char*s,int start_pos,pinterval_t interval) ;
	/************************************************************************/
	/*   判断s是否是完整的词语                                                                   */
	/************************************************************************/
	bool match_all(int innercode,char*s,int start_pos);

	/************************************************************************/
	/*  切分一个句子 ,切分结果在  segout返回                                              */
	/************************************************************************/
	bool cutsentence (char *sentence,bolan_t *segout,int*);

	/************************************************************************/
	/*  合并单个字符或者数字(包括全角的)                                          */
	/************************************************************************/
	void merge(bolan_t *bolan,int *count);
 
};



#endif