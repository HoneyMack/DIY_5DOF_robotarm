#include <stdio.h>
#include <string.h>

#define NULL 0

#define MAX_LEN 8
#define MAX_WORDS_NUM 8


void main(){
    int i;
	int w_count = 0;
	char *p;
	char order[64] = {"G1 X100 Y10 Z1 "};
	char words[MAX_WORDS_NUM][MAX_LEN]; //order����؂蕶���ŕ��������P�ꂽ����ێ�


	//���������؂蕶��" "�ŕ���
	p = strtok(order," ");
	//�I�y�����h���Ȃ��Ȃ�܂ő�����
	while (p != NULL) {
        //�I�y�����h�̕������R�s�[
		strcpy(words[w_count++], p);
		//������i�I�y�����h)����؂蕶��" "�ŕ���
		p = strtok(NULL," ");
	}
	for(i = 0;i < w_count;i++){
        printf("%s\n",words[i]);
	}

}
