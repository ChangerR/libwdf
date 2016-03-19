#ifndef __CPPUTILS_WASFILE
#define __CPPUTILS_WASFILE

class WasSpirit {
public:
	static WasSpirit* createSpirit(unsigned char* data,int len);

private:
	WasSpirit();

	bool init(unsigned char* data,int len);

#pragma pack(push,packing)
#pragma pack(1)
	struct WasHead {
		unsigned short tag;
		unsigned short headersize;
		unsigned short spritecount;
		unsigned short framecount;
		unsigned short width;
		unsigned short height;
		unsigned short centerX;
		unsigned short cneterY;
	};

#pragma pack(pop,packing)
};

#endif
