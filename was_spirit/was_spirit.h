#ifndef __CPPUTILS_WASFILE
#define __CPPUTILS_WASFILE
#include <string.h>
#include <assert.h>

namespace dream {
	
class Data {
public:

	Data(unsigned char* buf,size_t size):_buffer(buf),_pos(0),_buffer_end(buf + size){}
	
	virtual ~Data() {
		if(_buffer) {
			delete[] _buffer;
			_buffer = NULL;
		}
	}
	
	unsigned char read_byte() {
		unsigned char ret = *(unsigned char*)(_buffer + _pos);
		_pos = _pos + 1;
		
		assert(_buffer + _pos <= _buffer_end);
		
		return ret;
	}
	
	unsigned short read_ushort() {
		unsigned short ret = *(unsigned short*)(_buffer + _pos);
		_pos = _pos + 2;
		
		assert(_buffer + _pos <= _buffer_end);
		
		return ret;
	}
	
	int read_int() {
		int ret = *(int*)(_buffer + _pos);
		_pos = _pos + 4;
		
		assert(_buffer + _pos <= _buffer_end);
		
		return ret;
	}
	
	unsigned int read_uint() {
		unsigned int ret = *(unsigned int*)(_buffer + _pos);
		_pos = _pos + 4;
		
		assert(_buffer + _pos <= _buffer_end);
		
		return ret;
	}
	
	void read(unsigned char* buf,size_t size) {
		
		assert(_buffer + _pos + size <= _buffer_end);
		
		memcpy(buf,_buffer + _pos,size);
		
		_pos = _pos + size;
	}
	
	void seek(size_t pos) {
		_pos = pos;
		
		assert(_buffer + _pos <= _buffer_end);
	}
	
private:
	unsigned char* _buffer;
	size_t _pos;
	unsigned char* _buffer_end;
};

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

	void parse(Frame* frame,Data& d,unsigned int* palette);
	
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

};


#endif
