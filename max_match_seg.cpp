#include "max_match_seg.h"


//判断一个字符是否是操作符
bool isoperator(char &ch) 
{
	switch(ch)
	{
	case '+':
	case '|':
	case '(':
	case ')':
	case '[':
	case ']':
	case '{':
	case '}':
		return true;
	case ' ':
	case ',':
	case ';':
	case '.':
	case '!':
		return true;
	}	
	return false;
}

max_match_seg_t::max_match_seg_t(){}
max_match_seg_t::~max_match_seg_t(){}

bool max_match_seg_t::cutsentence(char *sentence,bolan_t *segbolan,int* segcount) {
	*segcount = 0;
	int step = 0,innercode,len;
	char *strpos = sentence;
	len = strlen(sentence);
	unsigned char ch,ch2;
	interval_t interval;
//	bool isallmath;
	while(*strpos && len >= step) {
		
		len -= step;
		step = 0;
		
		// 处理全角字符
		ch = *strpos;  ch2 = *(strpos + 1);
		if(len > step + 1 && isfullchar(ch,ch2)) {
			segbolan[*segcount] = bolan_t(1,strpos,2);
			++(*segcount);
			strpos += 2;
			step += 2;
			continue;
		}
	//	isallmath = false;
		//处理汉字
		if(ch & 0x80) {
			if( ch > 0x80) {
				innercode = gb2312_h(ch,ch2);
				while(len > step && match(innercode,strpos,step+2,&interval)) {

					if(interval.ispoint() || 	(strlen(strpos)<=step+2) ) {
						worditem_t *_tmp=m_dict.m_index_table[innercode].pworditem+interval.first;
						if(strncmp(_tmp->word, strpos+2 ,_tmp->wordlen)==0) step += _tmp->wordlen; 
						segbolan[*segcount]=bolan_t(4,strpos,step+2);
						++(*segcount);
						break;
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
					segbolan[*segcount] = bolan_t(0,strpos,1);
					++(*segcount);
					step += 1;
			} else {   //普通字符和数字
				segbolan[*segcount] = bolan_t(2,strpos,1);
				++(*segcount);
				step += 1;
			}
		}

		strpos += step;  //
		interval.reset();
	}
	
	merge(segbolan,segcount);

	return true;
} 

void max_match_seg_t::merge(bolan_t *bolan,int *count) {
	int cnt = *count;
	*count = 0;
	int i = 0;
	bolan_t bl[MAX_WORD_PER_SENTENCE];
	while(i < cnt) {
		int j = i + 1;
		while(j<cnt &&  bolan[j].ismerge() && (bolan[i].flag == bolan[j].flag)) ++j;
		bl[(*count)].flag = bolan[i].flag;
		bl[(*count)].wordlen = bolan[i].wordlen;
		memset(bl[(*count) ].word,0,sizeof(bl[(*count) ].word));
		for(int r=i;r<j;++r) strcat(bl[(*count) ].word,bolan[r].word);
		++(*count);
		i = j;
	}
	for(i=0;i<*count;++i) {
		bolan[i] = bl[i];
	}
}

bool max_match_seg_t::match(int innercode,char*s,int start_pos,pinterval_t interval) {
	
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

bool max_match_seg_t::match_all(int innercode,char*s,int start_pos) {
	return true;
}
