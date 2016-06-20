#include "WdfPack.h"
#include <string.h>

int main(int argc,char** argv) {
	
	WdfPack* pack = WdfPack::createWdfPack("shape.wdf");
	
	if(pack) {
		pack->sort();
		pack->beginEnumFile();
		unsigned char* buffer = NULL;
		int size;
		unsigned int hash;
		while((buffer = pack->enumFile(hash,size))!= NULL) {
			char buf[32];
			sprintf(buf,"shape/%X",hash);
			FILE* f = fopen(buf,"wb"); 
			if(f == NULL)
			{
				printf("open file failed\n");
				break;
			}
			fwrite(buffer,size,1,f);
			fflush(f);
			fclose(f);
		}
		delete pack;
	}
		
	return 0;
}
