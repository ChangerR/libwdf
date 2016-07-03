/*********************************************************************************************
*                                         网易公司西游系列
*      《梦幻西游》《大话西游II》《大话西游3内测版》《大话西游3正式版》《大话西游外传》
*                                        地图文件格式读取类
*---------------------------------------------------------------------------------------------
*
*                                           版本 5.0.0
*
*                                     王大理 <wdl@sina.com>
*                                            <QQ:DaliStudio@QQ.com>
*
*                                     官方网址：http://DaliStudio.net
*
*---------------------------------------------------------------------------------------------
* 授权许可：
*     这份授权条款，在使用者符合以下三个条件的情形下，授予使用者使用及再发布本软件，包括源代码
* 及二进制可执行格式的权利，无论此软件是否经修改皆然：
* 
*     1、对于本软件源代码的再发布，必须保留版权声明、包括此三个条件的内容，以及下述的免责声明。
*     2、对于本软件二进制目标可执行格式的再发布，必须附带以文本文件形式和/或发布产品包装中的其
*        他媒介形式，再现上述的版权声明、包括此三个条件的内容，以及下述的免责声明。
*     3、未获得书面许可前，不得使用大理工作室或本软件贡献者之名义，来为本软件的衍生产品做任何
*        表示支持、认可或推广、促销等行为。
*
* 免责声明：
*     本源代码可应用于网易公司的网络游戏：《梦幻西游》、《大话西游II》、《大话西游3内测版》、
*《大话西游3正式版》和《大话西游外传》等游戏的地图文件格式的读取。与此源代码相应的文件格式版权
* 可能属于网易公司和/或云风的风魂游戏引擎所有。
*     此源代码仅应用于学习和教育目的，禁止用于一切商业目的，以及禁止用于一切有可能损害他人利益
* 的地方，否则使用者后果自负。凡是使用本源代码，作者不承担一切任何的责任，请使用者慎重使用本源
* 代码。凡使用本代码者即表示同意此声明，如有任何异议请勿使用本源代码，并永久删除此源代码。
*     此源代码的原作者和贡献者不负任何明示或暗示的任何责任。对于因使用本源代码和/或目标程序而
* 产生的任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限于替代商品或使用
* 损失、资料损失、利益损失、业务中断等等），不负任何责任，即在该种使用已获事前告知可能会造成此
* 类损害的情形下亦然。
*---------------------------------------------------------------------------------------------
*                         版权所有(c) 2003-2010 王大理，保留所有权利。
*********************************************************************************************/
#pragma once
#include <string>
using namespace std; // 使用C++标准文件头
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

// 定义TGA图片格式文件头
#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为4字节对齐
struct TGA_FILE_HEADER 
{
	uint8 IdLength;				// 图像信息字段(默认:0)
	uint8 ColorMapType;			// 颜色标的类型(默认0)
	uint8 ImageType;			// 图像类型码(支持2或10)
	uint16 ColorMapFirstIndex;	// 颜色表的引索(默认:0)
	uint16 ColorMapLength;		// 颜色表的长度(默认:0)
	uint8 ColorMapEntrySize;	// 颜色表表项的为数(默认:0，支持16/24/32)
	uint16 XOrigin;				// 图像X坐标的起始位置(默认:0)
	uint16 YOrigin;				// 图像Y坐标的起始位置(默认:0)
	uint16 ImageWidth;			// 图像的宽度
	uint16 ImageHeight;			// 图像的高度
	uint8 PixelDepth;			// 图像每像素存储占用位数
	uint8 ImageDescruptor;		// 图像描述字符字节(默认:0，不支持16bit 565格式)
};
#pragma pack(pop)//恢复对齐状态

class CXY
{
public:
	CXY(void);
	~CXY(void);
	BOOL BrowseFile(CString strFile, CString strFilter, CStringList *pFileList, CStringList *pFileNameList, CStringList *pShowNameList);//查找文件函数 
	bool FileExist(CString strFileName); // 判断文件是否存在
	bool FolderExists(CString s); // 判断目录是否存在
	bool SuperMkDir(CString P); // 创建多层目录
	int GetGameType(CString path); // 获得游戏的类型
	CStringList* GetFileList(CString path); // 获得文件的列表
	CStringList* m_MapFileList;
	CStringList* GetFileName(); // 获得文件名
	void OpenMap(CString path, CString filename, CString DstPath); // 打开MAP文件
	string WideCharToMultiChar(wstring str);
	wstring MultCHarToWideChar(string str);

	uint8* TempBuffer;
	uint8* MapHandler(uint8* Buffer, uint32 inSize, uint32* outSize); // 地图的JPEG数据处理器
	void ByteSwap(uint16 &value);
	uint16 RGB565to555(uint16 &Value);
	void WritePixel(uint16* Buffer, uint16* SubBmp, uint32 MapWidth, uint32 MapHeight, uint32 SubMapWidth, uint32 SubMapHeight, uint32 Offset);

	int m_GameType; // 游戏类型
	CString m_GameName; // 游戏名称
	CString m_MapFileName; // WDF的文件名
	CString m_OutFileName; // 输出的文件名
	CString m_FileName; // 仅含文件名字

	BOOL isBig; // 是否输出大地图
	BOOL isSmall; // 是否输出小图片
	BOOL isMask; // 是否输出遮掩图
	BOOL isCell; // 是否输出行路图
	BOOL isFileExist; // 文件是否存在

	void SetIsBig(BOOL b);
	void SetIsSmall(BOOL b);
	void SetIsMask(BOOL b);
	void SetIsCell(BOOL b);
	void SetIsFileExist(BOOL b);
};
