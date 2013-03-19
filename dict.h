#ifndef DICT_T_H
#define DICT_T_H

#include <string.h>
#include "util.h"

typedef struct worditem_t {
	int wordlen;
	char word[20];
	int handle;
	int nfreq;  
	worditem_t():handle(-1),nfreq(-1),wordlen(0){ memset(word,0,sizeof(word)); }
	worditem_t(int _len,char *_s,int _handle,int _nf):wordlen(_len),handle(_handle),nfreq(_nf){
		memset(word,0,sizeof(word));
		strcpy(word,_s);
	}
}*pworditem_t;

typedef struct indextable_t {
	int count;
	worditem_t *pworditem;
	indextable_t():count(0),pworditem(NULL){}
}*pindextable_t;

typedef struct wordchain_t 
{
	worditem_t data;
	wordchain_t *next;
	wordchain_t():next(NULL){}
}*pwordchain_t;

typedef struct modifytable_t {
	int count;
	int ndelete;
	wordchain_t *pwordchain;
	modifytable_t():count(0),ndelete(0),pwordchain(NULL) {}
}*pmodifytable_t;

class dict_t {
public:
	indextable_t m_index_table[CC];
	modifytable_t m_modify_table[CC];

public:
	dict_t();
	~dict_t();
	bool  load_dict (char *p) ;
	bool save_dict(char*p) ;
	bool bifind(int ninnercode,char*s,int handle,int *position) ;
	bool chainfind(int ninnercode,char*s,int handle,pwordchain_t*chain); 
	void add_item(char*s,int handle,int freq);
	void del_item(char*s ,int handle);
	bool slim(char*s,int *innercode);
};

#endif