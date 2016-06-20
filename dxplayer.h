#ifndef __DX_PLAYER__
#define __DX_PLAYER__
#include <d3d9.h>
#include <stdio.h>
#include "slconfig.h"
#include "Image.h"
#include "stringc.h"

class DxPlayer {
public:
	DxPlayer(HWND hwnd);
	virtual ~DxPlayer();
	
	bool initDriver(int _width,int _height);
	
	bool beginScene(bool backBuffer=true,u32 color=0xff000000);
			
	bool endScene();
	
	void showImage(Image* img);

	void showDefault();

	void setViewPort();

	void Text(int x,int y, const stringc& str);

	void TextR(int x,int y, const stringc& str);

	IDirect3DTexture9* get_texture_from_img(Image* img);
	
	int update_texture(Image*);

protected:
	D3DPRESENT_PARAMETERS present;
	HINSTANCE D3DLibrary;
	IDirect3D9* pID3D;
	IDirect3DDevice9* pID3DDevice;
	unsigned short VendorID;
	stringc VendorName;
	bool DeviceLost;
	IDirect3DTexture9* _tex;
	int m_texwidth,m_texheight;
	int m_clientwidth,m_clientheight;
	HWND m_hWnd;
};
#endif