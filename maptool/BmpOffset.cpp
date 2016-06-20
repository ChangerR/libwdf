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
#include "BmpOffset.h"

// BmpOffset(���ͼ�Ŀ��, ���ͼ�ĸ߶�, �ӵ�ͼ�Ŀ��, �ӵ�ͼ�ĸ߶�)
BmpOffset::BmpOffset(uint32 BmpWidth, uint32 BmpHeight, uint32 SubWidth, uint32 SubHeight)
{
	m_BmpWidth=BmpWidth;
	m_BmpHeight=BmpHeight;
	m_SubWidth=SubWidth;
	m_SubHeight=SubHeight;

	// ȡ���ڴ��ֵ,������С��������ֵ�Ƕ��ٶ���λ.
	uint32 row = (uint32)ceil((float)BmpHeight/(float)SubHeight); // SubBmp������
	uint32 col = (uint32)ceil((float)BmpWidth/(float)SubWidth); // SubBmp������
	uint32 bmps = row*col; // �ӵ�ͼ��������

	// ����
	uint32 Temp_row = BmpHeight/SubHeight; // ��ʱSubBmp������
	uint32 Temp_col = BmpWidth/SubWidth; // ��ʱSubBmp������
	uint32 Temp_bmps = Temp_row*Temp_col; // ��ʱ�ӵ�ͼ��������

	if (bmps != Temp_bmps)
	{
		m_BmpType = 1; // ���ͼ��������С��ͼ
	}
	else
	{
		m_BmpType = 0; // ���ͼ��������С��ͼ
	}

	//printf("BmpHeight=%d\t\tSubHeight=%d\n",BmpHeight,SubHeight);
	//printf("BmpWidth=%d\t\SubWidth=%d\n",BmpWidth,SubWidth);
	//printf("row=%d\t\tcol=%d\n",row,col);
	m_OffsetList =new LIST[bmps];

	// �����㷨~
	// �����㷨�ǽ�����3x3����Ԫ��ͼת��1x9��ͼ
	// ����֮������Ӧ��Ԫ��ʼλ�ù�ϵ
	// 123        1
	// 456    ��  2
	// 789        3
	//            4
	//            5
	//            6
	//            7
	//            8
	//            9
	//
	// ���Ϊ3x3��ͼ,�ߴ�ֱ��� 24x24 8x8,���ǹ�ϵ����
	//
	// OffsetBmp     OffsetSub
	// p1 = 0     �� p1 = 0
	// p2 = 8     �� p2 = 64
	// p3 = 16    �� p3 = 128
	// p4 = 192   �� p4 = 192
	// p5 = 200   �� p5 = 256
	// p6 = 208   �� p6 = 320
	// p7 = 384   �� p7 = 384
	// p8 = 392   �� p8 = 448
	// p9 = 400   �� p9 = 512
	//

	// ˳��: ������, ���ϵ��� �Ĺ���
	for (uint32 j = 0; j < row; j++) // ѭ��ÿ��~
	{
		for (uint32 k = 0; k < col; k++) // ѭ��ÿ��~
		{
			// ���ͼ��ƫ��ֵ = (���ͼ�Ŀ��*�ӵ�ͼ�ĸ߶�)*���� + �ӵ�ͼ�Ŀ��*����
			m_OffsetList[k+j*col].OffsetBmp = (BmpWidth*SubHeight)*j+SubWidth*k;

			// �ӵ�ͼ��ƫ��ֵ = �ӵ�ͼ�Ŀ��*�ӵ�ͼ�ĸ߶�*(����+����*������)
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
	// ��Ϊ����ĵ�ͼ�Ǵ�ֱ�ߵ��ģ�������Ҫת�����λ��
	// ת���㷨Ϊ���������䣬����������˳��
	// 123      789
	// 456  ->  456
	// 789      123
	//
	uint32 row = (uint32)ceil((float)m_BmpHeight/(float)m_SubHeight); // SubBmp������
	uint32 col = (uint32)ceil((float)m_BmpWidth/(float)m_SubWidth); // SubBmp������
	uint32 bmps = row*col;
	//printf("����=%d\t����=%d\t�ܵ�Ԫ��=%d\n",row,col,bmps);
	LIST *list1=new LIST[bmps];
	LIST *list2=new LIST[bmps];
	uint32 temp=0;

	int num=0;
	for(uint32 i= 0; i<row; i++) // ����
	{
		for(uint32 j=0; j<col; j++) // ����
		{
			// ����Ԫ���к��е�ֵ
			list1[num].OffsetBmp=i; // ��
			list1[num].OffsetSub=j; // ��

			// �㷨��Ϊ�������䣬�����������ƶ�
			list2[num].OffsetBmp=row-1-i;
			list2[num].OffsetSub=j;
			num++;
		}
	}

	// ����
	for (uint32 j=0;j<bmps;j++)
	{
		if (list1[Unit].OffsetBmp == list2[j].OffsetBmp && list1[Unit].OffsetSub == list2[j].OffsetSub)
		{
			temp = j;
		}
	}
	//printf("Unit=%d\t\ttemp=%d\n",Unit,temp);
	return temp; // ����ת����ĵ�Ԫֵ
}

