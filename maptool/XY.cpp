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
#include "StdAfx.h"
#include "XY.h"
#include "ReadGameMap.h"

using namespace NetEase;

CXY::CXY(void)
{
	isBig = TRUE;
	isSmall = FALSE;
	isMask = FALSE;
	isCell = FALSE;
	isFileExist = TRUE;
}

CXY::~CXY(void)
{
}

BOOL CXY::BrowseFile(CString strFile, CString strFilter, CStringList *pFileList, CStringList *pFileNameList, CStringList *pShowNameList)
{
	CFileFind   FileFind;   
	CString   SearchTarget   =   strFile;//   +   "\\"   +strFilter;   
	if(SearchTarget.Right(1)   !=   "\\")   
		SearchTarget   +=   "\\";   
	SearchTarget   +=   "*.*";
	BOOL res = FileFind.FindFile(SearchTarget);
	while(res)   
	{   
		res = FileFind.FindNextFile(); // 下一个文件
		if(FileFind.IsDirectory() && !FileFind.IsDots()) // 如果是目录和不是点
		{   
			//如果是一个子目录，用递归继续往深一层找   
			CString strPath = FileFind.GetFilePath();   
			CString strTitle = FileFind.GetFileTitle();   
			BrowseFile(strPath, strFilter, pFileList, pFileNameList, pShowNameList); // 递归
		}
		else   if(!FileFind.IsDirectory() && !FileFind.IsDots()) // 如果不是目录和不是点
		{   
			//显示当前访问的文件   
			CString strExtName; // 后缀名
			CString strName;    // 文件名
			strName = FileFind.GetFileName(); // 获得文件名 
			strExtName = strName.Right(strName.GetLength() - strName.Find('.')-1); // 获得后缀名 
			strExtName = strExtName.Left(strFilter.GetLength()); // 为了匹配过滤的后缀名
			strExtName.MakeUpper(); // 转换成大写  
			if(!strExtName.CompareNoCase(strFilter))//扩展名正确   
			{
				CString strPath = FileFind.GetFilePath();   
				pFileList->AddTail(strPath); 
				pFileNameList->AddTail(strName);   
				strName = strName.Left(strName.Find('.'));   
				pShowNameList->AddTail(strName);   
			}   
		}   
	}   
	FileFind.Close();//关闭   
	return   TRUE; 
}

// 判断文件是否存在
bool CXY::FileExist(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName); 
}

// 判断目录是否存在
bool CXY::FolderExists(CString s)
{
	DWORD attr; 
	attr = GetFileAttributes(s); 
	return (attr != (DWORD)(-1) ) &&
		( attr & FILE_ATTRIBUTE_DIRECTORY); 
}

// 递归创建目录， 如果目录已经存在或者创建成功返回TRUE
bool CXY::SuperMkDir(CString P)
{
	int len=P.GetLength();
	if ( len <2 ) return false; 

	if('\\'==P[len-1])
	{
		P=P.Left(len-1);
		len=P.GetLength();
	}
	if ( len <=0 ) return false;

	if (len <=3) 
	{
		if (FolderExists(P))return true;
		else return false; 
	}

	if (FolderExists(P))return true;



	CString Parent;
	Parent=P.Left(P.ReverseFind('\\') );

	if(Parent.GetLength()<=0)return false; 

	bool Ret=SuperMkDir(Parent); 

	if(Ret) 
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength=sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor=NULL;
		sa.bInheritHandle=0;
		Ret=(CreateDirectory(P,&sa)==TRUE);
		return Ret;
	}
	else
		return false;
}

int CXY::GetGameType(CString path)
{
	// 游戏类型：
	// Type = 1 : 梦幻西游
	// Type = 2 : 大话西游2
	// Type = 3 : 大话西游3
	// Type = 4 : 大话外传
	//
	int type=0;
	CString Value;
	CString strUpdatePath = path;
	strUpdatePath += "//update.ini";

	// 屏蔽取版本号判断游戏类型的代码
	//::GetPrivateProfileString(L"Setting",L"Version",L"Default",Value.GetBuffer(10),10,strUpdatePath);
	//char*   p=(LPSTR)(LPCTSTR)Value;
	//char temp='0';
	//memcpy((void *)&temp,p,1);
	//type = atoi(&temp);
	//

	//
	// 梦幻西游  ServerList=http://update.xyq.163.com/
	// 大话西游2 ServerList=http://update.xy.163.com/
	// 大话西游3 ServerList=http://update.xy3.163.com/
	// 大话外传  ServerList=http://update.xyw.163.com/
	//
	::GetPrivateProfileString(L"Setting",L"ServerList",L"Default",Value.GetBuffer(25),25,strUpdatePath);
	int idx = Value.Find(L"xy");
	CString StrTemp;
	StrTemp.SetString(Value.GetString());
	StrTemp= StrTemp.Mid(idx,3);

	if (StrTemp == "xyq")
	{
		type = 1;
	}
	if (StrTemp == "xy.")
	{
		type = 2;
	}
	if (StrTemp == "xy3")
	{
		type = 3;
	}
	if (StrTemp == "xyw")
	{
		type = 4;
	}
	m_GameType = type;
	return type;
}

// 获得文件的列表
CStringList* CXY::GetFileList(CString path)
{
	CString ScenePath;
	switch (m_GameType)
	{
	case 1: //梦幻
		ScenePath = "scene";
		break;
	case 2: //大话2
		ScenePath="newscene";
		//这里不处理大话2的旧地图文件格式
		break;
	case 3: //大话3
		ScenePath = "map";
		break;
	case 4: //大话外传
		ScenePath = "map";
		break;
	}
	path = path+L"\\"+ScenePath;
	CStringList *pFileList=new CStringList[1];
	CStringList *pFileNameList=new CStringList[1]; 
	CStringList *pShowNameList=new CStringList[1];
	BrowseFile(path,L"map",pFileList,pFileNameList,pShowNameList); // 查找所有*.map的文件路径
	m_MapFileList = pFileNameList;
	return pFileList;
}

CStringList* CXY::GetFileName()
{
	return m_MapFileList;
}

void CXY::OpenMap(CString path, CString filename, CString DstPath)
{
	// 
	CString ScenePath;
	m_MapFileName = path;
	m_FileName = filename;
	switch(m_GameType)
	{
	case 1: 
		m_GameName="xyq"; // 梦幻西游
		ScenePath="scene";
		break;
	case 2:
		m_GameName="xy2"; // 大话西游2
		ScenePath="newscene";
		break;
	case 3:
		m_GameName="xy3"; // 大话西游3
		ScenePath="map";
		break;
	case 4:
		m_GameName="xyw"; // 大话外传
		ScenePath="map";
		break;
	}

	DstPath += "\\NetEase\\\\";
	DstPath += m_GameName;
	DstPath += "\\\\";
	DstPath += ScenePath;
	m_OutFileName = DstPath;

	SuperMkDir(DstPath); // 创建多层目录
	

	//////////
	// 在这处理Map文件
	ReadGameMap *map = new ReadGameMap();
	string str = WideCharToMultiChar((LPCTSTR)path);
	map->LoadMap(str);
	uint32 UnitTotal=map->GetSubMapTotal();				// 地图的单元总数
	uint32 m_MapWidth=map->GetMapWidth();				// 地图的宽度
	uint32 m_MapHeight=map->GetMapHeight();				// 地图的高度
	uint32 m_SubMapWidth=map->GetSubMapWidth();			// 子地图的宽度
	uint32 m_SubMapHeight=map->GetSubMapHeight();		// 子地图的高度
	uint32 m_SubMapRowNum=map->GetSubMapRowNum();		// 子地图的行数量
	uint32 m_SubMapColNum=map->GetSubMapColNum();		// 子地图的列数量
	uint32 m_SubMapTotal=map->GetSubMapTotal();			// 子地图的总数

	BmpOffset bo(m_SubMapWidth*m_SubMapColNum,m_SubMapHeight*m_SubMapRowNum,m_SubMapWidth,m_SubMapHeight);
	uint32 bmpsize = m_SubMapWidth*m_SubMapHeight*m_SubMapTotal; // 得到大地图的像素大小
	uint16 *m_BmpData = new uint16[bmpsize];
	//memset(m_BmpData,0,bmpsize*2);

	// 循环处理所有的地图单元
	for (uint32 i=0; i< UnitTotal; i++)
	{	
		MapData jd;
		map->ReadUnit(i); // 读取i单元的数据到内存
		jd=map->GetJpegData(); // 从内存中读取JPEG的数据

		uint32 TempSize = 0;
		uint8* jpgdata;


		// 处理地图JPEG数据为标准的JPEG数据
		jpgdata	= MapHandler(jd.Data,jd.Size,&TempSize);

		// 在这里输出jpegdata数据为单元小地图
		if (isSmall)
		{
			CString JpegFileName = m_OutFileName + L"\\" + L"SMALL\\" + filename; // 目录名
			SuperMkDir(JpegFileName); // 创建目录
			CString IntName;
			IntName.Format(L"%04d",i);
			JpegFileName=JpegFileName + L"\\" + IntName + L".jpg";
			if (isFileExist)
			{
				if (!FileExist(JpegFileName))
				{
					CFile cf;
					if(cf.Open(JpegFileName,CFile::modeCreate|CFile::modeWrite))
					{
						cf.Write(jpgdata,TempSize); // 输出jpeg小块图片
					}
					cf.Close();
				}
			}else
			{
				CFile cf;
				if(cf.Open(JpegFileName,CFile::modeCreate|CFile::modeWrite))
				{
					cf.Write(jpgdata,TempSize); // 输出jpeg小块图片
				}
				cf.Close();
			}
		}

		BMP *bmpdata;
		init_jpeg();
		bmpdata = Unpak_jpg(jpgdata,TempSize);

		uint32 BmpOffset=0;
		BmpOffset=bo.FindOffsetBmp(bo.UnitSwitch(i));

		WritePixel(m_BmpData,(uint16*)bmpdata->line[0],m_SubMapWidth*m_SubMapColNum,m_SubMapHeight*m_SubMapRowNum,m_SubMapWidth,m_SubMapHeight,BmpOffset);

		delete[] bmpdata;
		delete[] jpgdata;
		delete[] ((uint8*)jd.Data);

		// 在这里处理CELL数据


		// 在这里处理MASK数据



		map->ReleaseUnit(); // 释放单元内存空间
		Sleep(10); // 延迟10毫秒，防止占用100%资源
	}


	//printf("\n解码完成，等待生成图像文件...\n");
	uint16 *m_OutBmpData = new uint16[m_MapWidth*m_MapHeight];
	//memset(m_OutBmpData,0,m_MapWidth*m_MapHeight*2);
	uint16 *DstData = m_OutBmpData;
	uint16 *SrcData = m_BmpData;
	// 转换成实际地图大小，除去无用的数据。
	uint32 i=0;
	uint32 j=0;
	uint32 jumpRow=(m_SubMapHeight*m_SubMapRowNum - m_MapHeight)*m_SubMapWidth*m_SubMapColNum; // 跳过行像素,到指定的位置,图片尾部移出多余的行像素
	*(SrcData+=jumpRow);

	for (i=0; i<m_MapHeight; i++)
	{
		for (j=0; j<m_MapWidth; j++) // 处理一行中每列的像素
		{
			*DstData++=*SrcData;
			*SrcData++;
		}
		if (j <= m_MapWidth) // 跳过一行中后面多余的列像素
		{
			for (uint32 k=0; k< m_SubMapWidth*m_SubMapColNum-m_MapWidth; k++)
			{
				*SrcData++;
			}
		}
	}


	TGA_FILE_HEADER TgaHeader;
	//memset(&TgaHeader,0,18);
	TgaHeader.IdLength=0;			// 图像信息字段(默认:0)
	TgaHeader.ColorMapType=0;		// 颜色标的类型(默认0)
	TgaHeader.ImageType=0x02;			// 图像类型码(支持2或10)
	TgaHeader.ColorMapFirstIndex=0;	// 颜色表的引索(默认:0)
	TgaHeader.ColorMapLength=0;		// 颜色表的长度(默认:0)
	TgaHeader.ColorMapEntrySize=0;	// 颜色表表项的为数(默认:0，支持16/24/32)
	TgaHeader.XOrigin=0;				// 图像X坐标的起始位置(默认:0)
	TgaHeader.YOrigin=0;				// 图像Y坐标的起始位置(默认:0)
	TgaHeader.ImageWidth=m_MapWidth;			// 图像的宽度
	TgaHeader.ImageHeight=m_MapHeight;			// 图像的高度
	//TgaHeader.ImageWidth=m_SubMapWidth*m_SubMapColNum;
	//TgaHeader.ImageHeight=m_SubMapHeight*m_SubMapRowNum;
	TgaHeader.PixelDepth=16;			// 图像每像素存储占用位数
	TgaHeader.ImageDescruptor=0;		// 图像描述字符字节(默认:0)


	if (isBig)
	{
		CString TgaFileName = m_OutFileName + L"\\" + filename + L".tga";
		if (isFileExist)
		{
			if (!FileExist(TgaFileName))
			{
				CFile cf;
				if(cf.Open(TgaFileName,CFile::modeCreate|CFile::modeWrite))
				{
					cf.Write(&TgaHeader,18);
					cf.Write(m_OutBmpData,m_MapWidth*m_MapHeight*2);
					//cf.Write(m_BmpData,TgaHeader.ImageWidth*TgaHeader.ImageHeight*2);
				}
				cf.Close();
			}
		}else
		{
			CFile cf;
			if(cf.Open(TgaFileName,CFile::modeCreate|CFile::modeWrite))
			{
				cf.Write(&TgaHeader,18);
				cf.Write(m_OutBmpData,m_MapWidth*m_MapHeight*2);
				//cf.Write(m_BmpData,TgaHeader.ImageWidth*TgaHeader.ImageHeight*2);
			}
			cf.Close();
		}
	}
	//ScenePath.ReleaseBuffer();
	delete[] m_OutBmpData;
	delete[] m_BmpData;
	
	
}

string CXY::WideCharToMultiChar(wstring str)
{
	string return_value;
	//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的
	int len=WideCharToMultiByte(CP_ACP,0,str.c_str(),str.size(),NULL,0,NULL,NULL);
	char *buffer=new char[len+1];
	WideCharToMultiByte(CP_ACP,0,str.c_str(),str.size(),buffer,len,NULL,NULL);
	buffer[len]='\0';
	//删除缓冲区并返回值
	return_value.append(buffer);
	delete []buffer;
	return return_value;
}

wstring CXY::MultCHarToWideChar(string str)
{
	//获取缓冲区的大小，并申请空间，缓冲区大小是按字符计算的
	int len=MultiByteToWideChar(CP_ACP,0,str.c_str(),str.size(),NULL,0);
	TCHAR *buffer=new TCHAR[len+1];
	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP,0,str.c_str(),str.size(),buffer,len);
	buffer[len]='\0';//添加字符串结尾
	//删除缓冲区并返回值
	wstring return_value;
	return_value.append(buffer);
	delete []buffer;
	return return_value;
}

// 地图的JPEG数据处理器
uint8* CXY::MapHandler(uint8* Buffer, uint32 inSize, uint32* outSize)
{
	// JPEG数据处理原理
	// 1、复制D8到D9的数据到缓冲区中
	// 2、删除第3、4个字节 FFA0
	// 3、修改FFDA的长度00 09 为 00 0C
	// 4、在FFDA数据的最后添加00 3F 00
	// 5、替换FFDA到FF D9之间的FF数据为FF 00

	uint32 TempNum = 0;						// 临时变量，表示已读取的长度
	uint8* outBuffer;
	TempBuffer = new uint8[inSize*2];		// 临时变量，表示处理后的数据
	memset(TempBuffer,0,inSize*2);
	outBuffer = TempBuffer;					// 已处理数据的开始地址
	uint16 TempTimes = 0;					// 临时变量，表示循环的次数
	uint32 Temp = 0;

	// 当已读取数据的长度小于总长度时继续
	while(TempNum < inSize && *Buffer++==0xFF)
	{
		*TempBuffer++=0xFF;
		TempNum++;
		switch(*Buffer)
		{
		case 0xD8:
			*TempBuffer++=0xD8;
			*Buffer++;
			TempNum++;
			break;
		case 0xA0:
			*Buffer++;
			TempBuffer--;
			TempNum++;
			break;
		case 0xC0:
			*TempBuffer++=0xC0;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(uint16)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序


			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}

			break;
		case 0xC4:
			*TempBuffer++=0xC4;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(uint16)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			break;
		case 0xDB:
			*TempBuffer++=0xDB;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(uint16)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序

			for(int i=0;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			break;
		case 0xDA:
			*TempBuffer++=0xDA;
			*TempBuffer++=0x00;
			*TempBuffer++=0x0C;
			*Buffer++;
			TempNum++;

			memcpy(&TempTimes,Buffer,sizeof(uint16)); // 读取长度
			ByteSwap(TempTimes); // 将长度转换为Intel顺序
			*Buffer++;
			TempNum++;
			*Buffer++;

			for(int i=2;i< TempTimes;i++)
			{
				*TempBuffer++=*Buffer++;
				TempNum++;
			}
			*TempBuffer++=0x00;
			*TempBuffer++=0x3F;
			*TempBuffer++=0x00;
			Temp += 1; // 这里应该是+3的，因为前面的0xFFA0没有-2，所以这里只+1。

			// 循环处理0xFFDA到0xFFD9之间所有的0xFF替换为0xFF00
			for (;TempNum < inSize-2;)
			{
				if (*Buffer==0xFF)
				{
					*TempBuffer++=0xFF;
					*TempBuffer++=0x00;
					*Buffer++;
					TempNum++;	
					Temp ++;
				}
				else
				{
					*TempBuffer++=*Buffer++;
					TempNum++;
				}
			}
			// 直接在这里写上了0xFFD9结束Jpeg图片.
			Temp--; // 这里多了一个字节，所以减去。
			*TempBuffer--;
			*TempBuffer--=0xD9;
			break;
		case 0xD9:
			// 算法问题，这里不会被执行，但结果一样。
			*TempBuffer++=0xD9;
			TempNum++;
			break;
		default:
			break;
		}
	}
	Temp+=inSize;
	*outSize=Temp;
	return outBuffer;
}

// 2字节高低位调换
void CXY::ByteSwap(uint16& value)
{
	uint16 tempvalue = value >> 8;
	value = (value << 8) | tempvalue;
}

// 2字节颜色格式转换
uint16 CXY::RGB565to555(uint16 &Value)
{
	uint16 Result=0;
	uint16 R,G,B;
	R=G=B=0;

	// 掩码 
	// R = 0xF800
	// G = 0x07E0
	// B = 0x001F

	R = Value & 0xF800;
	G = Value & 0x07E0;
	B = Value & 0x001F;

	R = R >> 1;
	G = G >> 6;
	G = G << 5;

	Result = R|G|B;
	return Result;
}

// 写一个像素单元
void CXY::WritePixel(uint16* Buffer, uint16* SubBmp, uint32 MapWidth, uint32 MapHeight, uint32 SubMapWidth, uint32 SubMapHeight, uint32 Offset)
{
	// 位的顺序和像素表示的顺序是一致的，由左到右 上到下的顺序
	uint32 temp=0;

	uint16* TempOffset=Buffer; // 地图数据的临时数据指针位置

	*(TempOffset+=Offset); // 跳转

	// 从最后一行像素开始写起
	for (int j=0;j<SubMapHeight;j++) // 行循环
	{
		TempOffset=Buffer;
		temp=MapWidth*(SubMapHeight-1-j)+Offset; // 计算最底行偏移值
		//temp=MapWidth*j+Offset; // 顶行开始写，图像颠倒
		*(TempOffset+=temp); // 跳转到正确的行偏移位置

		for(int i=0;i<SubMapWidth;i++) // 列循环
		{		
			*TempOffset++=*SubBmp;
			*SubBmp++;
		}
	}
}

void CXY::SetIsBig(BOOL b)
{

	isBig = b;
}

void CXY::SetIsSmall(BOOL b)
{
	isSmall = b;
}

void CXY::SetIsMask(BOOL b)
{
	isMask = b;
}

void CXY::SetIsCell(BOOL b)
{
	isCell = b;
}

void CXY::SetIsFileExist(BOOL b)
{
	isFileExist = b;
}