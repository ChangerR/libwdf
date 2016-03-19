#include "dxplayer.h"
#include <windows.h>

DxPlayer::DxPlayer(HWND hwnd) :D3DLibrary(0), pID3D(0), pID3DDevice(0),VendorID(0),_tex(0){
	
	m_hWnd = hwnd;
	m_texwidth = m_texheight = 0;
	m_clientwidth = m_clientheight = 0;
}

DxPlayer::~DxPlayer() {

	if (_tex)
		_tex->Release();

	if (pID3DDevice)
		pID3DDevice->Release();

	if (pID3D)
		pID3D->Release();
}

struct Vertres {
	Vertres(float _x,float _y,u32 _color,float _u,float _v):x(_x),y(_y),z(0.f),rhw(1.0f),color(_color),u(_u),v(_v){}
	Vertres(){}
	const Vertres& operator = (const Vertres& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		rhw = v.rhw;
		color = v.color;
		u = v.u;
		this->v = v.v;
		return *this;
	}

	float x,y,z,rhw;
	u32 color;
	float u,v;
};

Vertres vertres[4];
const short indices[6] = {0,1,2,0,2,3};

bool DxPlayer::initDriver(int _width,int _height) {
	if (!pID3D)
	{
		D3DLibrary = LoadLibrary( __TEXT("d3d9.dll") );

		if (!D3DLibrary)
		{
			printf("Error, could not load d3d9.dll.\n");
			return false;
		}

		typedef IDirect3D9 * (__stdcall *D3DCREATETYPE)(UINT);
		D3DCREATETYPE d3dCreate = (D3DCREATETYPE) GetProcAddress(D3DLibrary, "Direct3DCreate9");

		if (!d3dCreate)
		{
			printf("Error, could not get proc adress of Direct3DCreate9.\n");
			return false;
		}

		//just like pID3D = Direct3DCreate9(D3D_SDK_VERSION);
		pID3D = (*d3dCreate)(D3D_SDK_VERSION);

		if (!pID3D)
		{
			printf("Error initializing D3D.\n");
			return false;
		}
	}
	// print device information
	D3DADAPTER_IDENTIFIER9 dai;
	if (!FAILED(pID3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &dai)))
	{
		char tmp[512];

		s32 Product = HIWORD(dai.DriverVersion.HighPart);
		s32 Version = LOWORD(dai.DriverVersion.HighPart);
		s32 SubVersion = HIWORD(dai.DriverVersion.LowPart);
		s32 Build = LOWORD(dai.DriverVersion.LowPart);

		sprintf_s(tmp, 512,"%s %s %d.%d.%d.%d\n", dai.Description, dai.Driver, Product, Version,
			SubVersion, Build);
		printf(tmp);

		// Assign vendor name based on vendor id.
		VendorID= static_cast<unsigned short>(dai.VendorId);
		switch(dai.VendorId)
		{
			case 0x1002 : VendorName = "ATI Technologies Inc."; break;
			case 0x10DE : VendorName = "NVIDIA Corporation"; break;
			case 0x102B : VendorName = "Matrox Electronic Systems Ltd."; break;
			case 0x121A : VendorName = "3dfx Interactive Inc"; break;
			case 0x5333 : VendorName = "S3 Graphics Co., Ltd."; break;
			case 0x8086 : VendorName = "Intel Corporation"; break;
			default: VendorName = "Unknown VendorId: ";VendorName += (u32)dai.VendorId; break;
		}
	}
	printf("%s\n",VendorName.c_str());
	
	ZeroMemory(&present, sizeof(present));

	present.BackBufferCount = 1;
	present.EnableAutoDepthStencil = FALSE;
	present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	present.BackBufferFormat	= D3DFMT_UNKNOWN;
	present.SwapEffect		= D3DSWAPEFFECT_DISCARD;
	present.Windowed		= TRUE;
	
	UINT adapter = D3DADAPTER_DEFAULT;
	D3DDEVTYPE devtype = D3DDEVTYPE_HAL;
	
	HRESULT hr = pID3D->CreateDevice(adapter, devtype, m_hWnd,
				D3DCREATE_MULTITHREADED|D3DCREATE_PUREDEVICE|D3DCREATE_HARDWARE_VERTEXPROCESSING, &present, &pID3DDevice);

	if(FAILED(hr))
		hr = pID3D->CreateDevice(adapter, devtype, m_hWnd,
				D3DCREATE_MIXED_VERTEXPROCESSING , &present, &pID3DDevice);

	if(FAILED(hr))
		hr = pID3D->CreateDevice(adapter, devtype, m_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present, &pID3DDevice);

	if (FAILED(hr))
		printf("Was not able to create Direct3D9 device.\n");
		
	if (!pID3DDevice)
	{
		printf("Was not able to create DIRECT3D9 device.\n");
		return false;
	}
	DeviceLost = false;
	m_clientwidth = _width;
	m_clientheight = _height;
	setViewPort();
	
	pID3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pID3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pID3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	
	return true;
}

bool DxPlayer::beginScene(bool backBuffer,u32 color) {
	DWORD flags = 0;
	HRESULT hr;
	
	if (DeviceLost)
	{
		if (FAILED(hr = pID3DDevice->TestCooperativeLevel()))
		{
			if (hr == D3DERR_DEVICELOST)
			{
				Sleep(100);
				hr = pID3DDevice->TestCooperativeLevel();
				if (hr == D3DERR_DEVICELOST)
					return false;
			}
		}
	}
	
	if (backBuffer)
		flags |= D3DCLEAR_TARGET;

	if (flags)
	{
		hr = pID3DDevice->Clear( 0, NULL, flags, color, 1.0, 0);
		if (FAILED(hr))
			printf("DIRECT3D9 clear failed.");
	}

	hr = pID3DDevice->BeginScene();
	if (FAILED(hr))
	{
		printf("DIRECT3D9 begin scene failed.");
		return false;
	}

	return true;
}
		
bool DxPlayer::endScene() {
	HRESULT hr = pID3DDevice->EndScene();
	if (FAILED(hr))
	{
		printf("DIRECT3D9 end scene failed.");
		return false;
	}
	IDirect3DSwapChain9* swChain;
	hr = pID3DDevice->GetSwapChain(0, &swChain);
	hr = swChain->Present(NULL, NULL, m_hWnd, NULL, 0);
	swChain->Release();

	if (SUCCEEDED(hr))
		return true;

	if (hr == D3DERR_DEVICELOST)
	{
		DeviceLost = true;
		printf("Present failed", "DIRECT3D9 device lost.");
	}
	return true;
}

void DxPlayer::showImage(Image* img) {
	
	IDirect3DTexture9* tex;
	
	if(_tex != NULL ) {
		if(img->height != m_texheight || img->width != m_texwidth) {
			_tex->Release();
			tex = get_texture_from_img(img);
			if(tex == NULL)
				return;
			_tex = tex;
			m_texheight = img->height;
			m_texwidth = img->width;
		}else{
			update_texture(img);
			tex = _tex;
		}
	}else {
		tex = get_texture_from_img(img);
		if (tex == NULL)
			return;
		_tex = tex;
		m_texheight = img->height;
		m_texwidth = img->width;
	}	

	pID3DDevice->SetTexture(0,tex);
	pID3DDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
	pID3DDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	pID3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,4,2,&indices[0],D3DFMT_INDEX16,&vertres[0],sizeof(Vertres));

	pID3DDevice->SetTexture(0,NULL);
}

void DxPlayer::setViewPort() {
	D3DVIEWPORT9 viewPort;
	viewPort.X = 0;
	viewPort.Y = 0;
	viewPort.Width = m_clientwidth;
	viewPort.Height = m_clientheight;
	viewPort.MinZ = -1.0f;
	viewPort.MaxZ = 1.0f;

	HRESULT hr = pID3DDevice->SetViewport(&viewPort);
	if (FAILED(hr))
		printf("Failed setting the viewport.");
	vertres[0] = Vertres(0.0f,0.0f,0xffffffff,1.0f,0.0f);
	vertres[1] = Vertres((float)m_clientwidth,0.0f,0xffffffff,0.0f,0.0f);
	vertres[2] = Vertres((float)m_clientwidth,(float)m_clientheight,0xffffffff,0.0f,1.0f);
	vertres[3] = Vertres(0.0f,(float)m_clientheight,0xffffffff,1.0f,1.0f);
}

void DxPlayer::Text(int x,int y, const stringc& str) {
	
}

void DxPlayer::TextR(int x,int y, const stringc& str) {
	
}

IDirect3DTexture9* DxPlayer::get_texture_from_img(Image* img) {
	IDirect3DTexture9* tex;
	D3DLOCKED_RECT rect;
	if(img->width == 0||img->height == 0)
		return NULL;
	
	if (FAILED(pID3DDevice->CreateTexture(img->width,img->height,0,D3DUSAGE_AUTOGENMIPMAP,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&tex,NULL)))
	{
		return NULL;
	}

	if (img->buf&&SUCCEEDED(tex->LockRect(0,&rect,NULL,NULL)))
	{
		u32* buf = (u32*)rect.pBits;
		u8* src_buf = img->buf;

		for(u32 i = 0; i < img->height;i++) {
			if(img->depth == 3) {
				u32 color;
				for (u32 j = 0; j < img->width;j++)
				{
					color = 0xff000000;
					color += (*src_buf++)<<16;
					color += (*src_buf++)<<8;
					color += *src_buf++;
					*buf++ = color;
				}				 
			}else if (img->depth == 4)
			{
				memcpy(buf,src_buf,img->width*img->depth);
				src_buf += img->width*img->depth;
			}
			buf = (u32*)((u8*)rect.pBits + rect.Pitch*(i+1));
		}
		tex->UnlockRect(0);
	}
	return tex;
}

int DxPlayer::update_texture(Image* img) {
	D3DLOCKED_RECT rect;
	
	if (img->buf&&SUCCEEDED(_tex->LockRect(0,&rect,NULL,NULL)))
	{
		u32* buf = (u32*)rect.pBits;
		u8* src_buf = img->buf;

		for(u32 i = 0; i < img->height;i++) {
			if(img->depth == 3) {
				u32 color;
				for (u32 j = 0; j < img->width;j++)
				{
					color = 0xff000000;
					color += (*src_buf++)<<16;
					color += (*src_buf++)<<8;
					color += *src_buf++;
					*buf++ = color;
				}				 
			}else if (img->depth == 4)
			{
				memcpy(buf,src_buf,img->width*img->depth);
				src_buf += img->width*img->depth;
				
			}
			buf = (u32*)((u8*)rect.pBits + rect.Pitch*(i+1));
		}
		_tex->UnlockRect(0);
	}
	return 1;
}

void DxPlayer::showDefault()
{
	if (_tex == NULL)
		return;
	pID3DDevice->SetTexture(0, _tex);
	pID3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	pID3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pID3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0], D3DFMT_INDEX16, &vertres[0], sizeof(Vertres));

	pID3DDevice->SetTexture(0, NULL);
}
