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
		res = FileFind.FindNextFile(); // ��һ���ļ�
		if(FileFind.IsDirectory() && !FileFind.IsDots()) // �����Ŀ¼�Ͳ��ǵ�
		{   
			//�����һ����Ŀ¼���õݹ��������һ����   
			CString strPath = FileFind.GetFilePath();   
			CString strTitle = FileFind.GetFileTitle();   
			BrowseFile(strPath, strFilter, pFileList, pFileNameList, pShowNameList); // �ݹ�
		}
		else   if(!FileFind.IsDirectory() && !FileFind.IsDots()) // �������Ŀ¼�Ͳ��ǵ�
		{   
			//��ʾ��ǰ���ʵ��ļ�   
			CString strExtName; // ��׺��
			CString strName;    // �ļ���
			strName = FileFind.GetFileName(); // ����ļ��� 
			strExtName = strName.Right(strName.GetLength() - strName.Find('.')-1); // ��ú�׺�� 
			strExtName = strExtName.Left(strFilter.GetLength()); // Ϊ��ƥ����˵ĺ�׺��
			strExtName.MakeUpper(); // ת���ɴ�д  
			if(!strExtName.CompareNoCase(strFilter))//��չ����ȷ   
			{
				CString strPath = FileFind.GetFilePath();   
				pFileList->AddTail(strPath); 
				pFileNameList->AddTail(strName);   
				strName = strName.Left(strName.Find('.'));   
				pShowNameList->AddTail(strName);   
			}   
		}   
	}   
	FileFind.Close();//�ر�   
	return   TRUE; 
}

// �ж��ļ��Ƿ����
bool CXY::FileExist(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName); 
}

// �ж�Ŀ¼�Ƿ����
bool CXY::FolderExists(CString s)
{
	DWORD attr; 
	attr = GetFileAttributes(s); 
	return (attr != (DWORD)(-1) ) &&
		( attr & FILE_ATTRIBUTE_DIRECTORY); 
}

// �ݹ鴴��Ŀ¼�� ���Ŀ¼�Ѿ����ڻ��ߴ����ɹ�����TRUE
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
	// ��Ϸ���ͣ�
	// Type = 1 : �λ�����
	// Type = 2 : ������2
	// Type = 3 : ������3
	// Type = 4 : ���⴫
	//
	int type=0;
	CString Value;
	CString strUpdatePath = path;
	strUpdatePath += "//update.ini";

	// ����ȡ�汾���ж���Ϸ���͵Ĵ���
	//::GetPrivateProfileString(L"Setting",L"Version",L"Default",Value.GetBuffer(10),10,strUpdatePath);
	//char*   p=(LPSTR)(LPCTSTR)Value;
	//char temp='0';
	//memcpy((void *)&temp,p,1);
	//type = atoi(&temp);
	//

	//
	// �λ�����  ServerList=http://update.xyq.163.com/
	// ������2 ServerList=http://update.xy.163.com/
	// ������3 ServerList=http://update.xy3.163.com/
	// ���⴫  ServerList=http://update.xyw.163.com/
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

// ����ļ����б�
CStringList* CXY::GetFileList(CString path)
{
	CString ScenePath;
	switch (m_GameType)
	{
	case 1: //�λ�
		ScenePath = "scene";
		break;
	case 2: //��2
		ScenePath="newscene";
		//���ﲻ�����2�ľɵ�ͼ�ļ���ʽ
		break;
	case 3: //��3
		ScenePath = "map";
		break;
	case 4: //���⴫
		ScenePath = "map";
		break;
	}
	path = path+L"\\"+ScenePath;
	CStringList *pFileList=new CStringList[1];
	CStringList *pFileNameList=new CStringList[1]; 
	CStringList *pShowNameList=new CStringList[1];
	BrowseFile(path,L"map",pFileList,pFileNameList,pShowNameList); // ��������*.map���ļ�·��
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
		m_GameName="xyq"; // �λ�����
		ScenePath="scene";
		break;
	case 2:
		m_GameName="xy2"; // ������2
		ScenePath="newscene";
		break;
	case 3:
		m_GameName="xy3"; // ������3
		ScenePath="map";
		break;
	case 4:
		m_GameName="xyw"; // ���⴫
		ScenePath="map";
		break;
	}

	DstPath += "\\NetEase\\\\";
	DstPath += m_GameName;
	DstPath += "\\\\";
	DstPath += ScenePath;
	m_OutFileName = DstPath;

	SuperMkDir(DstPath); // �������Ŀ¼
	

	//////////
	// ���⴦��Map�ļ�
	ReadGameMap *map = new ReadGameMap();
	string str = WideCharToMultiChar((LPCTSTR)path);
	map->LoadMap(str);
	uint32 UnitTotal=map->GetSubMapTotal();				// ��ͼ�ĵ�Ԫ����
	uint32 m_MapWidth=map->GetMapWidth();				// ��ͼ�Ŀ��
	uint32 m_MapHeight=map->GetMapHeight();				// ��ͼ�ĸ߶�
	uint32 m_SubMapWidth=map->GetSubMapWidth();			// �ӵ�ͼ�Ŀ��
	uint32 m_SubMapHeight=map->GetSubMapHeight();		// �ӵ�ͼ�ĸ߶�
	uint32 m_SubMapRowNum=map->GetSubMapRowNum();		// �ӵ�ͼ��������
	uint32 m_SubMapColNum=map->GetSubMapColNum();		// �ӵ�ͼ��������
	uint32 m_SubMapTotal=map->GetSubMapTotal();			// �ӵ�ͼ������

	BmpOffset bo(m_SubMapWidth*m_SubMapColNum,m_SubMapHeight*m_SubMapRowNum,m_SubMapWidth,m_SubMapHeight);
	uint32 bmpsize = m_SubMapWidth*m_SubMapHeight*m_SubMapTotal; // �õ����ͼ�����ش�С
	uint16 *m_BmpData = new uint16[bmpsize];
	//memset(m_BmpData,0,bmpsize*2);

	// ѭ���������еĵ�ͼ��Ԫ
	for (uint32 i=0; i< UnitTotal; i++)
	{	
		MapData jd;
		map->ReadUnit(i); // ��ȡi��Ԫ�����ݵ��ڴ�
		jd=map->GetJpegData(); // ���ڴ��ж�ȡJPEG������

		uint32 TempSize = 0;
		uint8* jpgdata;


		// �����ͼJPEG����Ϊ��׼��JPEG����
		jpgdata	= MapHandler(jd.Data,jd.Size,&TempSize);

		// ���������jpegdata����Ϊ��ԪС��ͼ
		if (isSmall)
		{
			CString JpegFileName = m_OutFileName + L"\\" + L"SMALL\\" + filename; // Ŀ¼��
			SuperMkDir(JpegFileName); // ����Ŀ¼
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
						cf.Write(jpgdata,TempSize); // ���jpegС��ͼƬ
					}
					cf.Close();
				}
			}else
			{
				CFile cf;
				if(cf.Open(JpegFileName,CFile::modeCreate|CFile::modeWrite))
				{
					cf.Write(jpgdata,TempSize); // ���jpegС��ͼƬ
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

		// �����ﴦ��CELL����


		// �����ﴦ��MASK����



		map->ReleaseUnit(); // �ͷŵ�Ԫ�ڴ�ռ�
		Sleep(10); // �ӳ�10���룬��ֹռ��100%��Դ
	}


	//printf("\n������ɣ��ȴ�����ͼ���ļ�...\n");
	uint16 *m_OutBmpData = new uint16[m_MapWidth*m_MapHeight];
	//memset(m_OutBmpData,0,m_MapWidth*m_MapHeight*2);
	uint16 *DstData = m_OutBmpData;
	uint16 *SrcData = m_BmpData;
	// ת����ʵ�ʵ�ͼ��С����ȥ���õ����ݡ�
	uint32 i=0;
	uint32 j=0;
	uint32 jumpRow=(m_SubMapHeight*m_SubMapRowNum - m_MapHeight)*m_SubMapWidth*m_SubMapColNum; // ����������,��ָ����λ��,ͼƬβ���Ƴ������������
	*(SrcData+=jumpRow);

	for (i=0; i<m_MapHeight; i++)
	{
		for (j=0; j<m_MapWidth; j++) // ����һ����ÿ�е�����
		{
			*DstData++=*SrcData;
			*SrcData++;
		}
		if (j <= m_MapWidth) // ����һ���к�������������
		{
			for (uint32 k=0; k< m_SubMapWidth*m_SubMapColNum-m_MapWidth; k++)
			{
				*SrcData++;
			}
		}
	}


	TGA_FILE_HEADER TgaHeader;
	//memset(&TgaHeader,0,18);
	TgaHeader.IdLength=0;			// ͼ����Ϣ�ֶ�(Ĭ��:0)
	TgaHeader.ColorMapType=0;		// ��ɫ�������(Ĭ��0)
	TgaHeader.ImageType=0x02;			// ͼ��������(֧��2��10)
	TgaHeader.ColorMapFirstIndex=0;	// ��ɫ�������(Ĭ��:0)
	TgaHeader.ColorMapLength=0;		// ��ɫ��ĳ���(Ĭ��:0)
	TgaHeader.ColorMapEntrySize=0;	// ��ɫ������Ϊ��(Ĭ��:0��֧��16/24/32)
	TgaHeader.XOrigin=0;				// ͼ��X�������ʼλ��(Ĭ��:0)
	TgaHeader.YOrigin=0;				// ͼ��Y�������ʼλ��(Ĭ��:0)
	TgaHeader.ImageWidth=m_MapWidth;			// ͼ��Ŀ��
	TgaHeader.ImageHeight=m_MapHeight;			// ͼ��ĸ߶�
	//TgaHeader.ImageWidth=m_SubMapWidth*m_SubMapColNum;
	//TgaHeader.ImageHeight=m_SubMapHeight*m_SubMapRowNum;
	TgaHeader.PixelDepth=16;			// ͼ��ÿ���ش洢ռ��λ��
	TgaHeader.ImageDescruptor=0;		// ͼ�������ַ��ֽ�(Ĭ��:0)


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
	//��ȡ�������Ĵ�С��������ռ䣬��������С�ǰ��ֽڼ����
	int len=WideCharToMultiByte(CP_ACP,0,str.c_str(),str.size(),NULL,0,NULL,NULL);
	char *buffer=new char[len+1];
	WideCharToMultiByte(CP_ACP,0,str.c_str(),str.size(),buffer,len,NULL,NULL);
	buffer[len]='\0';
	//ɾ��������������ֵ
	return_value.append(buffer);
	delete []buffer;
	return return_value;
}

wstring CXY::MultCHarToWideChar(string str)
{
	//��ȡ�������Ĵ�С��������ռ䣬��������С�ǰ��ַ������
	int len=MultiByteToWideChar(CP_ACP,0,str.c_str(),str.size(),NULL,0);
	TCHAR *buffer=new TCHAR[len+1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP,0,str.c_str(),str.size(),buffer,len);
	buffer[len]='\0';//����ַ�����β
	//ɾ��������������ֵ
	wstring return_value;
	return_value.append(buffer);
	delete []buffer;
	return return_value;
}

// ��ͼ��JPEG���ݴ�����
uint8* CXY::MapHandler(uint8* Buffer, uint32 inSize, uint32* outSize)
{
	// JPEG���ݴ���ԭ��
	// 1������D8��D9�����ݵ���������
	// 2��ɾ����3��4���ֽ� FFA0
	// 3���޸�FFDA�ĳ���00 09 Ϊ 00 0C
	// 4����FFDA���ݵ�������00 3F 00
	// 5���滻FFDA��FF D9֮���FF����ΪFF 00

	uint32 TempNum = 0;						// ��ʱ��������ʾ�Ѷ�ȡ�ĳ���
	uint8* outBuffer;
	TempBuffer = new uint8[inSize*2];		// ��ʱ��������ʾ����������
	memset(TempBuffer,0,inSize*2);
	outBuffer = TempBuffer;					// �Ѵ������ݵĿ�ʼ��ַ
	uint16 TempTimes = 0;					// ��ʱ��������ʾѭ���Ĵ���
	uint32 Temp = 0;

	// ���Ѷ�ȡ���ݵĳ���С���ܳ���ʱ����
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

			memcpy(&TempTimes,Buffer,sizeof(uint16)); // ��ȡ����
			ByteSwap(TempTimes); // ������ת��ΪIntel˳��


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

			memcpy(&TempTimes,Buffer,sizeof(uint16)); // ��ȡ����
			ByteSwap(TempTimes); // ������ת��ΪIntel˳��

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

			memcpy(&TempTimes,Buffer,sizeof(uint16)); // ��ȡ����
			ByteSwap(TempTimes); // ������ת��ΪIntel˳��

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

			memcpy(&TempTimes,Buffer,sizeof(uint16)); // ��ȡ����
			ByteSwap(TempTimes); // ������ת��ΪIntel˳��
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
			Temp += 1; // ����Ӧ����+3�ģ���Ϊǰ���0xFFA0û��-2����������ֻ+1��

			// ѭ������0xFFDA��0xFFD9֮�����е�0xFF�滻Ϊ0xFF00
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
			// ֱ��������д����0xFFD9����JpegͼƬ.
			Temp--; // �������һ���ֽڣ����Լ�ȥ��
			*TempBuffer--;
			*TempBuffer--=0xD9;
			break;
		case 0xD9:
			// �㷨���⣬���ﲻ�ᱻִ�У������һ����
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

// 2�ֽڸߵ�λ����
void CXY::ByteSwap(uint16& value)
{
	uint16 tempvalue = value >> 8;
	value = (value << 8) | tempvalue;
}

// 2�ֽ���ɫ��ʽת��
uint16 CXY::RGB565to555(uint16 &Value)
{
	uint16 Result=0;
	uint16 R,G,B;
	R=G=B=0;

	// ���� 
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

// дһ�����ص�Ԫ
void CXY::WritePixel(uint16* Buffer, uint16* SubBmp, uint32 MapWidth, uint32 MapHeight, uint32 SubMapWidth, uint32 SubMapHeight, uint32 Offset)
{
	// λ��˳������ر�ʾ��˳����һ�µģ������� �ϵ��µ�˳��
	uint32 temp=0;

	uint16* TempOffset=Buffer; // ��ͼ���ݵ���ʱ����ָ��λ��

	*(TempOffset+=Offset); // ��ת

	// �����һ�����ؿ�ʼд��
	for (int j=0;j<SubMapHeight;j++) // ��ѭ��
	{
		TempOffset=Buffer;
		temp=MapWidth*(SubMapHeight-1-j)+Offset; // ���������ƫ��ֵ
		//temp=MapWidth*j+Offset; // ���п�ʼд��ͼ��ߵ�
		*(TempOffset+=temp); // ��ת����ȷ����ƫ��λ��

		for(int i=0;i<SubMapWidth;i++) // ��ѭ��
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