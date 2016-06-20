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
#include "stdafx.h"
#include "BmpOffset.h"

// BmpOffset(大地图的宽度, 大地图的高度, 子地图的宽度, 子地图的高度)
BmpOffset::BmpOffset(uint32 BmpWidth, uint32 BmpHeight, uint32 SubWidth, uint32 SubHeight)
{
	m_BmpWidth=BmpWidth;
	m_BmpHeight=BmpHeight;
	m_SubWidth=SubWidth;
	m_SubHeight=SubHeight;

	// 取趋于大的值,即不管小数点后面的值是多少都进位.
	uint32 row = (uint32)ceil((float)BmpHeight/(float)SubHeight); // SubBmp的行数
	uint32 col = (uint32)ceil((float)BmpWidth/(float)SubWidth); // SubBmp的列数
	uint32 bmps = row*col; // 子地图的总数量

	// 整除
	uint32 Temp_row = BmpHeight/SubHeight; // 临时SubBmp的行数
	uint32 Temp_col = BmpWidth/SubWidth; // 临时SubBmp的列数
	uint32 Temp_bmps = Temp_row*Temp_col; // 临时子地图的总数量

	if (bmps != Temp_bmps)
	{
		m_BmpType = 1; // 大地图不能整除小地图
	}
	else
	{
		m_BmpType = 0; // 大地图可以整除小地图
	}

	//printf("BmpHeight=%d\t\tSubHeight=%d\n",BmpHeight,SubHeight);
	//printf("BmpWidth=%d\t\SubWidth=%d\n",BmpWidth,SubWidth);
	//printf("row=%d\t\tcol=%d\n",row,col);
	m_OffsetList =new LIST[bmps];

	// 核心算法~
	// 以下算法是将例如3x3个单元的图转成1x9的图
	// 它们之间所对应单元开始位置关系
	// 123        1
	// 456    →  2
	// 789        3
	//            4
	//            5
	//            6
	//            7
	//            8
	//            9
	//
	// 如果为3x3的图,尺寸分别是 24x24 8x8,它们关系如下
	//
	// OffsetBmp     OffsetSub
	// p1 = 0     → p1 = 0
	// p2 = 8     → p2 = 64
	// p3 = 16    → p3 = 128
	// p4 = 192   → p4 = 192
	// p5 = 200   → p5 = 256
	// p6 = 208   → p6 = 320
	// p7 = 384   → p7 = 384
	// p8 = 392   → p8 = 448
	// p9 = 400   → p9 = 512
	//

	// 顺序: 从左到右, 从上到下 的规则
	for (uint32 j = 0; j < row; j++) // 循环每行~
	{
		for (uint32 k = 0; k < col; k++) // 循环每列~
		{
			// 大地图的偏移值 = (大地图的宽度*子地图的高度)*行数 + 子地图的宽度*列数
			m_OffsetList[k+j*col].OffsetBmp = (BmpWidth*SubHeight)*j+SubWidth*k;

			// 子地图的偏移值 = 子地图的宽度*子地图的高度*(列数+行数*行总数)
			m_OffsetList[k+j*col].OffsetSub = SubWidth*SubHeight*(k+j*col);

			//printf("OffsetBmp=%d\t\tOffsetSub=%d\n",(BmpWidth*SubHeight)*j+SubWidth*k,SubWidth*SubHeight*(k+j*col));
		}
	}
}

BmpOffset::~BmpOffset(void)
{
	delete[] m_OffsetList;
}

uint32 BmpOffset::FindOffsetBmp(uint32 unit)
{
	return m_OffsetList[unit].OffsetBmp;
}
uint32 BmpOffset::FindOffsetSub(uint32 unit)
{
	return m_OffsetList[unit].OffsetSub;
}

uint32 BmpOffset::GetBmpType()
{
	return m_BmpType;
}

uint32 BmpOffset::UnitSwitch(uint32 Unit)
{
	// 因为输出的地图是垂直颠倒的，所以需要转换这个位置
	// 转换算法为：列数不变，调整行数的顺序
	// 123      789
	// 456  ->  456
	// 789      123
	//
	uint32 row = (uint32)ceil((float)m_BmpHeight/(float)m_SubHeight); // SubBmp的行数
	uint32 col = (uint32)ceil((float)m_BmpWidth/(float)m_SubWidth); // SubBmp的列数
	uint32 bmps = row*col;
	//printf("行数=%d\t列数=%d\t总单元数=%d\n",row,col,bmps);
	LIST *list1=new LIST[bmps];
	LIST *list2=new LIST[bmps];
	uint32 temp=0;

	int num=0;
	for(uint32 i= 0; i<row; i++) // 行数
	{
		for(uint32 j=0; j<col; j++) // 列数
		{
			// 给单元赋行和列的值
			list1[num].OffsetBmp=i; // 行
			list1[num].OffsetSub=j; // 列

			// 算法即为列数不变，行数做上下移动
			list2[num].OffsetBmp=row-1-i;
			list2[num].OffsetSub=j;
			num++;
		}
	}

	// 查找
	for (uint32 j=0;j<bmps;j++)
	{
		if (list1[Unit].OffsetBmp == list2[j].OffsetBmp && list1[Unit].OffsetSub == list2[j].OffsetSub)
		{
			temp = j;
		}
	}
	//printf("Unit=%d\t\ttemp=%d\n",Unit,temp);
	return temp; // 返回转换后的单元值
}

