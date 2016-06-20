/*********************************************************************************************
*                                         ���׹�˾����ϵ��
*      ���λ����Ρ���������II����������3�ڲ�桷��������3��ʽ�桷���������⴫��
*                                        ��ͼ�ļ���ʽ��ȡ��
*---------------------------------------------------------------------------------------------
*
*                                           �汾 5.0.0
*
*                                     ������ <wdl@sina.com>
*                                            <QQ:DaliStudio@QQ.com>
*
*                                     �ٷ���ַ��http://DaliStudio.net
*
*---------------------------------------------------------------------------------------------
* ��Ȩ��ɣ�
*     �����Ȩ�����ʹ���߷����������������������£�����ʹ����ʹ�ü��ٷ��������������Դ����
* �������ƿ�ִ�и�ʽ��Ȩ�������۴�����Ƿ��޸Ľ�Ȼ��
* 
*     1�����ڱ����Դ������ٷ��������뱣����Ȩ�������������������������ݣ��Լ�����������������
*     2�����ڱ����������Ŀ���ִ�и�ʽ���ٷ��������븽�����ı��ļ���ʽ��/�򷢲���Ʒ��װ�е���
*        ��ý����ʽ�����������İ�Ȩ�������������������������ݣ��Լ�����������������
*     3��δ����������ǰ������ʹ�ô������һ����������֮���壬��Ϊ�������������Ʒ���κ�
*        ��ʾ֧�֡��Ͽɻ��ƹ㡢��������Ϊ��
*
* ����������
*     ��Դ�����Ӧ�������׹�˾��������Ϸ�����λ����Ρ�����������II������������3�ڲ�桷��
*��������3��ʽ�桷�͡��������⴫������Ϸ�ĵ�ͼ�ļ���ʽ�Ķ�ȡ�����Դ������Ӧ���ļ���ʽ��Ȩ
* �����������׹�˾��/���Ʒ�ķ����Ϸ�������С�
*     ��Դ�����Ӧ����ѧϰ�ͽ���Ŀ�ģ���ֹ����һ����ҵĿ�ģ��Լ���ֹ����һ���п�������������
* �ĵط�������ʹ���ߺ���Ը�������ʹ�ñ�Դ���룬���߲��е�һ���κε����Σ���ʹ��������ʹ�ñ�Դ
* ���롣��ʹ�ñ������߼���ʾͬ��������������κ���������ʹ�ñ�Դ���룬������ɾ����Դ���롣
*     ��Դ�����ԭ���ߺ͹����߲����κ���ʾ��ʾ���κ����Ρ�������ʹ�ñ�Դ�����/��Ŀ������
* �������κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣������������������Ʒ��ʹ��
* ��ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ��������κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ�
* ���𺦵���������Ȼ��
*---------------------------------------------------------------------------------------------
*                         ��Ȩ����(c) 2003-2010 ��������������Ȩ����
*********************************************************************************************/
#pragma once
#include <string>
using namespace std; // ʹ��C++��׼�ļ�ͷ
#include "BmpOffset.h"

#ifdef  __cplusplus
extern "C" {
#endif
#include "../jpeglib/jpeg.h"
#ifdef  __cplusplus
}
#endif

#ifdef _DEBUG
#pragma comment (lib,"../jpeglib/debug/jpeglib.lib")
#else
#pragma comment (lib,"../jpeglib/release/jpeglib.lib")
#endif

// ����TGAͼƬ��ʽ�ļ�ͷ
#pragma pack(push) //�������״̬
#pragma pack(1)//�趨Ϊ4�ֽڶ���
struct TGA_FILE_HEADER 
{
	uint8 IdLength;				// ͼ����Ϣ�ֶ�(Ĭ��:0)
	uint8 ColorMapType;			// ��ɫ�������(Ĭ��0)
	uint8 ImageType;			// ͼ��������(֧��2��10)
	uint16 ColorMapFirstIndex;	// ��ɫ�������(Ĭ��:0)
	uint16 ColorMapLength;		// ��ɫ��ĳ���(Ĭ��:0)
	uint8 ColorMapEntrySize;	// ��ɫ������Ϊ��(Ĭ��:0��֧��16/24/32)
	uint16 XOrigin;				// ͼ��X�������ʼλ��(Ĭ��:0)
	uint16 YOrigin;				// ͼ��Y�������ʼλ��(Ĭ��:0)
	uint16 ImageWidth;			// ͼ��Ŀ��
	uint16 ImageHeight;			// ͼ��ĸ߶�
	uint8 PixelDepth;			// ͼ��ÿ���ش洢ռ��λ��
	uint8 ImageDescruptor;		// ͼ�������ַ��ֽ�(Ĭ��:0����֧��16bit 565��ʽ)
};
#pragma pack(pop)//�ָ�����״̬

class CXY
{
public:
	CXY(void);
	~CXY(void);
	BOOL BrowseFile(CString strFile, CString strFilter, CStringList *pFileList, CStringList *pFileNameList, CStringList *pShowNameList);//�����ļ����� 
	bool FileExist(CString strFileName); // �ж��ļ��Ƿ����
	bool FolderExists(CString s); // �ж�Ŀ¼�Ƿ����
	bool SuperMkDir(CString P); // �������Ŀ¼
	int GetGameType(CString path); // �����Ϸ������
	CStringList* GetFileList(CString path); // ����ļ����б�
	CStringList* m_MapFileList;
	CStringList* GetFileName(); // ����ļ���
	void OpenMap(CString path, CString filename, CString DstPath); // ��MAP�ļ�
	string WideCharToMultiChar(wstring str);
	wstring MultCHarToWideChar(string str);

	uint8* TempBuffer;
	uint8* MapHandler(uint8* Buffer, uint32 inSize, uint32* outSize); // ��ͼ��JPEG���ݴ�����
	void ByteSwap(uint16 &value);
	uint16 RGB565to555(uint16 &Value);
	void WritePixel(uint16* Buffer, uint16* SubBmp, uint32 MapWidth, uint32 MapHeight, uint32 SubMapWidth, uint32 SubMapHeight, uint32 Offset);

	int m_GameType; // ��Ϸ����
	CString m_GameName; // ��Ϸ����
	CString m_MapFileName; // WDF���ļ���
	CString m_OutFileName; // ������ļ���
	CString m_FileName; // �����ļ�����

	BOOL isBig; // �Ƿ�������ͼ
	BOOL isSmall; // �Ƿ����СͼƬ
	BOOL isMask; // �Ƿ��������ͼ
	BOOL isCell; // �Ƿ������·ͼ
	BOOL isFileExist; // �ļ��Ƿ����

	void SetIsBig(BOOL b);
	void SetIsSmall(BOOL b);
	void SetIsMask(BOOL b);
	void SetIsCell(BOOL b);
	void SetIsFileExist(BOOL b);
};
