/*
 INI Library Portable by hiroi01 ver. 1.00
 
 bases are 
 mp3play and bright.prx by plum
 and,
 iso tool by takka
 
 thanks to  plum , takka !
 
 */

#include <string.h>
#include <stdio.h>

#include <libinip.h>



// strtok関数の代わり(NULL指定不可)
// delimで指定した文字列をstrから探して分断する
char *ILPChToken(char *str, const char *delim)
{
	char *ptr = strstr(str, delim);
	if(ptr == NULL){
		return NULL;
	}
	
	*ptr = '\0';
	return ptr + strlen(delim);
}

int ILPIsSkipLine(const char *str)
{
	int i;
	
	
	for( i = 0; str[i] != '\0' ; i++ ){
		if( ! ILPISSPACE(str[i]) ) break;
	}
	if( str[i] == '\0' || str[i] == '#' ){
		return 1;
	}
	
	return 0;
}

//base is in iso tool
//thanks to takka
int ILPReadLine(SceUID fp, char* line, int maxline)
{
	char* end;
	int len;	
	
	len = sceIoRead(fp, line, maxline - 1);
	if(len == 0){//error
		return 0;
	}
	
	end = strchr(line, '\n');
	if(end == NULL){// \nが見つからない場合 / if not found \n
		line[len] = '\0';
		return len;
	}
	
	end[1] = '\0';
	sceIoLseek(fp, - (len - (end - line + 1)), PSP_SEEK_CUR);

	return end - line +	1;
}

//わたされた文字列のスペースおよび改行コードを取り除く
int ILPRemoveSpaceAndNewline(char *str)
{
	int i,j;
	
	for( i = 0; str[i] != '\0'; i++ ){
		if( ILPISSPACE(str[i]) ){
			for( j = i; str[j] != '\0'; j++ ) str[j] = str[j+1]; //1文字詰める
			i--;
		}
	}
	return i;
}

