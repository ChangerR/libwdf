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
#include "stdafx.h"
#include "ReadGameMap.h"

using namespace NetEase; // ʹ��NetEase�����ռ�

ReadGameMap::ReadGameMap(void)
{
}

ReadGameMap::~ReadGameMap(void)
{
}

// ���ص�ͼ�ļ�
bool ReadGameMap::LoadMap(string filename)
{
	ifstream infile;
	m_FileName = filename; // ��ͼ�ļ���

	// �򿪵�ͼ�ļ�
	locale::global(locale(""));//��ȫ��������Ϊ����ϵͳĬ������
	infile.open((char*)filename.c_str(), ios::in|ios::binary);
	locale::global(locale("C"));//��ԭȫ�������趨
	// �ж��ļ��Ƿ����
	if(!infile)
	{
		cerr << "Can't open Map File :" << filename << endl;
		exit(EXIT_FAILURE); // �˳�
	}

	uint32 TempFlag; // ��ʱ�ļ���־
	infile.read((char*)&TempFlag,sizeof(uint32));
	//infile.seekg(-4); // ������4���ֽ�
	infile.seekg(0,ios::beg);

	// �ļ���־
	// 0x4D415058 (MAPX) ��2�ɵ�ͼ
	// 0x4D312E30 (M1.0) ��2�µ�ͼ �λõ�ͼ
	// 0x4D322E35 (M2.5) ��3�ڲ��
	// 0x4D332E30 (M3.0) ��3��ʽ��
	// 0x524F4C30 (ROL0) ��3��ͼ�����ļ�
	if ((TempFlag == 0x4D415058) | (TempFlag == 0x4D312E30) | (TempFlag == 0x4D322E35) | (TempFlag == 0x4D332E30) | (TempFlag == 0x524F4C30))
	{
		switch(TempFlag)
		{
		case 0x4D415058:
			m_FileType = 1; // ��2�ɵ�ͼ��ʽ
			break;
		case 0x4D312E30:
			m_FileType = 2; // ��2�µ�ͼ���λõ�ͼ��ʽ
			break;
		case 0x4D322E35:
			m_FileType = 3; // ��3�ڲ���ͼ��ʽ
			break;
		case 0x4D332E30:
			m_FileType = 4; // ��3��ʽ���ͼ��ʽ
			break;
		case 0x524F4C30:
			m_FileType = 5; // ��3��ͼ�����ļ���ʽ
			break;
		}

		if ((m_FileType == 1) | (m_FileType == 2))
		{
			m_SubMapWidth = 320;
			m_SubMapHeight = 240;
			MapHeader1 *header = new MapHeader1[1];
			ZeroMemory(header,sizeof(MapHeader1));
			infile.read((char*)header,sizeof(MapHeader1));
			m_FileFlag=header->Flag;		// �ļ���־
			m_MapWidth = (float)header->Width;	// ��ͼ�Ŀ��
			m_MapHeight= (float)header->Height;	// ��ͼ�ĸ߶�
			delete [] header;
		}

		if ((m_FileType == 3) | (m_FileType == 4))
		{
			m_SubMapWidth = 400;
			m_SubMapHeight = 320;
			MapHeader3 *header = new MapHeader3[1];
			ZeroMemory(header,sizeof(MapHeader3));
			infile.read((char*)header,sizeof(MapHeader3));
			m_FileFlag=header->Flag;						// �ļ���־
			m_MapWidth = (float)header->Width;					// ��ͼ�Ŀ��
			m_MapHeight= (float)header->Height;					// ��ͼ�ĸ߶�
			m_SubMapWidth = header->SubWidth;			// �ӵ�ͼ�Ŀ��
			m_SubMapHeight = header->SubHeight;			// �ӵ�ͼ�ĸ߶�
			m_PointWidth = header->PointWidth;			// ����Ŀ��
			m_PointHeight = header->PointHeight;			// ����ĸ߶�

			m_Reserved=header->Reserved;					// ����
			m_UnitIndexOffset=header->UnitIndexOffset;	// ��Ԫ������λ��
			m_UnitIndexNum=header->UnitIndexNum;			// ��Ԫ����������
			m_IndexOffset=header->IndexOffset;			// ����λ��(δ֪����)
			m_IndexNum=header->IndexNum;					// ��������(δ֪����)
			delete [] header;
		}

		if (m_FileType == 5)
		{
			m_SubMapWidth = 400;
			m_SubMapHeight = 320;
			Rol0Header *header = new Rol0Header[1];
			ZeroMemory(header,sizeof(Rol0Header));
			infile.read((char*)header,sizeof(Rol0Header));
			m_FileFlag=header->Flag;						// �ļ���־
			m_Reserved=header->Reserved;					// ����
			m_MapWidth = (float)header->Width;					// ��ͼ�Ŀ��
			m_MapHeight= (float)header->Height;					// ��ͼ�ĸ߶�
			m_SubMapWidth = header->SubWidth;			// �ӵ�ͼ�Ŀ��
			m_SubMapHeight = header->SubHeight;			// �ӵ�ͼ�ĸ߶�
			m_UnitIndexNum=header->UnitIndexNum;			// ��Ԫ����������
		}

		// ע����Ϊ��Щ��ͼ�ĳߴ粢һ�����Ա�С��ͼ�ߴ�������������Ҫ�������ȡ��
		m_SubMapRowNum=(uint32)ceil((float)(m_MapHeight/m_SubMapHeight)); // �����ӵ�ͼ�е�������
		m_SubMapColNum=(uint32)ceil((float)(m_MapWidth/m_SubMapWidth)); // �����ӵ�ͼ�е�������
		m_SubMapTotal=m_SubMapRowNum*m_SubMapColNum; // �����ͼ���ܵ��ӵ�ͼ����
		m_UnitIndexNum = m_SubMapTotal;

		// ��ȡ��Ԫƫ��ֵ�б�
		m_UnitOffsetList=new uint32[m_UnitIndexNum]; // �Զ������б�ռ�
		infile.read((char*)m_UnitOffsetList, sizeof(uint32) * m_SubMapTotal); // ��ȡ�б�

		// ����3��ͼʹ��
		if ((m_FileType == 3) | (m_FileType == 4))
		{
			// ��ȡδ֪����ƫ��ֵ�б�
			m_OffsetList=new uint32[m_IndexNum]; // ����(δ֪����)���б�ռ�
			infile.read((char*)m_OffsetList, sizeof(uint32) * m_IndexNum); // ��ȡ�б�(δ֪����)
		}
	}
	else
	{
		cerr << filename << ": Map File Format Error!" << endl;
		exit(EXIT_FAILURE); // �˳�
	}

	if (m_FileType == 1)
	{
		//ReadJPGH(&infile); // ��ȡJPGH������
	}

	if ((m_FileType == 2) | (m_FileType == 3) | (m_FileType == 4))
	{
		//ReadHEAD(&infile); // ��ȡ�µ�ͼ������
	}

	if (m_FileType == 5)
	{
		// �˸�ʽ���账��
	}
	infile.close(); // �ر��ļ�
	return true;
}

// ��ȡ�ɵ�ͼJPGH������
bool ReadGameMap::ReadJPGH(ifstream *file)
{	
	UnitHeader JpegHead;
	uint32 FileSize = 0; // �ļ���С
	file->read((char*)&FileSize,sizeof(uint32));
	file->read((char *)&JpegHead,sizeof(JpegHead)); // ��ȡ��Ԫͷ������

	// �жϱ�־�Ƿ���ȷ(HGPJ)
	if (JpegHead.Flag!=0x4A504748)
	{
		printf("JPEG HEADER ��־����\n");
		return false;
	}
	m_jpgh.Data = new uint8[JpegHead.Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
	m_jpgh.Size=JpegHead.Size;
	file->read((char*)m_jpgh.Data,JpegHead.Size);
	return true;
}

// ��ȡ�µ�ͼHEAD������
bool ReadGameMap::ReadHEAD(ifstream *file)
{	
	uint32 HeadOffset = 0; // HEAD��ƫ��λ��ֵ
	file->read((char*)&HeadOffset,sizeof(uint32));
	uint32 HeaderSize;

	HeaderSize=m_UnitOffsetList[0]-HeadOffset; // �����ͼͷ���ݵĴ�С

	m_head.Size=HeaderSize;
	m_head.Data = new uint8[HeaderSize];
	file->read((char*)m_head.Data,sizeof(uint8)); // ��ȡ��ͼͷ����
	return true;
}

// ��ȡ��ͼ�ĵ�Ԫ����
bool ReadGameMap::ReadUnit(uint32 UnitNum)
{
	ifstream file;
	locale::global(locale(""));//��ȫ��������Ϊ����ϵͳĬ������
	file.open(m_FileName.c_str(), ios::in|ios::binary);
	locale::global(locale("C"));//��ԭȫ�������趨
	long seek;			// ��ת��ַ
	bool Result = true;		// ���
	bool loop=true;		// �Ƿ�ѭ��
	m_MaskTemp=1;

	m_isImag=false;
	m_isMask=false;
	m_isJpeg=false;
	m_isBlok=false;
	m_isCell=false;
	m_isBrig=false;
	m_isLigt=false;

	seek=m_UnitOffsetList[UnitNum];
	file.seekg(seek); // ��ת����Ԫ��ʼ��λ��
	
	if ((m_FileType == 1) | (m_FileType == 2))
	{
		uint32 Num = 0; // ��Ԫ��ʼ��ͷ4���ֽ�
		file.read((char*)&Num,sizeof(uint32));
		m_UnitHeadData = new uint32[Num];
		for (int i=0;i<Num;i++)
		{
			file.read((char*)&m_UnitHeadData[i],sizeof(uint32));
		}
	}


	UnitHeader *unit = new UnitHeader[1];
	ZeroMemory(unit,sizeof(UnitHeader));

	while(loop)
	{
		file.read((char*)unit,sizeof(UnitHeader));

		switch(unit->Flag)
		{
			// GAMI "47 414D 49"
		case 0x494D4147:
			Result=ReadIMAG(&file,unit->Flag,unit->Size);
			break;

			// 2KSM "32 4B 53 4D"
		case 0x4D534B32:
			Result=ReadMASK(&file,unit->Flag,unit->Size);
			break;
			// KSAM "4B 53 41 4D"
		case 0x4D41534B:
			Result=ReadMASK(&file,unit->Flag,unit->Size);
			break;

			// GEPJ "47 45 50 4A"
		case 0x4A504547:
			Result=ReadJPEG(&file,unit->Flag,unit->Size);

			if (m_FileFlag==0x524F4C30) // �Ƿ�ΪROL�ļ�
			{
				loop=false;
			}

			break;

			// KOLB "4B 4F 4C 42"
		case 0x424C4F4B:
			Result=ReadBLOK(&file,unit->Flag,unit->Size);
			break;

			// LLEC "4C 4C 45 43"
		case 0x43454C4C:
			Result=ReadCELL(&file,unit->Flag,unit->Size);
			break;

			// GIRB "47 49 52 42"
		case 0x42524947:
			Result=ReadBRIG(&file,unit->Flag,unit->Size);
			break;

			// TGIL "54 47 49 4C"
		case 0x4C494754:
			Result=ReadLIGT(&file,unit->Flag,unit->Size);
			break;

			//  DNE "20 44 4E 45"
		case 0x454E4420:
			Result=ReadEND(&file,unit->Flag,unit->Size);
			loop=false;
			break;

			// Ĭ�ϴ���
		default:
			// �����־
			m_ErrorUnit.Flag=unit->Flag;
			m_ErrorUnit.Size=unit->Size;
			loop=false;
			break;
		}
	}

	file.close(); // �ر�MAP�ļ�
	return Result;
}

// �ͷŵ�ͼ��Ԫ���ڴ�ռ�
bool ReadGameMap::ReleaseUnit()
{
	if (m_isImag)
	{
		m_imag.Data = NULL;
		delete[] ((uint8*)m_imag.Data);
	}
	if (m_isJpeg)
	{
		m_jpeg.Data = NULL;
		delete[] ((uint8*)m_jpeg.Data); // �ͷ�JPEG���ڴ�ռ�
	}
	if (m_isCell)
	{
		m_cell.Data = NULL;
		delete[] ((uint8*)m_cell.Data); // �ͷ�CELL���ڴ�ռ�
	}
	if (m_isMask)
	{
		//mask�����鷳Щ
		delete[] m_mask[0].Data;
	}
	if (m_isBlok)
	{
		m_blok.Data = NULL;
		delete[] ((uint8*)m_blok.Data);
	}
	if (m_isBrig)
	{
		m_brig.Data = NULL;
		delete[] ((uint8*)m_brig.Data);
	}
	if (m_isLigt)
	{
		m_ligt.Data = NULL;
		delete[] ((uint8*)m_ligt.Data); // �ͷ�LIGT���ڴ�ռ�
	}

	return true;
}

// ��õ�ͼ���
uint32 ReadGameMap::GetMapWidth()
{
	return (uint32)m_MapWidth;
}

// ��õ�ͼ�߶�
uint32 ReadGameMap::GetMapHeight()
{
	return (uint32)m_MapHeight;
}

// ����ӵ�ͼ�Ŀ��
uint32 ReadGameMap::GetSubMapWidth()
{
	return (uint32)m_SubMapWidth;
}

// ����ӵ�ͼ�ĸ߶�
uint32 ReadGameMap::GetSubMapHeight()
{
	return (uint32)m_SubMapHeight;
}

// �������Ŀ��
uint32 ReadGameMap::GetPointWidth()
{
	return m_PointWidth;
}

// �������ĸ߶�
uint32 ReadGameMap::GetPointHeight()
{
	return m_PointHeight;
}

// ��õ�ͼ������
uint32 ReadGameMap::GetSubMapRowNum()
{
	return m_SubMapRowNum;
}

// ��õ�ͼ������
uint32 ReadGameMap::GetSubMapColNum()
{
	return m_SubMapColNum;
}

// ����ӵ�ͼ��������
uint32 ReadGameMap::GetSubMapTotal()
{
	return m_SubMapTotal;
}

// ����ӵ�ͼ��Mask������
uint32 ReadGameMap::GetMaskTotal()
{
	return m_MaskNum;
}

// ���ROL0��Ԫ����
bool ReadGameMap::ReadROLO(ifstream *file, uint32 Size)
{
	if (m_FileFlag==0x524F4C30) // �Ƿ�ΪROL�ļ�
	{
		m_isJpeg = true;
		m_jpeg.Data = new uint8[Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
		//fread(m_jpeg.Data,Size,1,fp); // ��ȡ��ԪJPEG������
		m_jpeg.Size = Size;
		return true;
	}
	return false;
}

// ��ȡ��ͼIMAG������
bool ReadGameMap::ReadIMAG(ifstream *file, uint32 Flag, uint32 Size)
{
	if (Flag==0x494D4147)
	{
		m_isImag = true;
		m_imag.Data = new uint8[Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
		file->read((char*)m_imag.Data,Size);// ��ȡ��ԪIMAG������
		m_imag.Size=Size;
	}
	else
	{
		printf("IMAG��־����\n");
		m_isImag = false;
		return false;
	}
	return true;
}

// ��ȡ��ͼMASK������
bool ReadGameMap::ReadMASK(ifstream *file, uint32 Flag, uint32 Size)
{
	if (Flag==0x4D41534B || Flag==0x4D534B32)
	{
		// ���������ܴ�������,ȱ��ѭ����־
		m_isMask = true;
		if ((m_FileType == 1) | (m_FileType == 2))
		{
			m_mask[0].Data = new uint8[Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
			file->read((char*)m_mask[0].Data,Size);// ��ȡ��ԪIMAG������
			m_mask[0].Size=Size;
		}
		if ((m_FileType == 3) | (m_FileType == 4))
		{
			for (int i=0;i<Size;i++)
			{
				m_mask[i].Data = new uint8[4];
				file->read((char*)m_mask[i].Data,4);
			}
		}
		
	}
	else
	{
		printf("MASK��־����\n");
		m_isMask = false;
		return false;
	}
	return true;
}

// ��ȡ��ͼJPEG������
bool ReadGameMap::ReadJPEG(ifstream *file, uint32 Flag, uint32 Size)
{
	if (Flag==0x4A504547)
	{
		m_isJpeg = true;
		m_jpeg.Data = new uint8[Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
		file->read((char*)m_jpeg.Data,Size);// ��ȡ��ԪJPEG������
		m_jpeg.Size=Size;
	}
	else
	{
		printf("JPEG��־����\n");
		m_isJpeg = false;
		return false;
	}
	return true;
}

// ��ȡ��ͼBLOK������
bool ReadGameMap::ReadBLOK(ifstream *file, uint32 Flag, uint32 Size)
{
	if (Flag==0x424C4F4B)
	{
		m_isBlok = true;
		m_blok.Data = new uint8[Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
		file->read((char*)m_blok.Data,Size); // ��ȡ��ԪBLOK������
		m_blok.Size=Size;
	}
	else
	{
		printf("BLOK��־����\n");
		m_isBlok = false;
		return false;
	}
	return true;
}

// ��ȡ��ͼCELL������
bool ReadGameMap::ReadCELL(ifstream *file, uint32 Flag, uint32 Size)
{
	if (Flag==0x43454C4C)
	{
		m_isCell = true;
		m_cell.Data = new uint8[Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
		file->read((char*)m_cell.Data,Size); // ��ȡ��ԪCELL������
		m_cell.Size=Size;
	}
	else
	{
		printf("CELL��־����\n");
		m_isCell = false;
		return false;
	}
	return true;
}

// ��ȡ��ͼBRIG������
bool ReadGameMap::ReadBRIG(ifstream *file, uint32 Flag, uint32 Size)
{
	if (Flag==0x42524947)
	{
		m_isBrig = true;
		m_brig.Data = new uint8[Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
		file->read((char*)m_brig.Data,Size);
		m_brig.Size=Size;
	}
	else
	{
		printf("BRIG��־����\n");
		m_isCell = false;
		return false;
	}
	return true;
}

// ��ȡ��ͼLIGT������
bool ReadGameMap::ReadLIGT(ifstream *file, uint32 Flag, uint32 Size)
{
	if (Flag==0x4C494754)
	{
		m_isLigt = true;
		m_ligt.Data = new uint8[Size]; // ���䵥Ԫ���ݵ��ڴ�ռ�
		file->read((char*)m_ligt.Data,Size); // ��ȡ��ԪJPEG������
		m_ligt.Size=Size;
	}
	else
	{
		printf("LIGT��־����\n");
		m_isLigt = false;
		return false;
	}
	return true;
}

// ��ȡ��ͼEND ������
bool ReadGameMap::ReadEND (ifstream *file, uint32 Flag, uint32 Size)
{
	if (Flag==0x454E4420)
	{
		// ����Ҫ��������
	}
	else
	{
		printf("END ��־����\n");
		return false;
	}
	return true;
}


// ��ȡHEAD������
MapData ReadGameMap::GetHeadData()
{
	return m_head;
}

// ��ȡJPEG������
MapData ReadGameMap::GetJpegData()
{
	return m_jpeg;
}


// ��ȡMASK������
MapData ReadGameMap::GetMaskData(uint8 ID)
{
	return m_mask[ID];
}

// ��ȡCELL������
MapData ReadGameMap::GetCellData()
{
	return m_cell;
}

// ��ȡBRIG������
MapData ReadGameMap::GetBrigData()
{
	return m_brig;
}