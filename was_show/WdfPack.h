#ifndef __CPPUTILS_WDFPACK_
#define __CPPUTILS_WDFPACK_
#include <stdio.h>

#define WDFMAGIC ((unsigned int)(('W' << 24)|('D' << 16)|('F' << 8)|('P')))

class WdfPack{
public:

	static WdfPack* createWdfPack(const char* file) {
		WdfPack* pack = new WdfPack;
		
		if(pack->init(file) == true){
			return pack;
		}else{
			delete pack;
			return NULL;
		}
	}
	
	virtual ~WdfPack() {
		if(_file)
			fclose(_file);
		if(_list)
			delete[] _list;
		if(_buffer)
			delete[] _buffer;
	}
	
	void sort() {
		qsort(0,_wdf.count - 1);
	}
	
	void debugWdf() {
		printf("File Count:%d offset=%d\n",_wdf.count,_wdf.offset);
		for(int i = 0; i < _wdf.count;i++) {
			printf("HASH=%X offset=%d size=%d space=%d\n",_list[i].hash,
						_list[i].offset,_list[i].size,_list[i].space);
		}
	}

	void beginEnumFile() {
		_pointer = 0;
	}

	unsigned char* enumFile(unsigned int& hash,int& size) {
		if(_pointer >= _wdf.count || _file == NULL)
			return NULL;
		if(_alloc_size < _list[_pointer].size) {
			_alloc_size = (_list[_pointer].size + 7) & ~0x7;
			if(_buffer != NULL)
				delete[] _buffer;
			_buffer = new unsigned char[_alloc_size];
		}
		fseek(_file,_list[_pointer].offset,SEEK_SET);
		fread(_buffer,_list[_pointer].size,1,_file);
		size = _list[_pointer].size;
		hash = _list[_pointer].hash;
		_pointer++;

		return _buffer;
	}
private:
	WdfPack():_list(NULL),_file(NULL),_alloc_size(0),_buffer(NULL),_pointer(0){};
	
	bool init(const char* file) {
		bool ret = false;

		do {
			if(file == NULL)break;
			
			_file = fopen(file,"rb");
			if(_file == NULL)break;
			
			fread(&_wdf,sizeof(wdf_t),1,_file);
			
			if(_wdf.magic != WDFMAGIC) {
				printf("This file is not a wdf package magic=%x rel=%X\n",
						_wdf.magic,WDFMAGIC);
				break;
			}
			
			_list = new wdflist_t[_wdf.count];
			
			fseek(_file,_wdf.offset,SEEK_SET);
			
			fread(_list,sizeof(wdflist_t),_wdf.count,_file);
			
			ret = true;
		}while(0);
		
		return ret;
	}
	
	void qsort(int l,int n) {
		if(l >= n)
			return;
		int m = l;
		wdflist_t t;
		for(int i = l + 1; i <= n;i++) {
			if(_list[i] < _list[l]) {
				t = _list[++m];
				_list[m] = _list[i];
				_list[i] = t;
			}
		}
		t = _list[l];
		_list[l] = _list[m];
		_list[m] = t;
		qsort(l,m-1);
		qsort(m+1,n);
	}


#pragma pack(push,packing)
#pragma pack(1)
	struct wdf_t {
		unsigned int magic;
		int count;
		int offset;
	};
	
	class wdflist_t {
	public:
		unsigned int hash;
		int offset;
		int size;
		int space;
		
		wdflist_t& operator = (const wdflist_t& l) {
			this->hash = l.hash;
			this->offset = l.offset;
			this->size = l.size;
			this->space = l.space;

			return *this;
		}
		
		bool operator < (const wdflist_t& l) const {
			return this->offset < l.offset;
		}
	};
#pragma pack(pop,packing)

	wdf_t _wdf;
	wdflist_t* _list;
	FILE* _file;
	unsigned char* _buffer;
	int _alloc_size;
	int _pointer;
};
#endif
