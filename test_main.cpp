#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "max_match_seg.h"
#include "max_prob_seg.h"

int main(int argc,char**argv) {
	
	//FILE *fp = fopen("C:\\Users\\Administrator\\Desktop\\wc\\seg_freq.txt","r");
	//FILE *fp = fopen("dict_tmp.txt","r");
	char buf[100];
	
	dict_t dict;
	//dict.load_dict("cc.dct");
	char word[50];
	int cnt;
	/************************************************************************/
	/* 词典测试                                                                     */
	/************************************************************************/
	/*while(fgets(buf,sizeof(buf),fp)) {
		if(strlen(buf) <= 0) continue;
		sscanf(buf,"%s\t%d",word,&cnt);
		printf("%d %s\n",cnt,word);
		dict.add_item(word,'Z',cnt);
	}
	
	remove("cc.dct");
*/
//	dict.save_dict("cc.dct");  


	/************************************************************************/
	/* 最大匹配分词设计                                                                     */
	/************************************************************************/
	bolan_t bolan[MAX_WORD_PER_SENTENCE]; //必须要比实际词数多一个
	max_match_seg_t mseg;
	//char *sent = "上地西路泰思特大厦";
	char sent[100];
	memset(sent,0,sizeof(sent));
	strcpy(sent,argv[1]);
	int count=0;
	mseg.m_dict.load_dict("cc.dct");
	mseg.cutsentence(sent,bolan,&count);
	for(int i=0;i<count;++i) {
		printf("%s\t",bolan[i].word/*,seger.m_bolan[i].ist*/);
	}
	
	printf("\n");
	//最大概率分词
	max_prob_seg_t seger;

//	bolan_t bolan[MAX_WORD_PER_SENTENCE]; //必须要比实际词数多一个
	
	seger.m_dict.load_dict("cc.dct");
	//seger.cutsentence(sent,bolan,&cnt);
	seger.build_graph(sent);
	for(int i=0;i<seger.m_nbolan;++i) {
			printf("%s\t",seger.m_bolan[i].word/*,seger.m_bolan[i].ist*/);
	}
	printf("\n");

	seger.kpath();
	return 0;
}
