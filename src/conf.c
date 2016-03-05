/*
 
 INI Library Portable ver. 1.01   by hiroi01
 
 thanks to
 mp3play by plum 
 and
 bright.prx by plum
 and
 iso tool by takka
 
 */

#include <pspkernel.h>
#include <pspctrl.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <libinip.h>
/*--------------------------------------------------------*/

#ifndef ILP_NOUSE_DEFAULT_BUTTON_INFO
static ILP_Button_Info defaultButtonInfo[] =  {
	{PSP_CTRL_SELECT,   "SELECT"},
	{PSP_CTRL_START,    "START"},
	{PSP_CTRL_UP,       "UP"},
	{PSP_CTRL_RIGHT,    "RIGHT"},
	{PSP_CTRL_DOWN,     "DOWN"},
	{PSP_CTRL_LEFT,     "LEFT"},
	{PSP_CTRL_LTRIGGER, "LTRIGGER"},
	{PSP_CTRL_LTRIGGER, "L_TRI"},
	{PSP_CTRL_RTRIGGER, "RTRIGGER",},
	{PSP_CTRL_RTRIGGER, "R_TRI"},
	{PSP_CTRL_TRIANGLE, "TRIANGLE"},
	{PSP_CTRL_CIRCLE,   "CIRCLE"},
	{PSP_CTRL_CROSS,    "CROSS"},
	{PSP_CTRL_SQUARE,   "SQUARE"},
	{PSP_CTRL_HOME,     "HOME"},
	{PSP_CTRL_NOTE,     "NOTE"},
	{PSP_CTRL_SCREEN,   "SCREEN"},
	{PSP_CTRL_VOLUP,    "VOLUP"},
	{PSP_CTRL_VOLDOWN,  "VOLDOWN"},
	{0, "+"},
};
#endif

/*--------------------------------------------------------*/

int ILPInitKey(ILP_Key *key)
{
	key[0].buf = 0;
	
	return 0;
}

inline char *ILPGetNameAddressByKeynumber(ILP_Key *key, int keynumber)
{
	return key[keynumber].name;
}

inline void* ILPGetValueAddressByKeynumber(ILP_Key *key, int keynumber)
{
	return key[keynumber].value;
}

char *ILPGetStringFormatValueByKeynumber(ILP_Key *key, char *rtn, int keynumber)
{
	char **listPtr;
	switch (key[keynumber].type) {
		case ILP_TYPE_BUTTON:
			ILPGetButtonNameByKey(rtn, key + keynumber);
			break;
		case ILP_TYPE_BOOL:
			strcpy(rtn, *(bool *)key[keynumber].value?"true":"false");
			break;
		case ILP_TYPE_LIST:
			listPtr = key[keynumber].ex;
			strcpy(rtn, listPtr[*(int *)key[keynumber].value]);
			break;
		case ILP_TYPE_HEX:
			sprintf(rtn, "0x%X", *(u32 *)key[keynumber].value);
			break;
		case ILP_TYPE_STRING:
			strcpy(rtn, "\"");
			strcat(rtn, (char *)key[keynumber].value);
			strcat(rtn, "\"");
			break;
		case ILP_TYPE_DEC:
			sprintf(rtn, "%d", *(int *)key[keynumber].value);
			break;
	}

	return rtn;
}


void ILPSetDefault(ILP_Key *key)
{
	int i;
	int keyNum = key[0].buf;
	
	for( i = 0; i < keyNum; i++ ){
		switch ( key[i].type ) {		
			case ILP_TYPE_LIST:
			case ILP_TYPE_DEC:
				*(int *)key[i].value = key[i].defaultValue.int_;
				break;
			case ILP_TYPE_HEX:
			case ILP_TYPE_BUTTON:
				*(u32 *)key[i].value = key[i].defaultValue.u32_;
				break;
			case ILP_TYPE_STRING:
				strcpy((char *)key[i].value, key[i].defaultValue.str_);
				break;
			case ILP_TYPE_BOOL:
				*(bool *)key[i].value = key[i].defaultValue.bool_;
				break;
		}
	}
	
	return;
}


u32 ILPGetButtonFlagByName(const char* name, ILP_Button_Info *info)
{
	int i;
	if( info == NULL ){
#ifndef ILP_NOUSE_DEFAULT_BUTTON_INFO
		info = defaultButtonInfo;
#else
		return 0;
#endif
	}
	
	for( i = 0; info[i].flag != 0; i++ ){
		if(strcasecmp(name, info[i].name) == 0){
			return info[i].flag;
		}
	}
	
	return 0;
}




char* ILPGetButtonNameByFlag(char *rtn, u32 flag, ILP_Button_Info *info)
{
	int i, isFirst;
	u32 beforeFlag = 0;
	char *ptr = rtn;
	char *delim;
	
	if( info == NULL ){
#ifndef ILP_NOUSE_DEFAULT_BUTTON_INFO
		info = defaultButtonInfo;
#else
		rtn[0] = '\0';
		return rtn;
#endif
	}
	
	
	for( i = 0; info[i].flag != 0; i++ );	
	delim = info[i].name;
	
	isFirst = 1;
	
	for( i = 0; info[i].flag != 0; i++ ){
		if( beforeFlag & info[i].flag ) continue;
		
		if( flag & info[i].flag ){
			if( ! isFirst ){
				sprintf(ptr, " %s ", delim);
				ptr = strchr(ptr, '\0');
			}
			strcpy(ptr, info[i].name);
			ptr = strchr(ptr, '\0');
			isFirst = 0;
		}
		
		beforeFlag |= info[i].flag;
	}
	
	return rtn;
}


inline char* ILPGetButtonNameByKey(char *rtn, ILP_Key *key)
{
	return ILPGetButtonNameByFlag(rtn, *(u32 *)key->value, (ILP_Button_Info *)key->ex);
}



void ILPGetButton(u32 *rtn, const char *str, u32 defaultValue, ILP_Button_Info *info)
{

	char buf[256];
	char *next;
	char *current;
	char *delim;
	int i;

	//init
	*rtn = 0;

	if( info == NULL ){
#ifndef ILP_NOUSE_DEFAULT_BUTTON_INFO
		info = defaultButtonInfo;
#else
		return;
#endif
	}

	strcpy(buf, str);
	current = buf;

	for( i = 0; info[i].flag != 0; i++ );	
	delim = info[i].name;
	
	while(1){
		next = ILPChToken(current, delim);
		*rtn |= ILPGetButtonFlagByName(current, info);
		
		if( next == NULL ){
			break;
		}
		
		current = next;
	}

	if( *rtn == 0 ){
		*rtn = defaultValue;
	}
	
	return;
}



void ILPGetString(char *rtn, const char *str , const char *defaultValue)
{
	int i, j;
	
	for( i = 0; ILPISSPACE(str[i]); i++ );
	
	//"で始まる文字なら
	if( str[i] == '"' ){
		for( i++, j = 0; str[i] != '\0'; i++, j++ ){
			if( str[i] == '"' ){
				rtn[j] = '\0';
				return;
			}
			rtn[j] = str[i];
		}
	}
	
	strcpy(rtn, defaultValue);
	return;
}

void ILPGetNumberFromList(int *rtn, const char *str, int defaultNum, char **list )
{
	
	int i;
	
	for( i = 0; list[i] != NULL; i++ ){
		if( strcasecmp(str,list[i]) == 0 ){
			*rtn = i;
			return;
		}
	}
	
	*rtn = defaultNum;
	return;
}

void ILPGetHex(u32 *rtn, const char *str, u32 defaultNum)
{

	int i;
	*rtn = 0;
	
	i = (str[0] == '0' && str[1] == 'x')?2:0;
	for( ; str[i] != '\0'; i++ ){
		*rtn *= 16;
		switch( str[i] ){
			case '0': *rtn += 0x0; break;
			case '1': *rtn += 0x1; break;
			case '2': *rtn += 0x2; break;
			case '3': *rtn += 0x3; break;
			case '4': *rtn += 0x4; break;
			case '5': *rtn += 0x5; break;
			case '6': *rtn += 0x6; break;
			case '7': *rtn += 0x7; break;
			case '8': *rtn += 0x8; break;
			case '9': *rtn += 0x9; break;
			case 'a':
			case 'A': *rtn += 0xa; break;
			case 'b':
			case 'B': *rtn += 0xb; break;
			case 'c':
			case 'C': *rtn += 0xc; break;
			case 'd':
			case 'D': *rtn += 0xd; break;
			case 'e':
			case 'E': *rtn += 0xe; break;
			case 'f':
			case 'F': *rtn += 0xf; break;
			default :
				*rtn = defaultNum;
				return;
		}
	}
	return;
}

void ILPGetDec(int *rtn, const char *str ,int defaultNum)
{

	int i;
	*rtn = 0;
	for( i = 0; str[i] != '\0'; i++ ){
		*rtn *= 10;
		switch( str[i] ){
			case '0': *rtn += 0; break;
			case '1': *rtn += 1; break;
			case '2': *rtn += 2; break;
			case '3': *rtn += 3; break;
			case '4': *rtn += 4; break;
			case '5': *rtn += 5; break;
			case '6': *rtn += 6; break;
			case '7': *rtn += 7; break;
			case '8': *rtn += 8; break;
			case '9': *rtn += 9; break;
			default :
				*rtn = defaultNum;
				return;
		}
	}
	return;
}

void ILPGetBool(bool *rtn, const char *str, bool defaultValue)
{
	
	*rtn = defaultValue;
	
	if( strcasecmp(str, "true") == 0 ){
		*rtn = true;
	}else if( strcasecmp(str, "false") == 0 ){
		*rtn = false;
	}
	return;
}



int ILPReadFromFile(ILP_Key *key, const char *path)
{

	SceUID fd;
	char buf[256];
	char *ptr, *ptr2;
	int i;
	int keyNum = key[0].buf;

	
	ILPSetDefault(key);

	// 設定ファイル・オープン
	fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
	if(fd < 0){
		return ILP_ERROR_OPEN;
	}	

	
	// ファイル終端までループ
	while ( 1 )
	{
		// 一行読み込む
		if( ILPReadLine(fd, buf, 256) == 0 ) break;

		//飛ばしていい行なら
		if( ILPIsSkipLine(buf) ) continue;
		
		//コメントアウト
		ptr2 = strchr(buf, '#');
		if( ptr2 != NULL ) *ptr2 = '\0';
		
		//=で左辺(buf)右辺(prt)に分けるsplit
		ptr = ILPChToken(buf, "=");
		if(ptr == NULL) continue;//=が見つからないなら
		
		
		//左辺のみ余分なスペースなどを削除
		ILPRemoveSpaceAndNewline(buf);		
		
		
		for( i = 0; i < keyNum; i++ ){
			//左辺を探す
			if( strcasecmp(buf, key[i].name) == 0 ){
				//見つかったらTYPE別に値をget
				switch(key[i].type){
					case ILP_TYPE_HEX:
						//よけいなスペースなどを削除
						ILPRemoveSpaceAndNewline(ptr);
						ILPGetHex((u32 *)key[i].value, ptr, key[i].defaultValue.u32_);
						break;
					case ILP_TYPE_BUTTON:
						//よけいなスペースなどを削除
						ILPRemoveSpaceAndNewline(ptr);
						ILPGetButton((u32 *)key[i].value, ptr, key[i].defaultValue.u32_, key[i].ex);
						break;
					case ILP_TYPE_STRING:
						//コメントアウトもとに戻す
						if( ptr2 != NULL ) *ptr2 = '#';
						ILPGetString((char *)key[i].value, ptr, key[i].defaultValue.str_ );
						break;
					case ILP_TYPE_LIST:
						//よけいなスペースなどを削除
						ILPRemoveSpaceAndNewline(ptr);
						ILPGetNumberFromList((int *)key[i].value, ptr, key[i].defaultValue.int_, (char **)key[i].ex);
						break;
					case ILP_TYPE_BOOL:
						//よけいなスペースなどを削除
						ILPRemoveSpaceAndNewline(ptr);
						ILPGetBool((bool *)key[i].value, ptr, key[i].defaultValue.bool_);
						break;
					case ILP_TYPE_DEC:
						//よけいなスペースなどを削除
						ILPRemoveSpaceAndNewline(ptr);
						ILPGetDec((int *)key[i].value, ptr, key[i].defaultValue.int_);
						break;
				}
			}
		}

	}

	sceIoClose(fd);
	return 0;
}



void ILPRegisterHex(ILP_Key *key, char *keyName, u32 *value, u32 defaultValue)
{
	int keyNum = key[0].buf;

	key[keyNum].value = (void *)value;
	key[keyNum].defaultValue.u32_ = defaultValue;

	key[keyNum].type = ILP_TYPE_HEX;
	
	key[keyNum].ex = NULL;

	//左辺をセット
	key[keyNum].name = keyName;
	
	key[0].buf = keyNum + 1;
}

void ILPRegisterDec(ILP_Key *key, char *keyName, int *value, int defaultValue)
{
	int keyNum = key[0].buf;

	key[keyNum].value = (void *)value;
	key[keyNum].defaultValue.int_ = defaultValue;

	key[keyNum].type = ILP_TYPE_DEC;
	
	key[keyNum].ex = NULL;
	
	//左辺をセット
	key[keyNum].name = keyName;
	
	key[0].buf = keyNum + 1;
}

void ILPRegisterButton(ILP_Key *key, char *keyName,  u32 *value, u32 defaultValue, ILP_Button_Info *buttonInfo)
{
	int keyNum = key[0].buf;

	key[keyNum].value = (void *)value;
	key[keyNum].defaultValue.u32_ = defaultValue;

	key[keyNum].type = ILP_TYPE_BUTTON;
	
	key[keyNum].ex = (void *)buttonInfo;

	
	//左辺をセット
	key[keyNum].name = keyName;

	key[0].buf = keyNum + 1;
}

void ILPRegisterString(ILP_Key *key, char *keyName, char *value, char *defaultValue)
{
	int keyNum = key[0].buf;

	key[keyNum].value = (void *)value;
	key[keyNum].defaultValue.str_ = defaultValue;

	key[keyNum].type = ILP_TYPE_STRING;
	
	key[keyNum].ex = NULL;


	//左辺をセット
	key[keyNum].name = keyName;

	key[0].buf = keyNum + 1;
}

void ILPRegisterList(ILP_Key *key, char *keyName, int *value, int defaultValue, char *list[] )
{
	int keyNum = key[0].buf;

	key[keyNum].value = (void *)value;
	key[keyNum].defaultValue.int_ = defaultValue;

	key[keyNum].type = ILP_TYPE_LIST;
	
	key[keyNum].ex = list;


	//左辺をセット
	key[keyNum].name = keyName;

	key[0].buf = keyNum + 1;
}

void ILPRegisterBool(ILP_Key *key, char *keyName, bool *value, bool defaultValue)
{
	int keyNum = key[0].buf;

	key[keyNum].value = (void *)value;
	key[keyNum].defaultValue.bool_ = defaultValue;

	key[keyNum].type = ILP_TYPE_BOOL;
	
	key[keyNum].ex = NULL;

	
	//左辺をセット
	key[keyNum].name = keyName;

	key[0].buf = keyNum + 1;
}






int ILPWriteLine(SceUID fd, ILP_Key *key, const char *lineFeedCode)
{
	char buf[256];
	
	sprintf(buf, "%s = ", key->name);
	
	ILPGetStringFormatValueByKeynumber(key, buf + strlen(buf), 0);
	
	strcat(buf, lineFeedCode);
	
	return sceIoWrite(fd, buf, strlen(buf));
}


int ILPWriteToFile(ILP_Key *key, const char *path, const char *lineFeedCode)
{
	SceUID fd,fdw;
	int readSize,i;
	char buf[256];
	char write_path[256];
	char *ptr;
	int keyNum = key[0].buf;
	
	//init
	for( i = 0; i < keyNum; i++ ){
		key[i].buf = 0;
	}

	strcpy(write_path, path);
	strcat(write_path, ".tmp");

	fdw = sceIoOpen(write_path, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	if( fdw < 0 ){
		key[0].buf = keyNum;
		return ILP_ERROR_OPEN;
	}
	
	
	fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
	
	
	while( fd >= 0 ){
		readSize = ILPReadLine(fd, buf, 256);
		if( readSize == 0 ) break;//EOF
		
				
		//コメントや改行だけの行など飛ばしてもいい行なら
		if( ILPIsSkipLine(buf) ){
			
			if( sceIoWrite(fdw, buf, readSize) != readSize ){
				return ILP_ERROR_WRITE;
			}
			continue;
		}

		//余計なスペースと改行を削除する
		ILPRemoveSpaceAndNewline(buf);
		
		ptr = ILPChToken(buf, "=");
		if(ptr == NULL) continue; //=が存在しないなら
		for( i = 0; i < keyNum; i++ ){
			if( ( ! key[i].buf ) && strcasecmp( buf, key[i].name ) == 0 ){
				key[i].buf = 1;
				ILPWriteLine(fdw, &key[i], lineFeedCode);
			}
		}
	}


	for( i = 0; i < keyNum; i++ ){
		if( ! key[i].buf ){
			key[i].buf = 1;
			ILPWriteLine(fdw, &key[i], lineFeedCode);			
		}
	}


	if( fd >= 0 ) sceIoClose(fd);
	sceIoClose(fdw);
	
	sceIoRemove(path);
	sceIoRename(write_path, path);
	sceIoRemove(write_path);

	key[0].buf = keyNum;
	return 0;



}
