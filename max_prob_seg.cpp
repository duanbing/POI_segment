#include <string.h>
#include "max_prob_seg.h"
#include "kspath.h"
#include <ctype.h>
#include <math.h>
#include <queue>
using namespace std;

max_prob_seg_t::max_prob_seg_t() {
	m_nbolan = 0;
	
	m_row = 0;
	m_col = 0;
	
}

max_prob_seg_t::~max_prob_seg_t() {}

bool max_prob_seg_t::match(int innercode,char*s,int start_pos,pinterval_t interval) {
	if(strlen(s)<=start_pos)  return true;
	//if(!s) return true;
	if(m_dict.m_index_table[innercode].count<=interval->last) return false;
	worditem_t *worditem = m_dict.m_index_table[innercode].pworditem;

	if(interval->last < 0)  {
		interval->first = 0;
		interval->last = m_dict.m_index_table[innercode].count-1;
	}
	int mid=-1, left = interval->first, right = interval->last + 1;
	char * strtmp = NULL;

	//找出最左边
	while(left < right) {  
		mid = (left+right)/2;	
		if( (worditem[mid].wordlen < start_pos)  || 
			strncmp(s + start_pos , worditem[mid].word,2)  > 0)  left = mid + 1;
		else if(strncmp(s + start_pos , worditem[mid].word,2) <= 0) right = mid;
	}

	//	if(strncmp(s + start_pos , worditem[mid].word+start_pos,2) < 0) --mid;

	right = interval->last + 1;

	//if( strncmp(worditem[left].word ,s,worditem[left].wordlen) == 0 ) {
	// *allmatch = true;
	// *addstep = worditem[left].wordlen - start_pos;
	// //start_pos = worditem[mid].wordlen;
	//}

	mid = left;
	interval->first = left;


	//找出最右边
	while(left < right) {  
		mid = (left+right)/2;	
		if( (worditem[mid].wordlen < start_pos)  || 
			strncmp(s + start_pos , worditem[mid].word,2)  >= 0)  left = mid + 1;
		else if(strncmp(s + start_pos , worditem[mid].word,2) < 0) right = mid;
	}

	interval->last = mid;
	// if(*addstep == 0 && interval->isvalid()) *addstep = 2;
	return true;
}

void max_prob_seg_t::add_edge(int u,int v,double cost,int idx) {
	m_adj[u][v].cost = cost;
	m_adj[u][v].idx = idx;//边的编号
}

bool max_prob_seg_t::build_graph(char* sentence) {
	
	if(!sentence) return true;
	strcpy(m_sentence,sentence);
	int row = 0,column = 0;
	
	int step = 0,innercode,len,itmp;
	bool isprenum = false;
	bool isprealpha = false;
	bool isrowinc=true;
	char *strpos = sentence;
	len = strlen(sentence);
	unsigned char ch,ch2;
	interval_t interval;
	worditem_t *_tmp=NULL;
	//	bool isallmath;
	while(*strpos && len >= step) {
		isrowinc=true;
		column = row+1;

		len -= step;
		step = 0;
		
		ch = *strpos;  ch2 = *(strpos + 1);
		innercode = gb2312_h(ch,ch2);
		if(isprealpha==false && isprenum == false) {
			if(isdigit(ch) || isalpha(ch)) {
				m_bolan[m_nbolan] = bolan_t(2,strpos,1/*,strpos-sentence*/);
				itmp = 1;
				add_edge(row,column ++,-log((0.0 + itmp)/MAX_FREQUENCE),m_nbolan);
				++m_nbolan;	
			} else if(ch & 0x80){
				m_bolan[m_nbolan] = bolan_t(3,strpos,2/*,strpos-sentence*/);
				itmp = m_dict.m_index_table[innercode].pworditem[0].nfreq;
				itmp=itmp?itmp:1;
				add_edge(row,column ++,-log((0.0 + itmp)/MAX_FREQUENCE),m_nbolan);
				++m_nbolan;	
			} 
			
			
		} else if(ch & 0x80) {  //前面是字母或者字符，接下来是汉字

			++row;
			column = row;

			itmp = m_dict.m_index_table[innercode].pworditem[0].nfreq; 
			m_bolan[m_nbolan] = bolan_t(3,strpos,2/*,strpos-sentence*/);
			add_edge(row,column ++,-log((0.0 + itmp)/MAX_FREQUENCE),m_nbolan);
			++m_nbolan;		
			
		}

		// 处理全角字符
		if(len > step + 1 && isfullchar(ch,ch2)) {
			char isnum_tmp = fulltohalfchar(ch,ch2);

			if(isprenum && isdigit(isnum_tmp)) {
				m_bolan[m_nbolan-1].add(strpos,2);
			}  else if(isprealpha && (isalpha(isnum_tmp)) ){
				m_bolan[m_nbolan-1].add(strpos,2);
			} else {
				if(isdigit(isnum_tmp))  isprenum = true;
				else isprealpha = true;
				m_bolan[m_nbolan-1].flag = 1;

				//m_bolan[m_nbolan] = bolan_t(1,strpos,2);
				//add_edge(row,column++,-log(1.0/MAX_FREQUENCE),m_nbolan);
				//++m_nbolan;
			}
			strpos += 2;
			step += 2;
		
			continue;
		}
		//	isallmath = false;
		//处理汉字
		if(ch & 0x80) {
			if( ch > 0x80) {
				isprealpha = isprenum = false;
				//innercode = gb2312_h(ch,ch2);
				while(len > step && match(innercode,strpos,step+2,&interval)) {

					if(interval.isvalid() || 	(strlen(strpos)<=step+2) ) {
						_tmp=m_dict.m_index_table[innercode].pworditem+interval.first;
						if(strncmp(_tmp->word, strpos+2 ,_tmp->wordlen)==0) //step += _tmp->wordlen; 
						{   
							m_bolan[m_nbolan]=bolan_t(4,strpos,step+2+_tmp->wordlen/*,strpos-sentence*/);
							add_edge(row,column++,-log( (_tmp->nfreq+1.0)/MAX_FREQUENCE),m_nbolan);
							++(m_nbolan);
						}
						if(interval.ispoint()) break;
						//break;
					}
					step += 2;
				}	
				step += 2;
			} 
		} else {
			if(!(ch >= BYTE_NUMBER_START && ch <= BYTE_NUMBER_END
				|| ch >= BYTE_LOWER_CASE_START && ch <= BYTE_LOWER_CASE_END
				|| ch >= BYTE_UPPER_CASE_START && ch <= BYTE_UPPER_CASE_END
				)) {  // 其他字符
					m_bolan[m_nbolan] = bolan_t(0,strpos,1/*,strpos-sentence*/);
					//m_bolan[m_nbolan++] = 0;
					/*	add_edge(row,column++,-log( (1.0)/MAX_FREQUENCE),m_nbolan);*/
					++(m_nbolan);
					step += 1;
					isprenum = isprealpha = false;
			} else {   //普通字符和数字
				isrowinc=false;
				if(isprenum && isdigit(ch)) {
					m_bolan[m_nbolan-1].add(strpos,1);
				}  else if(isprealpha && (isalpha(ch)) ){
					m_bolan[m_nbolan-1].add(strpos,1);
				} 
				if(isdigit(ch))  isprenum = true;
				else isprealpha = true;
				step += 1;
			}
		}
		if(step>2)step = 2;
		strpos += step;  //
		interval.reset();
		if(isrowinc)++row;
	}
	if(!isrowinc) ++row;
	m_row = row;
	m_col = row+1;
	
	return true;
}

void max_prob_seg_t::kpath() {
	int cnt[MAX_WORD_PER_SENTENCE]={0};

	kspath_t ksp(m_row,m_col,m_adj,5);
	
	ksp.cal_kth_path();
	
	for(int r=0;r<ksp.m_nkpath;++r) {
		int id = ksp.idx[r];
		for(int i=1;i<ksp.m_kpath[id].size();++i) {
			int u = ksp.m_kpath[id][i-1] ;
			int v = ksp.m_kpath[id][i];
			printf("%s/",m_bolan[m_adj[u][v].idx].word);
		}
		printf("\n");
	}
}
