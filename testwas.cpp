#include "was_spirit.h"
#include <stdio.h>

int main(int argc,char** argv) {
	FILE* file = NULL;
	size_t file_len = -1;
	WasSpirit* spirit;
	unsigned char* buff = NULL;
	if(argc < 2) {
		printf("Please input was files\n");
		return 1;
	}
	file = fopen(argv[1],"rb");
	
	if(file == NULL) {
		printf("open file failed,please check\n");
		return 1;
	}
	fseek(file,0,SEEK_END);
	file_len = ftell(file);
	fseek(file,0,SEEK_SET);

	if(file_len <= 0) {
		printf("this is a empty file,^_^\n");
		fclose(file);
		return 1;
	}
	buff = new unsigned char[file_len + 1];
	memset(buff,0,file_len + 1);

	fread(buff,file_len,1,file);

	fclose(file);

	spirit = WasSpirit::createSpirit(buff,file_len);
	
	if(spirit) {
		spirit->debug();
		delete spirit;
	}		
	
	delete [] buff;
	return 0;
}

