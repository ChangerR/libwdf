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
#include <stdio.h>
#include <math.h>
//======= 定义数据类型 ========
typedef long long			int64;
typedef unsigned long long	uint64;
typedef int				int32;
typedef unsigned int	uint32;
typedef short			int16;
typedef unsigned short	uint16;
typedef char			int8;
typedef unsigned char	uint8;
//=============================

class BmpOffset
{
public:
	BmpOffset(uint32 BmpWidth, uint32 BmpHeight, uint32 SubWidth, uint32 SubHeight);
public:
	~BmpOffset(void);
public:
	uint32 FindOffsetBmp(uint32 unit); // 返回这个单元所对应的大地图的偏移值
	uint32 FindOffsetSub(uint32 unit); // 返回这个单元所对应的小地图的偏移值
	uint32 UnitSwitch(uint32 Unit); // 返回这个单元调整后的新位置
	uint32 GetBmpType(); // 即时返回地图的类型,即大地图是否整除小地图, 整除为0 , 不整除为1

	uint32 m_BmpWidth;
	uint32 m_BmpHeight;
	uint32 m_SubWidth;
	uint32 m_SubHeight;

	// 偏移量对应表
	struct LIST
	{
		uint32 OffsetBmp; // 大地图的偏移值
		uint32 OffsetSub; // 小地图的偏移值
	};
private:

	LIST* m_OffsetList; // 大小地图的偏移值对应表
	uint32 m_BmpType; // 地图的类型, 0为整除, 1为不整除
};
