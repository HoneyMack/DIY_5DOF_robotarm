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
	char words[MAX_WORDS_NUM][MAX_LEN]; //orderを区切り文字で分割した単語たちを保持


	//文字列を区切り文字" "で分割
	p = strtok(order," ");
	//オペランドがなくなるまで続ける
	while (p != NULL) {
        //オペランドの部分をコピー
		strcpy(words[w_count++], p);
		//文字列（オペランド)を区切り文字" "で分割
		p = strtok(NULL," ");
	}
	for(i = 0;i < w_count;i++){
        printf("%s\n",words[i]);
	}

}
