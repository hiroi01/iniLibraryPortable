
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspctrl.h>

#include <string.h>

#include <libinip.h>

// - - - - - - - - - - - - - - - - - - - - //

PSP_MODULE_INFO("sample", 0, 0, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

// - - - - - - - - - - - - - - - - - - - - //

#define CONFIG_NUM 6

typedef struct myconfig_ {
	u32 hexValue;
	int decValue;
	u32 button;
	char string[256];
	int list;
	bool boolFlag;
} myconfig;



#define INI_FILE_PATH "sample.ini"
#define INI_FILE_PATH2 "sample_2.ini"

// - - - - - - - - - - - - - - - - - - - - //
myconfig g_conf;

ILP_Key g_key[CONFIG_NUM];

char *mylist[] = {
	"First",
	"Second",
	"Third",
	NULL
};
// - - - - - - - - - - - - - - - - - - - - //


void initConfig(ILP_Key *key, myconfig *conf)
{
	ILPInitKey(key);
	ILPRegisterHex(key, "HexValue", &conf->hexValue, 0xf);
	ILPRegisterDec(key, "DecValue", &conf->decValue, 9);
	ILPRegisterButton(key, "Button", &conf->button, PSP_CTRL_CIRCLE, NULL);
	ILPRegisterString(key, "String", conf->string, "Default String");
	ILPRegisterList(key, "List", &conf->list, 0, mylist);
	ILPRegisterBool(key, "Bool", &conf->boolFlag, false);	
}

void printConfig(ILP_Key *key, int keynum)
{
	int i;
	char buf[256];
	char *name;
	
	for( i = 0; i < keynum; i++ ){
		name = ILPGetNameAddressByKeynumber(key, i);
		ILPGetStringFormatValueByKeynumber(key, buf, i);
		pspDebugScreenPrintf("%s = %s\n", name, buf);
	}
}



int main(void)
{
	SceCtrlData pad;
	int ret;

	
	pspDebugScreenInit();
	
	//init
	initConfig(g_key, &g_conf);
	
	
	
	//read
	ret = ILPReadFromFile(g_key, INI_FILE_PATH);
	if( ret == 0 ){
		pspDebugScreenPrintf("read : OK\n");
	}
	
	//print
	pspDebugScreenPrintf("----------\n");
	pspDebugScreenPrintf(INI_FILE_PATH);
	pspDebugScreenPrintf("\n\n");
	printConfig(g_key, CONFIG_NUM);

	
	
	//change config
	g_conf.hexValue = 0xab;
	g_conf.decValue = 50;
	g_conf.button = PSP_CTRL_CIRCLE + PSP_CTRL_CROSS;
	strcpy(g_conf.string, "new string");
	g_conf.list = 2;
	g_conf.boolFlag = false;
	
	
	
	//write
	ret = ILPWriteToFile(g_key, INI_FILE_PATH2, "\r\n");
	if( ret == 0 ){
		pspDebugScreenPrintf("write : OK\n");
	}
	
	//print
	pspDebugScreenPrintf("----------\n");
	pspDebugScreenPrintf(INI_FILE_PATH2);
	pspDebugScreenPrintf("\n\n");
	printConfig(g_key, CONFIG_NUM);
	
	
	
	pspDebugScreenPrintf("\nPress cross to exit\n");
	while(1){
		sceCtrlReadBufferPositive(&pad,1);
		if( pad.Buttons & PSP_CTRL_CROSS ){
			break;
		}
		sceKernelDelayThread(100);
	}
	
	pspDebugScreenPrintf("Exiting...\n");
	sceKernelExitGame();
	return 0;
}
