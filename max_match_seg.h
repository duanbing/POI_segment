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
	  �� interval�����ÿ��poi�ĵ�start_pos��ʼ����s��
	  �ҵ�֮�󽫷�Χ��interval����  
	************************************************************************/
	bool match(int innercode,char*s,int start_pos,pinterval_t interval) ;
	/************************************************************************/
	/*   �ж�s�Ƿ��������Ĵ���                                                                   */
	/************************************************************************/
	bool match_all(int innercode,char*s,int start_pos);

	/************************************************************************/
	/*  �з�һ������ ,�зֽ����  segout����                                              */
	/************************************************************************/
	bool cutsentence (char *sentence,bolan_t *segout,int*);

	/************************************************************************/
	/*  �ϲ������ַ���������(����ȫ�ǵ�)                                          */
	/************************************************************************/
	void merge(bolan_t *bolan,int *count);
 
};



#endif