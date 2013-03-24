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
	/* �ʵ����                                                                     */
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
	/* ���ƥ��ִ����                                                                     */
	/************************************************************************/
	bolan_t bolan[MAX_WORD_PER_SENTENCE]; //����Ҫ��ʵ�ʴ�����һ��
	max_match_seg_t mseg;
	//char *sent = "�ϵ���·̩˼�ش���";
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
	//�����ʷִ�
	max_prob_seg_t seger;

//	bolan_t bolan[MAX_WORD_PER_SENTENCE]; //����Ҫ��ʵ�ʴ�����һ��
	
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
