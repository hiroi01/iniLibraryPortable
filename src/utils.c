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



// strtok�֐��̑���(NULL�w��s��)
// delim�Ŏw�肵���������str����T���ĕ��f����
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
	if(end == NULL){// \n��������Ȃ��ꍇ / if not found \n
		line[len] = '\0';
		return len;
	}
	
	end[1] = '\0';
	sceIoLseek(fp, - (len - (end - line + 1)), PSP_SEEK_CUR);

	return end - line +	1;
}

//�킽���ꂽ������̃X�y�[�X����щ��s�R�[�h����菜��
int ILPRemoveSpaceAndNewline(char *str)
{
	int i,j;
	
	for( i = 0; str[i] != '\0'; i++ ){
		if( ILPISSPACE(str[i]) ){
			for( j = i; str[j] != '\0'; j++ ) str[j] = str[j+1]; //1�����l�߂�
			i--;
		}
	}
	return i;
}

