#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dict_t::dict_t() {}
dict_t::~dict_t(){
	for(int i=0;i<CC;++i) {
		pwordchain_t p=m_modify_table[i].pwordchain,pcur;
		while (p != NULL) {
			pcur = p;
			p = p->next;
			delete pcur;
		}
	}
}

bool dict_t::load_dict(char *p) {
	FILE *fp = fopen(p,"rb");
	if(!fp) return false;
	indextable_t *pit = NULL;
	for(int i=0;i<CC;++i) {
		int ncount ;
		fread(&ncount,sizeof(int),1,fp);
		m_index_table[i].count = ncount;
		m_index_table[i].pworditem = new worditem_t[ncount];
		
		for(int j=0;j<ncount;++j) {
			worditem_t *wi = m_index_table[i].pworditem + j;
			fread(&wi->wordlen,sizeof(int),1,fp);
			fread(wi->word,sizeof(char),wi->wordlen,fp);
			fread(&wi->handle,sizeof(int),1,fp);
			fread(&wi->nfreq,sizeof(int),1,fp);
		}
	}
	return true;
}

inline void wordwrite(worditem_t* word,FILE *fp)  {
	fwrite(&(word->wordlen),sizeof(int),1,fp); fwrite(word->word,sizeof(char),word->wordlen,fp) ;
	fwrite(&(word->handle),sizeof(int),1,fp); fwrite(&(word->nfreq),sizeof(int),1,fp);
}

bool dict_t::save_dict(char*p){
	FILE *fp = fopen(p,"wb");
	if(!fp)  return false;

	wordchain_t *chain = NULL;
	worditem_t *item = NULL;

	int ncount;
	char buf[1000];

	for(int i=0,j;i<CC;++i) {
		chain = m_modify_table[i].pwordchain;
		j = 0;
		ncount = m_index_table[i].count + m_modify_table[i].count - m_modify_table[i].ndelete;
		fwrite(&ncount,sizeof(int),1,fp);
		while(chain && j < m_index_table[i].count) {
			item = m_index_table[i].pworditem;
			int cmpval = strcmp(chain->data.word,item[j].word);
			if(cmpval < 0 || (cmpval == 0 && chain->data.handle < item[j].handle)) {
				//sprintf(buf,"%04d%s%04d%04d",chain->data.wordlen,chain->data.word,chain->data.handle,chain->data.nfreq);
				//fwrite(buf,strlen(buf),1,fp);
				wordwrite(&(chain->data),fp);
				chain = chain->next;
			} else if(item[j].nfreq < 0) {
				++j;
			} else if(cmpval > 0 || (cmpval == 0 && chain->data.handle > item[j].handle)) {
				/*sprintf(buf,"%04d%s%04d%04d",item[j].wordlen,item[j].word,item[j].handle,item[j].nfreq);
				fwrite(buf,strlen(buf),1,fp);*/
				wordwrite(item + j,fp);
				++j;
			}
		}
		
		while(j < m_index_table[i].count) {
		/*	sprintf(buf,"%04d%s%04d%04d",item[j].wordlen,item[j].word,item[j].handle,item[j].nfreq);
			fwrite(buf,strlen(buf),1,fp);*/
			wordwrite(item + j,fp);
			++j;
		} 

		while(chain) {
		/*	sprintf(buf,"%04d%s%04d%04d",chain->data.wordlen,chain->data.word,chain->data.handle,chain->data.nfreq);
			fwrite(buf,strlen(buf),1,fp);*/
			wordwrite(&(chain->data),fp);
			chain = chain->next;
		}
	}
	fclose(fp);
	return true;
}

bool dict_t::slim(char*s,int *innercode) {
	
	int i, len = strlen(s);
	char *l = s,*r = s+len-1;
	while(*l == ' ')++l; 	while(*r == ' ')--r;
	char buf[200];
	for(i=0;l<=r;++i,++l) buf[i] = *l; 
	buf[i] = '\0';
	*innercode = gb2312_h((uint8_t)buf[0],(uint8_t)buf[1]);
	if(*innercode < 0) //出现非汉字
		return false;
	strcpy(s,buf);
	return true;
}

void dict_t::add_item(char*s,int handle,int freq) {
	int pos=-1;
	int innercode = -1;
	if(!slim(s,&innercode)) return ;
	char *stmp = s + 2;//减去第一个字符
	if(bifind(innercode,stmp,handle,&pos)) {
		if(m_index_table[innercode].pworditem[pos].nfreq==-1) 
			m_index_table[innercode].pworditem[pos].nfreq=0;
		m_index_table[innercode].pworditem[pos].nfreq += freq;
		return ;
	} 
	
	if(strncmp("埃",s,2) == 0) {
		puts("--");
	}

	wordchain_t *chain = NULL;
	if(chainfind(innercode,stmp,handle,&chain)) {
		if(chain) chain = chain->next;
		else chain = m_modify_table[innercode].pwordchain;
		if(chain) {
			if(chain->data.nfreq==-1) chain->data.nfreq = 0;
			chain->data.nfreq += freq;
		}
		return ;
	}

	wordchain_t* newchain = new wordchain_t;
	newchain->data = worditem_t(strlen(stmp),stmp,handle,freq);

	if(chain) {
		newchain->next = chain->next;
		chain->next = newchain;
	}  else {
		newchain->next = m_modify_table[innercode].pwordchain;
		m_modify_table[innercode].pwordchain = newchain;
	}
	m_modify_table[innercode].count ++ ;
}

void dict_t::del_item(char*s ,int handle) { 
	int innercode,pos = -1;
	wordchain_t *pre= NULL,*pcur;
	slim(s,&innercode);
	if(bifind(innercode,s,handle,&pos)) {
		m_index_table[innercode].pworditem[pos].nfreq = -1;
		++ m_modify_table[innercode].ndelete;
		if(handle == -1) {
			int nxtpos = pos + 1;
			while(nxtpos < m_index_table[innercode].count && strcmp(m_index_table[innercode].pworditem[nxtpos].word,s) == 0) {
				m_index_table[innercode].pworditem[nxtpos].nfreq = -1;
				++ m_modify_table[innercode].ndelete;
				++nxtpos;
			}
		}
		return ;
	}

	if(chainfind(innercode,s,handle,&pre)) {
		if(pre)
			pcur = pre->next;
		while(pcur && strcmp(pcur->data.word,s) == 0) {
			++ m_modify_table[innercode].ndelete;
			pcur->data.nfreq = -1;
			pre = pcur;
			pcur = pcur->next;

			delete pre->data.word;
			delete pre;
		}
		return ;
	}
}

bool dict_t::chainfind(int ninnercode,char*s,int handle,pwordchain_t*chain) {
	wordchain_t *pchain = m_modify_table[ninnercode].pwordchain, *pre=NULL;
	while (pchain && (strcmp(pchain->data.word,s)<0 || (strcmp(pchain->data.word,s) == 0 && pchain->data.handle < handle)))
	{
		pre = pchain;
		pchain = pchain->next;
	}
	*chain = pre;  //返回前一个
	if(pchain && strcmp(pchain->data.word,s) == 0 && (handle == -1 || pchain->data.handle == handle)) {	
		return true;
	}
	return false;
}

bool dict_t::bifind(int ninnercode,char*s,int handle,int *position) {
	worditem_t *pword = m_index_table[ninnercode].pworditem;
	int count = m_index_table[ninnercode].count;

	int l = 0, r = count , mid;
	char *tmps = s;
	while(l < r) {
		mid = (l + r)>>1;
		int ret = strcmp(pword[mid].word,tmps); 
		if(ret == 0 && (pword[mid].handle == handle || handle == -1)) {
			if(handle == -1) {
				while(mid >=0 && strcmp(pword[mid].word,tmps) == 0) --mid;
				if(mid < 0) ++mid;
			} 
			*position = mid;
			return true;
		} else if(ret < 0 || (ret == 0 && pword[mid].handle < handle)) {
			l = mid+1;
		} else if(ret > 0 || (ret == 0 && pword[mid].handle > handle)) {
			r = mid;
		}
	}
	return false;
}

