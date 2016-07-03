#include "was_spirit.h"
#include <stdio.h>


namespace dream {
	
const int TYPE_ALPHA = 0x00;// 前2位

const int TYPE_ALPHA_PIXEL = 0x20;// 前3位 0010 0000

const int TYPE_ALPHA_REPEAT = 0x00;// 前3位

const int TYPE_FLAG = 0xC0;// 2进制前2位 1100 0000

const int TYPE_PIXELS = 0x40;// 以下前2位 0100 0000

const int TYPE_REPEAT = 0x80;// 1000 0000

const int TYPE_SKIP = 0xC0; // 1100 0000

bool WasSpirit::init(unsigned char* data,size_t len)
{
	Data d(data,len);
	
	bool ret = false;
	unsigned int *palette = NULL;

	do {
		if(data == NULL) {
			printf("null data detect\n");
			break;
		}
		
		d.read((unsigned char*)&_wasHead,sizeof(WasHead));

		if(_wasHead.tag != WAS_FILE_TAG) {
			printf("this is not a was file\n");
			break;
		}
		
		if(4 + _wasHead.headersize > sizeof(WasHead)) {
			_delayLine_len = _wasHead.headersize + 4 - sizeof(WasHead);
			_delayLine = new unsigned char[_delayLine_len];
			d.read(_delayLine,_delayLine_len);
		}

		d.seek(_wasHead.headersize + 4);

		palette = new unsigned int[256];
		for(int i = 0; i < 256;i++) {
			unsigned short wColor = d.read_ushort();
			palette[i] = ((wColor & 0x001F) << 3) + ((wColor & 0x07E0) << 5) + ((wColor & 0xF800) << 8);
		}

        _frames = new Frame*[_wasHead.spiritcount];
		
		d.seek(_wasHead.headersize + 4 + 512);
		
        for(int i = 0;i < _wasHead.spiritcount;i++) {
			_frames[i] = new Frame[_wasHead.framecount];
			for(int n = 0; n < _wasHead.framecount;n++) {
				if(_delayLine != NULL && n < _delayLine_len) {
					_frames[i][n].delay = _delayLine[n];
				}
				_frames[i][n].addrOffset = d.read_int();
			}
		}

        for(int i = 0;i < _wasHead.spiritcount;i++) {
			for(int n = 0;n < _wasHead.framecount;n++) {
				Frame* frame = &_frames[i][n];
				int offset = frame->addrOffset;
				
                if(offset <= 0) {
                    frame->offX = 0;
                    frame->offY = 0;
                    frame->width = 0;
                    frame->height = 0;
                    frame->lineOffsets = NULL;
                    frame->data = NULL;
                    continue;
                }

				d.seek( _wasHead.headersize + 4 + offset);

				frame->offX = d.read_int();
				frame->offY = d.read_int();
				frame->width = d.read_int();
				frame->height = d.read_int();

                if(frame->height > 1000) {
                    return false;
                }
				frame->lineOffsets = new int[frame->height];
				
				d.read((unsigned char*)frame->lineOffsets,frame->height * 4);

				parse(frame,d,palette);
			}
		}

		ret = true;
		
	}while(0);

	if(palette) {
		delete[] palette;
		palette = NULL;
	}

	return ret;
}

void WasSpirit::parse(Frame* frame,Data& d,unsigned int* palette) {

	int frameWidth = frame->width;
	int frameHeight = frame->height;
	unsigned int* dst = NULL;
	unsigned char flag;
	unsigned char count;
	unsigned int color;
	unsigned int alpha;

	frame->data = new unsigned int[frameWidth * frameHeight];
	memset(frame->data,0,frameWidth * frameHeight * 4);
	
	for(int y = 0; y < frameHeight; y++) {
		
		d.seek(_wasHead.headersize + 4 + frame->lineOffsets[y] + frame->addrOffset);
		dst = frame->data + y * frameWidth;
		
		for(int x = 0; x < frameWidth;) {
			flag = d.read_byte();

			switch(flag & TYPE_FLAG) {
				case TYPE_ALPHA:
				{
					if ((flag & TYPE_ALPHA_PIXEL) > 0) 
					{
						color = palette[d.read_byte()];
						alpha = flag & 0x1F;
						dst[x] = color + (alpha << 27);
						x++;
					} 
					else if (flag != 0) 
					{
						// ???
						count = flag & 0x1F;// count
						alpha = d.read_byte();// alpha
						alpha = alpha & 0x1F;
						color = palette[d.read_byte()];
						for (int i = 0; i < count; i++) 
						{
							dst[x] = color + (alpha << 27);
							x++;
						}
					} else {
						x = frameWidth;
					}
				}

				break;

				case TYPE_PIXELS:
					{
						count = flag & 0x3F;
						for (int i = 0; i < count; i++) 
						{
							color = palette[d.read_byte()];
							alpha = 0x1F;
							dst[x] = color + (alpha << 27);
							x++;
						}
					}
					break;

				case TYPE_REPEAT:
					{
						count = flag & 0x3F;
						color = palette[d.read_byte()];
						alpha = 0x1F;
						for (int i = 0; i < count; i++) 
						{
							dst[x] = color + (alpha << 27);
							x++;
						}
					}
					break;

				case TYPE_SKIP:
					{
						count = flag & 0x3F;
						x += count;
					}
					break;
			}
		}
	}	
}

void WasSpirit::debug() {
	printf("Debug spirit info:\n");
	printf("spirit width:%d\n",_wasHead.width);
	printf("spirit height:%d\n",_wasHead.height);
    printf("spirit count:%d\n",_wasHead.spiritcount);
	printf("spirit frame count:%d\n",_wasHead.framecount);
	printf("spirit centerx:%d\n",_wasHead.centerX);
	printf("spirit centery:%d\n",_wasHead.centerY);
}

};
