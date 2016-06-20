#ifndef __CPPUTILS_WASFILE
#define __CPPUTILS_WASFILE
#include <string.h>

class WasSpirit {
public:
	class Frame {
	public:
		Frame():lineOffsets(NULL),data(NULL){}

		~Frame() {
			if(lineOffsets) {
				delete[] lineOffsets;
				lineOffsets = NULL;
			}

			if(data) {
				delete[] data;
				data = NULL;
			}
		}
		// 帧数据偏移
		int addrOffset;
	
		// 行数据偏移
		int* lineOffsets;
	
		int delay;// 延时帧数
		int height;
		int width;
		int offX;// 图像偏移
		int offY;

		unsigned int *data;
	};


public:
	static WasSpirit* createSpirit(unsigned char* data,int len) {
		WasSpirit* spirit = new WasSpirit;

		if(spirit != NULL && spirit->init(data,len)) {
			return spirit;
		}

		return NULL;
	}

	~WasSpirit() {
		if(_delayLine) {
			delete[] _delayLine;
			_delayLine = NULL;
		}
		if(_frames) {
			for(int i = 0;i < _wasHead.spritecount;i++) {
				delete[] _frames[i];
			}
			delete[] _frames;
			_frames = NULL;
		}
	}

	void debug();
private:
	WasSpirit() {
		_delayLine = NULL;
		memset(&_wasHead,0,sizeof(WasHead));
		_frames = NULL;
	}

	bool init(unsigned char* data,int len);

	void parse(Frame* frame,unsigned char* data,unsigned int* palette);
	
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
		unsigned short centerY;
	};
#pragma pack(pop,packing)
	unsigned char* _delayLine;
	int _delayLine_len;
	WasHead _wasHead;
	Frame** _frames;
};

#endif
