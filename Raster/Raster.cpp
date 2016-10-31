/********************************************************************
ʵ��������դ�񻯵Ĺ��ܣ���Ҫ��Ϊ�������֣�դ�񻯡���դ�񻯹�������ÿ�����ص�͸����ȷ����������
ʵ��ԭ�����£�

һ��դ���㷨
	***KIM�㷨��
	�������εĶ�������Ļ�ϵ������涥�㿪ʼդ�񻯣�������ơ�����ÿ�����Ttriֵ��������Ttri�Ĳ�ͬ������һ������
	1.	�����ǰ��TtriΪ00��֤����ǰ�����������ڣ����浱ǰ���ұߵ���Ϣ��RightSave�У�Ȼ��ֱ������������������
	2.	�����ǰ���TtriΪ10�����������ֱ��TtriΪ01����11��Ȼ������һ�У�
	3.	�����ǰ���TtriΪ01�����ұ�����ֱ��TtriΪ10����11��Ȼ����һ�У�
	4.	�����ǰ���TtriΪ11���������У�ֱ������һ�С�
	5.	ֱ��û�е㱻����
	Ttri�ļ��㷽�����£�
	Ttri��һ����λ�Ķ�����������4��״̬��00��01��10��11
	Ttri��״̬���ɵ�ǰ��������ε������ߣ�˳ʱ�뷽�򣩵Ĺ�ϵT�����ģ���������Tȡ���򡱵Ľ����
	T��״̬����E�������Լ���ǰ�ߵĦ�y������,�����㷨�ǣ��Ե�ǰ���ص�ͱ�0�Ĺ�ϵΪ������
	T0 = 00 if E0(x, y) >= 0 
	T0 = 01 if E0(x, y) < 0 and ��y >= 0
	T0 = 10 if E0(x, y) < 0 and ��y < 0
	***WANG�㷨
	���Ȱ���y����˳��������������Ļ��������������϶�������¶�����ɵıߣ���ʼ�ߣ���ÿ���ϵ�������Ϊÿ�еĳ�������
	�����м䶥�������ߵĹ�ϵ����ÿ�������������ұ���������м������ʼ�ߵ��ұߣ������ұ�������֮����ֱ������
	�жϳ���������ǵ�ǰ���غͳ���ʼ�������������ߵĹ�ϵ����������Ttri������ʡȥ������1/3�ļ�������
�������������ֵ
	�͡�һ���е�դ���㷨������أ�ÿ�����ص�������������ȡ�������ڵ���أ�դ�񻯹�����˳����еģ����ϵ��£������һ���ҵ���
	�����б�����
	1����Ļ���꣺				screen coord:(x, y)
	2����Ļ����������ʽ��		object coord:(x',y',w')
	3������ӳ�����
								Matrix = 
								| K  L  M |
								| N  P  Q |
								| R  S  T |
	4������֮��Ĺ�ϵΪ��(����(u, v)Ϊ��������)
							(x, y) = (x' / w', y' / w')
							| u | = | K  L  M | |x'|
							| v | = | N  P  Q | |y'|
							| 1 | = | R  S  T | |w'|
	5�����Եõ����������Ļ����֮��Ĺ�ϵ�ǣ�
					(u, v) = ((Kx + Ly + M)/(Rx + Sy + T), (Nx + Py +Q)/(Rx + Sy + T))

	Ϊ�˱�����ʽ�������صĳ�������������դ�񻯹������������ص�����ԣ���ǰ��������Щ�����������ͨ����һ�����صõ��ģ����������¼��㣩��
	���һ�����ص����������Ϊ���������ֺ�С������������
	A��������������������֣������������MidPoint Algorithm.����������ͬ��ֻ��A��E�ļ��������𣩣�
		�趨������ʽ
				�������u
						E(x, y, u) = ud(x, y) - 2*(Kx + Ly + M)
						A(u) = 2uR - 2K
						B(u) = 2uS - 2L
				�������v
						F(x, y, v) = vd(x, y) - 2*(Nx +Py +Q)
						A(v) = 2vR - 2N
						B(v) = 2vs - 2P
		ÿ�����ؼ���õ���u��Ҫ���������������d>0,�����෴��
						-d(xi, yi) < E(xi, yi, ui) <= 0   -----------------  *
		����������£�
		ÿ�����ص�d��E��F��A��B��������һ���Ѽ�������صõ���
		���*ʽ��������ǰ��u����v��������������u��v��1���߼�1��
		������ϵ���£�
			(1)���u
				��xǰ��1��ʱ��
						d(x+1, y) = d(x, y) + 2R
						E(x+1, y, u) = E(x, y, u) + A(u)
				��yǰ��1��ʱ��
						d(x, y+1) = d(x, y) + 2S
						E(x, y+1, u) = E'(x, y, u) + B'(u)
				��uǰ��1��ʱ��
						E(x, y, u+1) = E'(x, y, u) + d(x, y)
						A(u + 1) = A'(u) + 2R
						B(u + 1) = B'(u) + 2S

			(2)���v
				��xǰ��1��ʱ��
						d(x+1, y) = d(x, y ) + 2R
						F(x+1, y, v) = F(x, y, u) + A(v)
				��yǰ��1��ʱ��
						d(x, y+1) = d(x, y) + 2S
						F(x, y+1, v) = F(x, y, v) + B'(v)
				��vǰ��1��ʱ��
						F(x, y, v+1) = F'(x, y, v) + d(x, y)
						A(v + 1) = A(v) + 2R
						B(v + 1) = B(v) + 2S
	B.������������С�����֣�����uΪ������С�����ֵľ��ȿɿأ�ͨ��m����С�����λ��
		С�����ֿ��Ա�ʾΪ:1/pow(2.0, 1)?+1/pow(2.0, 1)?+1/pow(2.0, 1)?+...+1/pow(2.0, m)?
		���С�������ʾ��ǰ��һλ�Ƿ���������������������������E�Ƿ�����������
		�㷨α�������£�
					En = thisPixel->E;
					for (int i = 1; i <= m; i++)
					{
						if (En <= 0 && En > (d/pow(2.0, i)))    
							��1λ������
						else if (En <= (-thisPixel->d/pow(2.0, i)) && En > (-thisPixel->d/pow(2.0, i-1)))
							��iλ����
						En = En + thisPixel->d*kSetU[i]/pow(2.0f, i);	// ������һλ��Enֵ
					}
*********************************************************************/

#include "stdafx.h"
#include "Raster.h"
#include <math.h>
#include "Matrix.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <Windows.h>
#include <fstream>
using namespace std;

#define  UNIT 1

//////////////////////////////////////////////////////////////////////////
// ����һ�����������������Ľ��
int round(double f)
{
	if (f < 0)
		return int(f-0.5);
	else
		return int(f+0.5);
}


//////////////////////////////////////////////////////////////////////////
// ���Ÿ��ⲿ���õ�������դ���Լ�����ӳ��ĺ���
// @GLdouble triangleVertex[9]�������������������������
// @double vertexTexCoord[6]:���������Ӧ����������ֵ
// @m:�����������ľ���
// @tex:��������ָ��
// @rasterType:դ�񻯷�ʽ�������֣�һ�������������Z���ͣ�����һ��������ʦ�����
double CRaster::RasterCalTex(GLdouble triangleVertex[9], double vertexTexCoord[6], int m, double view[9], Texture *tex
							 , RASTERTYPE rasterType, bool bPerPixelDiv, CALTEXCOORDTYPE texType, bool bLinearReduceError, int nRasterLine)
{

	// ɾ�����е���������
	Release();
	// ��¼դ������ʱ��
	LONGLONG   t1 = 0,t2 = 0;   
	LONGLONG   persecond = 0;   
	QueryPerformanceFrequency((LARGE_INTEGER   *)&persecond);//ѯ��ϵͳһ���ӵ�Ƶ��   
	double perms = persecond / 1000.0;
	QueryPerformanceCounter((LARGE_INTEGER   *)&t1);   
	// ���������������������Ļ����
	GLint   triangleScreenVertex[6];
	GetScreenCoord(triangleVertex, triangleScreenVertex);
	GetScreenCoord(triangleVertex+3, triangleScreenVertex+2);
	GetScreenCoord(triangleVertex+6, triangleScreenVertex+4);

	// �������ӳ�����
	double texMatrix[9];	
	double realTexCoord[6];
	if (tex != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			realTexCoord[i*2] = vertexTexCoord[i*2] * (tex->width-1);
			realTexCoord[i*2+1] = vertexTexCoord[i*2+1] * (tex->width-1);

		}	
		CalcTexMatrix(triangleVertex, realTexCoord, texMatrix);
	}

	// �Գ�Ա�������в����������ٵ��ú���ʱ�Ĳ�������
	InitParams(triangleScreenVertex, texMatrix, m, rasterType, bPerPixelDiv, texType);
//////////////////////////////////////////////////////////////////////////
	// �����WANGģʽ�õ���������Ļ�����Ҫ�������������������������
	if (texType == WANGTEX)
	{
		m_pWang = new WangTexData;
		m_pWang->bLinearReduceError = bLinearReduceError;
		m_pWang->nRasterLine = nRasterLine;
		// �õ��ڹ۲�����ϵ������������ƽ���xyƽ�潻�ߵ�б�ʣ��õ�б���Ժ���midpoint�㷨��ø�б�ʵ�ֱ��դ�񻯺�Ľ����
		m_pWang->pLinePattern = GetLinePattern(triangleVertex, view, m_pWang->nPatternNum, m_pWang->fGradient);
		// �Ը�б��Ϊɨ��ģʽ�����ɨ�������Ρ�����ÿ��б��ɨ���е������Ϣ��������������������ֵ������
		m_pWang->pBound = CalcBoundBoxData();
		LinearError(m_pWang);
	}

	// ����������դ���㷨����Ļ�����µ������ν���դ��
	// ��դ�񻯵Ĺ��̵��������������������������㷽������ÿ�����ص����������ֵ
	TriangleRasterization();

	QueryPerformanceCounter((LARGE_INTEGER   *)&t2);   
	double   time=double(t2-t1)/perms;			// ���դ������ʱ��

	// ���õõ�����������������յ�ÿ��������ɫֵ
	if (tex != NULL)
	{
		GetTexData(m_pRasteredPoints, tex);
	}

	return time;
}

void CRaster::InitParams(int screenV[6],double texMatrix[9],int m, RASTERTYPE rType, bool bPerPixelDiv, CALTEXCOORDTYPE texType)
{
	memcpy(m_pScreenV, screenV, 6*sizeof(int));
	// ������������Ϊ˳ʱ��˳��
	ClockWizeTriangle(m_pScreenV, m_pScreenV+2, m_pScreenV+4);

	memcpy(m_texMatrix, texMatrix, 9*sizeof(double));
	m_mBit = m;
	m_rType = rType;
	m_bPerPixelDiv = bPerPixelDiv;
	m_tType = texType;
}

//-----------------------------������դ����غ���----------------------------------//

//////////////////////////////////////////////////////////////////////////
// ���㺯��E(x, y) = (x - x0) �� (y1 - y0) - (y - y0) �� (x1 - x0)
// E(x,y) > 0 :��(x, y) �������(V0, V1)���ұ�
// E(x,y) < 0 :��(x, y) �������(V0, V1)�����
// E(x,y) = 0 :��(x, y) �������(V0, V1)����
// @vx, vy:���ж��ĵ�
// @v0x,v0y,v1x,v1y:�߶ε������յ�
inline int CRaster::E(int vx, int vy,int v0x,int v0y,int v1x,int v1y)
{
	return ((vx - v0x) * (v1y - v0y) - (vy - v0y) * (v1x - v0x));
}

//////////////////////////////////////////////////////////////////////////
// �������µķ�������һ�����Ttri
// Ttri��һ����λ�Ķ�����������4��״̬��00��01��10��11
// Ttri��״̬���ɵ�ǰ��������ε������ߣ�˳ʱ�뷽�򣩵Ĺ�ϵT�����ģ���������Tȡ���򡱵Ľ����
// T��״̬����E�������Լ���ǰ�ߵĦ�y������,�����㷨�ǣ�
// T0 = 00 if E0(x, y) >= 0 
// T0 = 01 if E0(x, y) < 0 and ��y >= 0
// T0 = 10 if E0(x, y) < 0 and ��y < 0
// ��������T���㷨��������㷨һ��
inline int CRaster::GetTtri(int vx, int vy, int v1x, int v1y,int v2x,int v2y,int v3x,int v3y)
{
	// ��õ�ǰ���ص�������ߵı��룬Ȼ���󡰻򡱣��õ����յı��벢����
	int Ttri = 0;
	int nE = E(vx, vy, v1x, v1y, v2x, v2y);
	int nDeltY = (v2y - v1y);
	if (nE >= 0)
		Ttri = 0x00;
	else if(nE < 0 && nDeltY >= 0)
		Ttri = 0x01;
	else if(nE < 0 && nDeltY < 0)
		Ttri = 0x02;

	nE = E(vx, vy, v2x, v2y, v3x, v3y);
	nDeltY = (v3y - v2y);
	if (nE >= 0)   
		Ttri |= 0x00;
	else if(nE < 0 && nDeltY >= 0)
		Ttri |= 0x01;
	else if(nE < 0 && nDeltY < 0)
		Ttri |= 0x02;

	nE = E(vx, vy, v3x, v3y, v1x, v1y);
	nDeltY = (v1y - v3y);
	if (nE >= 0)
		Ttri |= 0x00;
	else if(nE < 0 && nDeltY >= 0)
		Ttri |= 0x01;
	else if(nE < 0 && nDeltY < 0)
		Ttri |= 0x02;
	return Ttri;
}
//////////////////////////////////////////////////////////////////////////
// դ���㷨�ĵݹ麯��
// ԭ��:�жϵ�ǰ���ص�ı���,����������ǰ��������εĹ�ϵ:
// 1 �����ǰ��TtriΪ00��֤����ǰ�����������ڣ���¼��ǰ��.���Ű������еı�������������õ�ǰ����������һ�����ص�
// 2 �����ǰ���TtriΪ10����ǰ�㲻����������,�������øú������������ֱ��TtriΪ01����11��Ȼ�������
// 3 �����ǰ���TtriΪ01����ǰ�㲻����������,�������øú������ұ�����ֱ��TtriΪ10����11��Ȼ�������
// �ڱ����Ĺ�����,����CalcTexCoord�����Լ����ڵ����Ϣ��õ�ǰ�����������ֵ
// WANGդ��ģʽ�£��������ֻ�жϵ�ǰ���غ�������������ʼ�ߵĹ�ϵ

// @dir����ǰ���ص��ǰһ�����λ�ù�ϵ
// @unit�����ش�С��һ��Ϊ1��
// @pPrePixel��ǰһ�����ص����Ϣ
// @allPixels��դ�������ص��б�ָ�롣��ǰ���ڴ˺����н����ص���Ϣ��ʼ����Ȼ���������б���
// @x, y����ǰ���ص������ֵ
// @v1x, 1y,v2x,v2y,v3x,v3y�������εĶ�������ֵ������������롣���밴��˳ָ��˳����
// @texMatrix[9]������ӳ�����
// @m���������꾫�ȿ���
// @RASTERTYPE rType:դ�����ͣ�Ĭ��ΪKIM
// @double *sortedV�����մ��ϵ��µ�˳�����кõ������������꣬��WANGģʽ��ʹ�ã������ҵ�����ʼ��
void CRaster::Rasterization(DIR dir, int unit, RasteredPoints *pPrePixel, RasteredPoints *allPixels, int x, int y, int *sortedV)
{
	int v1x = m_pScreenV[0];
	int v1y = m_pScreenV[1];
	int v2x = m_pScreenV[2];
	int v2y = m_pScreenV[3];
	int v3x = m_pScreenV[4];
	int v3y = m_pScreenV[5];
	int fE1, fE2;
	int Ttri;
	// �����ǰ��WANGģʽ�������Ѿ��ҵ�ÿ�е���ʼ��Ļ������㵱ǰ�����������ʼ�ߵ�λ�ù�ϵ
	if (allPixels->firstValidate != NULL && m_rType == WANG)
	{
		fE1 = E(x, y, sortedV[0], sortedV[1], sortedV[2], sortedV[3]);
		fE2 = E(x, y, sortedV[2], sortedV[3], sortedV[4], sortedV[5]);
	}
	else
	{
		// ���㵱ǰդ���ı���
		Ttri = GetTtri(x, y, v1x, v1y, v2x, v2y, v3x, v3y);
	}

	// �½���ǰդ���Ľṹ��,���������դ����б���
	RasteredPoints * newPoint = new RasteredPoints;

	newPoint->x = x;
	newPoint->y = y;
	newPoint->next = NULL;
	newPoint->bIsInside = false;
	allPixels->last->next = newPoint;
	allPixels->last = newPoint;

	// ������һ������դ�����㵱ǰդ������������ֵ
	if (m_tType == KIMTEX)
		CalcTexCoord(m_texMatrix, newPoint, pPrePixel, dir, m_mBit, m_bPerPixelDiv);

	if (dir == LEFT)
	{
		if (allPixels->firstValidate == NULL || m_rType == KIM)		// �����KIMģʽ������WANGģʽ��û���ҵ���ʼ���صĻ������ձ����жϵ�ǰ���ص�����
		{
			if (Ttri == 0x01 || Ttri == 0x03)
				// ��¼���е�һ����Ч��
			{
				allPixels->firstInvalidate = newPoint;
				return;
			}
			else if (Ttri == 0x02)
				// �����������
			{
				Rasterization(dir, unit, newPoint, allPixels, x-unit, y ,sortedV);
			}
			else if (Ttri == 0x00)
				// �������������¼��ǰ��
			{
				if (allPixels->firstValidate == NULL)
					allPixels->firstValidate = newPoint;

				newPoint->bIsInside = true;

				if (m_tType == WANGTEX)
					CalcTexCoordWANG(newPoint);

				Rasterization(dir, unit, newPoint,allPixels,x-unit, y ,sortedV);	
			}
			else
				return;
		}
		else														// ��WANGģʽ�°��յ�ͷ���ʼ�ߵĹ�ϵ�жϵ�ǰ���ص��λ��
		{
			// E(x,y) > 0 :��(x, y) �������(V0, V1)���ұ�
			// E(x,y) < 0 :��(x, y) �������(V0, V1)�����
			// E(x,y) = 0 :��(x, y) �������(V0, V1)����
			if (fE1 <= 0 && fE2 <= 0)							// ��ǰ���ص�����������
			{
				newPoint->bIsInside = true;

				if (m_tType == WANGTEX)
					CalcTexCoordWANG( newPoint);

				Rasterization(dir, unit, newPoint,allPixels,x-unit, y ,sortedV);	
			}
			else													// ��ǰ���ص㲻����������
			{
				//allPixels->firstInvalidate = newPoint;
				return;
			}
		}
	}
	else if (dir == RIGHT)
	{
		if (allPixels->firstValidate == NULL || m_rType == KIM)
		{
			if (Ttri == 0x02 || Ttri == 0x03)
				// ��¼���е�һ����Ч��
			{
				allPixels->firstInvalidate = newPoint;
				return;
			}
			else if (Ttri == 0x01)
				// �������ұ���
			{
				Rasterization(dir, unit, newPoint, allPixels,x+unit, y,sortedV);
			}
			else if (Ttri == 0x00)
				// �������������¼��ǰ��
			{
				if (allPixels->firstValidate == NULL)
					allPixels->firstValidate = newPoint;

				newPoint->bIsInside = true;

				if (m_tType == WANGTEX)
					CalcTexCoordWANG(newPoint);

				Rasterization(dir, unit, newPoint, allPixels,x+unit, y ,sortedV);	
			}
			else
				return;
		}
		else
		{
			// E(x,y) > 0 :��(x, y) �������(V0, V1)���ұ�
			// E(x,y) < 0 :��(x, y) �������(V0, V1)�����
			// E(x,y) = 0 :��(x, y) �������(V0, V1)����
			if (fE1 >= 0 && fE2 >= 0)
			{
				newPoint->bIsInside = true;

				if (m_tType == WANGTEX)
					CalcTexCoordWANG(newPoint);

				Rasterization(dir, unit, newPoint,allPixels,x+unit, y,sortedV);	
			}
			else
			{
				//allPixels->firstInvalidate = newPoint;
				return;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ���ϵ������������ε���������
// @GLdouble v1x, GLdouble v1y,GLdouble v2x,GLdouble v2y,GLdouble v3x,GLdouble v3y:�������������������
// @GLdouble sortedV[6]:��������ϵ��µ�˳��
void CRaster::SortVertex(int v1x, int v1y,int v2x,int v2y,int v3x,int v3y, int sortedV[6])
{
	sortedV[1] = v1y;
	sortedV[0] = v1x;

	sortedV[3] = v2y;
	sortedV[2] = v2x;

	sortedV[5] = v3y;
	sortedV[4] = v3x;

	int tmpX, tmpY;
	for (int i = 0; i < 3; i++)
	{
		for (int j = i+1; j < 3; j++)
		{
			if (sortedV[2*i+1] < sortedV[2*j+1])
			{
				tmpX = sortedV[2*i];
				tmpY = sortedV[2*i+1];
				sortedV[2*i] = sortedV[2*j];
				sortedV[2*i+1] = sortedV[2*j+1];
				sortedV[2*j] = tmpX;
				sortedV[2*j+1] = tmpY;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// ������դ�񻯺�����ͨ������Rasterization������ɹ���
// ***KIMդ�񻯹��̣�
// ��������Ķ��㿪ʼ,��㰴���㷨��������,�ҵ��������ڵ����ص�,�����������.���е�ʱ��ͬʱ������һ�ж�Ӧ���ص���������
// 1 �����ǰ��TtriΪ00��֤����ǰ�����������ڣ���Ҫ�����������������ߣ�ֱ��Ttri��Ϊ11���ұߵ�ͷ�Ժ�����һ�У�
// 2 �����ǰ���TtriΪ10�����������ֱ��TtriΪ01����11��Ȼ������һ�У�
// 3 �����ǰ���TtriΪ01�����ұ�����ֱ��TtriΪ10����11��Ȼ����һ�У�
// 4 �����ǰ���TtriΪ11������դ����̣����е�TtriΪ11ʱ��ֱ������һ�У������������ǣ���û�е���ƣ�
// ***WANGդ�񻯹��̣�
// ���Ȱ���y����˳��������������Ļ��������������϶�������¶�����ɵıߣ���ʼ�ߣ���ÿ���ϵ�������Ϊÿ�еĳ�������
// �����м䶥�������ߵĹ�ϵ����ÿ�������������ұ���������м������ʼ�ߵ��ұߣ������ұ�������֮����ֱ������
// �жϳ���������ǵ�ǰ���غͳ���ʼ�������������ߵĹ�ϵ����������Ttri������ʡȥ������1/3�ļ�������

// @GLdouble v1x, GLdouble v1y,GLdouble v2x,GLdouble v2y,GLdouble v3x,GLdouble v3y:�����ε������������ꡣ���밴��˳ָ��˳����
// @texMatrix[9]������ӳ�����
// @m���������꾫�ȿ���
void CRaster::TriangleRasterization()
{	
	int sortedV[6];
	SortVertex(m_pScreenV[0], m_pScreenV[1], m_pScreenV[2], m_pScreenV[3], m_pScreenV[4], m_pScreenV[5], sortedV);	
	int fTopx = sortedV[0];
	int fTopy = sortedV[1];
	int fMidx = sortedV[2];
	int fMidy = sortedV[3];
	int fBotx = sortedV[4];
	int fBoty = sortedV[5];

	DIR Vdir = DOWN, Hdir = LEFT;		// դ�񻯹��̵�����ͺ����ƶ�����
	if (m_rType == WANG)					// �����WANGģʽ�Ļ�����Ҫ���շָ��ߵ�б���Լ��м䶥������ڷָ��ߵ�λ�þ���դ��˳��
	{
		double fE = E(fMidx, fMidy, fBotx, fBoty, fTopx, fTopy);
		// E(x,y) > 0 :��(x, y) �������(V0, V1)���ұ�
		// E(x,y) < 0 :��(x, y) �������(V0, V1)�����
		// E(x,y) = 0 :��(x, y) �������(V0, V1)����
		if (fE > 0)	
			Hdir = RIGHT;
		else if (fE < 0)
			Hdir = LEFT;
		else
			return;

		if (fTopx < fBotx)
		{
			if (LEFT == Hdir)
				Vdir = UP;
			else
				Vdir = DOWN;
		}
		else
		{
			if (LEFT == Hdir)
				Vdir = DOWN;
			else
				Vdir = UP;
		}	
	}									// �����KIMģʽ�Ļ����ҵ�������Ķ��㣬�������µ�˳��դ��
	else if (m_rType == KIM)
		Vdir = DOWN;

	RasteredPoints *RPoint = new RasteredPoints;

	// ��ʼ�����������к��������������صı���ֵ
	double K, L, M, N, P, Q, R, S, T;
	if (m_texMatrix !=NULL)
	{
		K = m_texMatrix[0];
		L = m_texMatrix[1];
		M = m_texMatrix[2];
		N = m_texMatrix[3];
		P = m_texMatrix[4];
		Q = m_texMatrix[5];
		R = m_texMatrix[6];
		S = m_texMatrix[7];
		T = m_texMatrix[8];
	}

	if (m_rType == KIM)
	{
		RPoint->x = fTopx;
		RPoint->y = fTopy;
	}
	else if (m_rType == WANG)
	{
		if (Vdir == DOWN)
		{
			RPoint->x = fTopx;
			RPoint->y = fTopy;
		}
		else
		{
			RPoint->x = fBotx;
			RPoint->y = fBoty;
		}
	}
	RPoint->bIsInside = true;			// ��ǵ�ǰ��Ϊ��������,��դ���

	RPoint->u = (K*RPoint->x + L*RPoint->y + M)/(R*RPoint->x + S*RPoint->y + T);
	RPoint->v = (N*RPoint->x + P*RPoint->y + Q)/(R*RPoint->x + S*RPoint->y + T);

	double u = floor(RPoint->u);
	double v = floor(RPoint->v);

	RPoint->d = 2*(R*RPoint->x + S*RPoint->y + T);

	RPoint->Au = 2*u*R - 2*K;
	RPoint->Bu = 2*u*S - 2*L;
	RPoint->E = u*RPoint->d - 2*(K*RPoint->x + L*RPoint->y + M);

	RPoint->Av = 2*v*R - 2*N;
	RPoint->Bv = 2*v*S - 2*P;
	RPoint->F = v*RPoint->d - 2*(N*RPoint->x + P*RPoint->y + Q);

	// ��������ص����դ����б���
	RPoint->next = NULL;
	RPoint->firstInvalidate = RPoint;
	m_pRasteredPoints = RPoint;
	m_pRasteredPoints->last = RPoint;

	RasteredPoints *pPreRowPixel = RPoint;

	int Ttri = 0;
	int bRastered = true;

	do
	{
		// ��ʼ��һ���µ�դ���,������һ��դ������һ��,ͬһ��
		RasteredPoints *pCurPoint = new RasteredPoints;

		pCurPoint->next = NULL;
		pCurPoint->bIsInside = false;
		m_pRasteredPoints->firstValidate = NULL;
		// ���´�����դ������դ����б���
		m_pRasteredPoints->last->next = pCurPoint;
		m_pRasteredPoints->last = pCurPoint;

		pCurPoint->x = pPreRowPixel->x;
		if (Vdir == DOWN)
			pCurPoint->y = pPreRowPixel->y - UNIT;
		else 
			pCurPoint->y = pPreRowPixel->y + UNIT;

		if (m_tType == KIMTEX)
			// ������һ�е�դ�������½�դ������������ֵ
			CalcTexCoord(m_texMatrix, pCurPoint, pPreRowPixel, Vdir, m_mBit, m_bPerPixelDiv);

		// ����½���դ���ı���
		Ttri = GetTtri(pCurPoint->x, pCurPoint->y, m_pScreenV[0], m_pScreenV[1], m_pScreenV[2], m_pScreenV[3], m_pScreenV[4], m_pScreenV[5]);

		if (m_rType == KIM)			// KIMģʽ����դ��
		{
			if (Ttri == 0x00)
				// ��ǰ�����������ڣ���¼��ǰ�㣬Ȼ���������ұ�����ֱ������������
			{
				pCurPoint->bIsInside = true;
				if (m_tType == WANGTEX)
					CalcTexCoordWANG(pCurPoint);

				Rasterization(LEFT, UNIT, pCurPoint, m_pRasteredPoints, pCurPoint->x - UNIT, pCurPoint->y, sortedV);
				Rasterization(RIGHT, UNIT, pCurPoint, m_pRasteredPoints, pCurPoint->x + UNIT, pCurPoint->y,sortedV);
			}
			else if (Ttri == 0x01)
				// ��ǰ������������ߣ���Ҫ���ұ���
			{
				Rasterization(RIGHT, UNIT, pCurPoint, m_pRasteredPoints,pCurPoint->x + UNIT, pCurPoint->y, sortedV);
			}

			else if(Ttri == 0x02)
				// ��ǰ�����������ұߣ���Ҫ�������
			{
				Rasterization(LEFT, UNIT, pCurPoint, m_pRasteredPoints,pCurPoint->x - UNIT, pCurPoint->y, sortedV);
			}
			else if (Ttri == 0x03 )
				// ��ǰ���Ѿ��������������½ǣ�����
			{
				bRastered = false;
				continue;
			}
			if (pCurPoint->y < fBoty)	// ��ֹ��ѭ�����п��ܵ�ǰ�������������½�
			{
				bRastered = false;
				continue;
			}
			// ����һ�У�ͬʱ��������һ�����ص�����������صĲ���
			pPreRowPixel = m_pRasteredPoints->firstInvalidate;
		}
		else if (m_rType == WANG)		// WANGģʽ����դ��
		{
			if (Ttri == 0x00)		// �ƶ�һ�к�����ص�����������
			{
				pCurPoint->bIsInside = true;
				if (m_tType == WANGTEX)
					CalcTexCoordWANG(pCurPoint);

				m_pRasteredPoints->firstValidate = pCurPoint;
			}	
			else if (Ttri == 0x03 )	// �Ѿ�û�����ص㴦����������
				// ��ǰ���Ѿ��������������½ǣ�����
			{
				bRastered = false;
				continue;
			}
			// ����դ�񻯷��������һ�����x����
			int nextX = (Hdir == LEFT)? (pCurPoint->x - UNIT):(pCurPoint->x + UNIT);
			// ����ѭ����դ�񻯱��е�����
			Rasterization(Hdir, UNIT, pCurPoint, m_pRasteredPoints, nextX, pCurPoint->y, sortedV);
			// ȡ����һ�д����������ڵĵ�һ������
			pPreRowPixel = m_pRasteredPoints->firstValidate;

			// �������û�����ش����������ڣ���դ�����
			if (pPreRowPixel == NULL)
			{
				if(pCurPoint->y < fBoty || pCurPoint->y > fTopy)
				{
					bRastered = false;
					continue;
				}
				pPreRowPixel = pCurPoint;
			}
		}
	}while(bRastered);
}

//-----------------------����ӳ����غ���---------------------//

//////////////////////////////////////////////////////////////////////////
// ����������⺯��
// �����������ص����Ϣ������ȡ��ǰ���ص����������ֵ��
// ʵ��ԭ��(��Ϊ��ȡ���������������ֺ�С������):
// ��������:
// d(x, y) = 2(Rx + Sy + T)
// -d(xi, yi) < E(xi, yi, ui) <= 0
// (1)���u
// E'(x, y, u) = ud(x, y) - 2*(Kx + Ly + M)
// A'(u) = 2uR - 2K
// B'(u) = 2uS - 2L
// ��xǰ��1��ʱ��
// d(x+1, y) = d(x, y) + 2R
// E'(x+1, y, u) = E'(x, y, u) + A'(u)
// ��yǰ��1��ʱ��
// d(x, y+1) = d(x, y) + 2S
// E'(x, y+1, u) = E'(x, y, u) + B'(u)
// ��uǰ��1��ʱ��
// E'(x, y, u+1) = E'(x, y, u) + d(x, y)
// A'(u + 1) = A'(u) + 2R
// B'(u + 1) = B'(u) + 2S
// (2)���v
// F'(x, y, v) = vd(x, y) - 2*(Nx +Py +Q)
// A'(v) = 2vR - 2N
// B'(v) = 2vs - 2P
// ��xǰ��1��ʱ��
// d(x+1, y) = d(x, y ) + 2R
// F'(x+1, y, v) = F'(x, y, u) + A'(v)
// ��yǰ��1��ʱ��
// d(x, y+1) = d(x, y) + 2S
// F'(x, y+1, v) = F'(x, y, v) + B'(v)
// ��vǰ��1��ʱ��
// F'(x, y, v+1) = F'(x, y, v) + d(x, y)
// A'(v + 1) = A'(v) + 2R
// B'(v + 1) = B'(v) + 2S
// С������:
// �㷨α�������£�
// En = thisPixel->E;
// for (int i = 1; i <= m; i++)
// {
// 		if (En <= 0 && En > (d/pow(2.0, i)))    
// 			��1λ������
// 		else if (En <= (-thisPixel->d/pow(2.0, i)) && En > (-thisPixel->d/pow(2.0, i-1)))
// 			��iλ����
// 		En = En + thisPixel->d*kSetU[i]/pow(2.0f, i);	// ������һλ��Enֵ
// }

// @texMatrix[9]:����
// @thisPixel,prePixel:��ǰ���ǰһ�������Ϣ
// @moveDir:��ǰ�����ǰһ������ƶ�����
// @m:�����������ľ���
inline void CRaster::CalcTexCoord(double texMatrix[9], RasteredPoints *thisPixel, RasteredPoints *prePixel, DIR moveDir, int m, bool bPerPixelDiv)
{
	if (texMatrix == NULL)
		return;

	double K, L, M, N, P, Q, R, S, T;
	K = texMatrix[0];
	L = texMatrix[1];
	M = texMatrix[2];
	N = texMatrix[3];
	P = texMatrix[4];
	Q = texMatrix[5];
	R = texMatrix[6];
	S = texMatrix[7];
	T = texMatrix[8];
	if (bPerPixelDiv)
	{
		double fTmp = 1.0/(R*thisPixel->x + S*thisPixel->y + T);
		thisPixel->u = (K*thisPixel->x + L*thisPixel->y + M)*fTmp;
		thisPixel->v = (N*thisPixel->x + P*thisPixel->y + Q)*fTmp;
		return;
	}
	// ���ô�����������س�ʼ����ǰ���ص���Ϣ
	bool bFoundU = false;
	bool bFoundV = false;

	thisPixel->u = floor(prePixel->u);	// ������������
	thisPixel->v = floor(prePixel->v);
	
	thisPixel->Au = prePixel->Au;
	thisPixel->Bu = prePixel->Bu;
	thisPixel->Av = prePixel->Av;
	thisPixel->Bv = prePixel->Bv;
	thisPixel->d = prePixel->d ;
	thisPixel->E = prePixel->E;
	thisPixel->F = prePixel->F;

	// ���ô�����������غ͵�ǰ���ص�λ�ù�ϵ���ĳЩ��Ϣ��ֵ����Щ��Ϣ����Ҫ���¼��㣬ֻ��Ҫ���ӷ����ɣ�
	switch(moveDir)
	{
	case RIGHT:
		thisPixel->d = prePixel->d + 2*R;
		thisPixel->E = prePixel->E + thisPixel->Au;
		thisPixel->F = prePixel->F + thisPixel->Av;
		break;
	case  LEFT:
		thisPixel->d = prePixel->d - 2*R;
		thisPixel->E = prePixel->E - thisPixel->Au;
		thisPixel->F = prePixel->F - thisPixel->Av;
		break;
	case UP:
		thisPixel->d = prePixel->d + 2*S;
		thisPixel->E = prePixel->E + thisPixel->Bu;
		thisPixel->F = prePixel->F + thisPixel->Bv;
		break;
	case DOWN:
		thisPixel->d = prePixel->d - 2*S;
		thisPixel->E = prePixel->E - thisPixel->Bu;
		thisPixel->F = prePixel->F - thisPixel->Bv;
		break;
	default:
		break;
	}

	// ������������������������
	if (thisPixel->d > 0)		// ���d����0�Ļ�
	{
		while (!bFoundU || !bFoundV)
		{
			if (!bFoundU)
			{
				if (thisPixel->E > -thisPixel->d&& thisPixel->E <= 0)
					bFoundU = true;
				else if (thisPixel->E <= -thisPixel->d)
				{
					thisPixel->u += 1;

					thisPixel->E = thisPixel->E + thisPixel->d;
					thisPixel->Au = thisPixel->Au + 2*R;
					thisPixel->Bu = thisPixel->Bu + 2*S;
				}
				else if (thisPixel->E > 0)
				{
					thisPixel->u -= 1;

					thisPixel->E = thisPixel->E - thisPixel->d;
					thisPixel->Au = thisPixel->Au - 2*R;
					thisPixel->Bu = thisPixel->Bu - 2*S;
				}
			}
			if (!bFoundV)
			{
				if (thisPixel->F > -thisPixel->d && thisPixel->F <= 0)
					bFoundV = true;
				else if (thisPixel->F <= -thisPixel->d)
				{
					thisPixel->v += 1;

					thisPixel->F = thisPixel->F + thisPixel->d;
					thisPixel->Av = thisPixel->Av + 2*R;
					thisPixel->Bv = thisPixel->Bv + 2*S;
				}
				else if (thisPixel->F > 0)
				{
					thisPixel->v -= 1;

					thisPixel->F = thisPixel->F - thisPixel->d;
					thisPixel->Av = thisPixel->Av - 2*R;
					thisPixel->Bv = thisPixel->Bv - 2*S;
				}
			}
		}	
	}
	else					// ���dС��0
	{
		while (!bFoundU || !bFoundV)
		{
			if (!bFoundU)
			{
				if (thisPixel->E < -thisPixel->d && thisPixel->E >= 0)
					bFoundU = true;
				else if (thisPixel->E >= -thisPixel->d)
				{
					thisPixel->u += 1;

					thisPixel->E = thisPixel->E + thisPixel->d;
					thisPixel->Au = thisPixel->Au + 2*R;
					thisPixel->Bu = thisPixel->Bu + 2*S;
				}
				else if (thisPixel->E < 0)
				{
					thisPixel->u -= 1;

					thisPixel->E = thisPixel->E - thisPixel->d;
					thisPixel->Au = thisPixel->Au - 2*R;
					thisPixel->Bu = thisPixel->Bu - 2*S;
				}
			}
			if (!bFoundV)
			{
				if (thisPixel->F <-thisPixel->d && thisPixel->F >= 0)
					bFoundV = true;
				else if (thisPixel->F >= -thisPixel->d)
				{
					thisPixel->v += 1;

					thisPixel->F = thisPixel->F + thisPixel->d;
					thisPixel->Av = thisPixel->Av + 2*R;
					thisPixel->Bv = thisPixel->Bv + 2*S;
				}
				else if (thisPixel->F < 0)
				{
					thisPixel->v -= 1;

					thisPixel->F = thisPixel->F - thisPixel->d;
					thisPixel->Av = thisPixel->Av - 2*R;
					thisPixel->Bv = thisPixel->Bv - 2*S;
				}
			}
		}	
	}


	// ����������������С�����֣�������m
	double uFraction = 0.0;
	double vFraction = 0.0;
	double En = thisPixel->E;
	double Fn = thisPixel->F;
	int kSetU = 0;
	int kSetV = 0;

	// ����ʹ�ó�������
	double fFactor = 0.5;
	double fFactorUp = 1;
	double fFactorDown = 1;
	if (thisPixel->d > 0)
	{
		for (int i = 1; i <= m; i++)
		{
			fFactorUp *= fFactor;

			// ����u
			if (En <= 0 && En > (-thisPixel->d*fFactorUp))
				kSetU = 0;
			else if (En <= (-thisPixel->d*fFactorUp) && En > (-thisPixel->d*fFactorDown))
				kSetU = 1;
			else					// �������
				kSetU = 0;
			En = En + thisPixel->d*kSetU*fFactorUp;		// ����i�����µ�E
			uFraction += kSetU*fFactorUp;
			// ����v
			if (Fn <= 0 && Fn > (-thisPixel->d*fFactorUp))
				kSetV = 0;
			else if (Fn <= (-thisPixel->d*fFactorUp) && Fn > (-thisPixel->d*fFactorDown))
				kSetV = 1;
			else					// �������
				kSetV = 0;
			Fn = Fn + thisPixel->d*kSetV*fFactorUp;
			vFraction += kSetV*fFactorUp;

			fFactorDown *= fFactor;
		}
	}
	else
	{
		for (int i = 1; i <= m; i++)
		{
			fFactorUp *= fFactor;

			// ����u
			if (En >= 0 && En < (-thisPixel->d*fFactorUp))
				kSetU = 0;
			else if (En >= (-thisPixel->d*fFactorUp) && En < (-thisPixel->d*fFactorDown))
				kSetU = 1;
			else					// �������
				kSetU = 0;
			En = En + thisPixel->d*kSetU*fFactorUp;
			uFraction += kSetU*fFactorUp;
			// ����v
			if (Fn >= 0 && Fn < (-thisPixel->d*fFactorUp))
				kSetV = 0;
			else if (Fn >= (-thisPixel->d*fFactorUp) && Fn < (-thisPixel->d*fFactorDown))
				kSetV = 1;
			else					// �������
				kSetV = 0;
			Fn = Fn + thisPixel->d*kSetV*fFactorUp;
			vFraction += kSetV*fFactorUp;

			fFactorDown *= fFactor;

		}
	}
	// �õ��������������ֵ
	thisPixel->u += uFraction; 
	thisPixel->v += vFraction;
}

//////////////////////////////////////////////////////////////////////////
// ��������ӳ�����M
// ������Ļ���꣨x, y�����õ�ͬ������(x', y', w')��Ȼ�����ø�������������������õ�����ӳ�����
	// (x, y) = (x' / w', y' / w')

	// | u | = | K  L  M | |x'|
	// | v | = | N  P  Q | |y'|
	// | 1 | = | R  S  T | |w'|
	// ���������ζ��㴦����������Ϊ��֪�����Կ�����þ������Ԫ�ص�ֵ
	// | u0 u1 u2| = | K  L  M | |x0 x1 x2|
	// | v0 v1 v2| = | N  P  Q | |y0 y1 y2|
	// | 1  1  1 | = | R  S  T | |w0 w1 w2|
	// | u0 u1 u2| |x0 x1 x2|-1	= | K  L  M |
	// | v0 v1 v2|*|y0 y1 y2|	= | N  P  Q |
	// | 1  1  1 | |w0 w1 w2|	= | R  S  T |
void CRaster::CalcTexMatrix(GLdouble triangleVertex[9], double vertexTexCoord[6], double texMatrix[9])
{
	//|x0 x1 x2|T
	//|y0 y1 y2|
	//|w0 w1 w2|
	double homoVertex[9];
	for (int i = 0; i < 3; i++)
	{
		int x, y;
		double w;
		CustomProject/*ViewProject*/(triangleVertex+i*3, x, y, w);
		homoVertex[i*3] = x*w;
		homoVertex[i*3+1] = y*w;
		homoVertex[i*3+2] = w;
	}	
	CMatrix vertMat(3, 3, homoVertex);

	//| u0 u1 u2|T
	//| v0 v1 v2|
	//| 1  1  1 |
	double homoTex[9];
	for (int i = 0; i < 3; i++)
	{
		homoTex[i*3]   = vertexTexCoord[i*2];
		homoTex[i*3+1] = vertexTexCoord[i*2+1];
		homoTex[i*3+2] = 1.0;

	}
	CMatrix verTextMat(3, 3, homoTex);
	if (!vertMat.InvertGaussJordan())
	{
		return;
	}

	//| K  L  M |T
	//| N  P  Q |
	//| R  S  T |
	CMatrix resultM = vertMat * verTextMat;
	resultM = resultM.Transpose();
	double *p = resultM.GetData();
	memcpy(texMatrix, p, sizeof(double)*9);
}

double *CRaster::GetTexMatrix()
{
	return m_texMatrix;
}

// ������������������������ɫֵ��ʹ�þ�����ΪȨ�أ����ĸ����������������
void CRaster::GetTexData(RasteredPoints *rasteredPoints, Texture *tex)
{
	if (tex == NULL)
		return;

	RasteredPoints *point = rasteredPoints;
	unsigned int uFloor, vFloor;
	int nbpp = tex->bpp / 8;
	double uWeight;
	double vWeight;
	int nIndex11, nIndex12, nIndex21, nIndex22;
	while (point != NULL)
	{
		if (!point->bIsInside)
		{
			point = point->next;
			continue;
		}
		double u = point->u, v = point->v;
		while (u < 0)
			u += tex->width -1;
		while (v < 0)
			v += tex->height -1;
		while (u > (tex->width-1))
			u -= tex->width-1;
		while (v > (tex->height-1))
			v -= tex->height-1;


		uFloor = unsigned int(floor(u));
		vFloor = unsigned int(floor(/*tex->height-1 - */v));

		//uFloor = (uFloor >= tex->width)?(tex->width-1):uFloor;
		//uFloor = (uFloor < 0)? 0:uFloor;
		//vFloor = (vFloor >= tex->height)?(tex->height-1):vFloor;
		//vFloor = (vFloor < 0)?0:vFloor;

		uWeight = (double)abs(u - uFloor);
		vWeight = (double)abs((/*tex->height -1- */v) - vFloor);

		unsigned int uNext = ((uFloor+1)>=tex->width)?uFloor:(uFloor+1) ;
		unsigned int vNext = ((vFloor+1)>=tex->height)?vFloor:(vFloor+1) ;
		// �õ���ǰ���ص���Χ���ڵ��ĸ����ص�����
		nIndex11 = (uFloor + vFloor * tex->width)*nbpp;
		nIndex12 = (uFloor + vNext* tex->width)*nbpp;
		nIndex21 = (uNext + vFloor* tex->width )*nbpp;
		nIndex22 = (uNext + vNext * tex->width)*nbpp;

		// ����˫�����˲��õ���Ӧ����������ֵ
		for(int i = 0; i < nbpp; i++)
		{
			point->color[i] = unsigned int((tex->imageData[nIndex11+i] * (1-uWeight) + tex->imageData[nIndex21+i]*uWeight)*(1-vWeight)
				+(tex->imageData[nIndex12+i]*(1-uWeight) + tex->imageData[nIndex22+i]*uWeight)*vWeight + 0.5f);
		}
		// Ѱ����һ��դ���
		point = point->next;
	}
}

//----------------����任��غ���------------------------//

 //////////////////////////////////////////////////////////////////////////
/* Transform a point(column vector) by a 4x4 matrix. Then, out = m * in  
Input: m ----- the 4x4 matrix, in ---- the 4x1 vector  
Output: out ---- the resulting 4x1 vector*/  
void CRaster::transform_point(GLdouble out[4], const GLdouble m[16], const GLdouble in[4])   
{   
#define M(row,col) m[col*4+row]   
	out[0] =    
		M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];   
	out[1] =   
		M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];   
	out[2] =   
		M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];   
	out[3] =   
		M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];   
#undef M   
}   

// gluProject source code (˵����OpenGL API�ĵ�)   
// �������޸ģ�winz�����ǹ���������ֵ������ͶӰ��ɺ���������wֵ
GLint  CRaster::Project(GLdouble objx, GLdouble objy, GLdouble objz, const GLdouble  modelMatrix[16], const GLdouble projMatrix[16], const GLint viewport[4], GLdouble *winx, GLdouble *winy, GLdouble *winz)   
{   
	// matrice transformation   
	GLdouble in[4], out[4];   
	//initialize matrice and column vector as a transformer   
	in[0] = objx;   
	in[1] = objy;   
	in[2] = objz;   
	in[3] = 1.0;   
	transform_point(out, modelMatrix, in);  //����ģ����ͼ����   
	transform_point(in, projMatrix, out);   //����ͶӰ����   
	//��������ĵ������Ϊ0   
	if(in[3] == 0.0)   
		return GL_FALSE;   
	//������λ���׼��   
	in[0] /= in[3];   
	in[1] /= in[3];   
	in[2] /= in[3];   
	//�ӿ�����������   
	*winx = viewport[0] + (1 + in[0]) * viewport[2] / 2;   
	*winy = viewport[1] + (1 + in[1]) * viewport[3] / 2;   
	//*winz = (1 + in[2]) / 2;   

	// �����ײ��� [1/14/2010 Leo Han]
	*winz = in[3];

	return GL_TRUE;   
}  

//////////////////////////////////////////////////////////////////////////
// ����ά�ռ�ͶӰ����ά�����ϵ����м��㺯��(�ǵ���gluProject)
void CRaster::CustomProject(double pos3D[3], int &xi, int &yi, double &depth)
{
	GLint viewPort[4];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	glGetIntegerv(GL_VIEWPORT, viewPort);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	GLdouble winX, winY, winZ;
	Project/*gluProject*/(pos3D[0], pos3D[1], pos3D[2],
		modelMatrix, projMatrix, viewPort, &winX, &winY, &winZ);
	xi = round(winX); yi = /*viewPort[3] - (int)*/round(winY)/* - 1*/; depth = (double)winZ;
}

// ------------------------------------------------------------------------------------
// �Ӷ�ά����ӳ�䵽��ά�ռ�
void CRaster::ViewUnProject(int xi, int yi, double depth, double pos3D[3])
{
	GLint viewPort[4];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	glGetIntegerv(GL_VIEWPORT, viewPort);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	yi = viewPort[3] - yi - 1;
	GLdouble wx, wy, wz;
	gluUnProject((GLdouble) xi, (GLdouble) yi, (GLdouble) depth,
		modelMatrix, projMatrix, viewPort, &wx, &wy, &wz);
	pos3D[0] = wx;
	pos3D[1] = wy;
	pos3D[2] = wz;

}
// ����ά�ռ�ͶӰ����ά������
void CRaster::ViewProject(double pos3D[3], int &xi, int &yi, double &depth)
{
	GLint viewPort[4];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	glGetIntegerv(GL_VIEWPORT, viewPort);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	GLdouble winX, winY, winZ;
	gluProject(pos3D[0], pos3D[1], pos3D[2],
		modelMatrix, projMatrix, viewPort, &winX, &winY, &winZ);
	xi = round(winX); yi = viewPort[3] - round(winY) - 1; depth = (double)winZ;
}

// �õ����������е�ĳ��������Ļ�ϵ�ͶӰ
double CRaster::GetScreenCoord(double worldCoord[3], int screenCoord[2])
{
	double h;
	CustomProject/*ViewProject*/(worldCoord, screenCoord[0], screenCoord[1], h);
	return h;
}

//////////////////////////////////////////////////////////////////////////
// ���������㰴��˳ʱ��˳������
bool CRaster::ClockWizeTriangle(int V1[2], int V2[2], int V3[2])
{
	int nE = E(V3[0], V3[1], V1[0], V1[1],	V2[0],	V2[1]);
	if (nE < 0)				// ���������ڱ�V12����ߣ�����ʱ�룬��Ҫ����Ϊ˳ʱ��
	{
		int temp = V3[0];
		V3[0] = V2[0];
		V2[0] = temp;

		temp = V3[1];
		V3[1] = V2[1];
		V2[1] = temp;
	}
	else if (nE == 0)		// �����Ϲ涨����Ҫɾ��
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// �������ε��������㰴��˳ʱ������
//bool CRaster::ClockWizeTriangle(double V1[2], double V2[2], double V3[2])
//{
//	int nE = E(V3[0], V3[1], V1[0], V1[1],	V2[0],	V2[1]);
//	if (nE < 0)				// ���������ڱ�V12����ߣ�����ʱ�룬��Ҫ����Ϊ˳ʱ��
//	{
//		double temp = V3[0];
//		V3[0] = V2[0];
//		V2[0] = temp;
//
//		temp = V3[1];
//		V3[1] = V2[1];
//		V2[1] = temp;
//	}
//	else if (nE == 0)		// �����Ϲ涨����Ҫɾ��
//		return false;
//
//	return true;
//}

//----------------����غ���------------------------//

//////////////////////////////////////////////////////////////////////////
// �ͷ�դ����б�allPoints��ռ�Ŀռ�
void CRaster::Release()
{
	while (m_pRasteredPoints != NULL)
	{
		RasteredPoints *tmp = m_pRasteredPoints->next;
		delete m_pRasteredPoints;
		m_pRasteredPoints = tmp;
	}
	m_pRasteredPoints = NULL;

	if (m_pWang != NULL)
	{
		for(int i = 0; i <= m_pWang->nRasterLine; i++)
			delete []m_pWang->pBound[i];
		delete []m_pWang->pBound;
		delete []m_pWang->pLinePattern;
		delete []m_pWang->pNearLine;
		delete []m_pWang->pStep;
		delete m_pWang;
		m_pWang = NULL;
	}
}

CRaster::CRaster(void)
{
	m_pRasteredPoints = NULL;
	m_pWang = NULL;
}

CRaster::~CRaster(void)
{
	Release();
}

//////////////////////////////////////////////////////////////////////////
// �ⲿ���õĺ���������դ���Ժ�����ص�����
RasteredPoints*  CRaster::GetRasteredPoints()
{
	return m_pRasteredPoints;
}

//////////////////////////////////////////////////////////////////////////
// ����WANG���������ֵ�㷨�ĺ���

//////////////////////////////////////////////////////////////////////////
// ��һ���������е�λ��
bool normalize(double v[3])
{
	double r = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	if (r == 0) 
		return false;

	v[0] = v[0] / r;
	v[1] = v[1] / r;
	v[2] = v[2] / r;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ���������������
void  crossp(double v1[3], double v2[3], double result[3])
{

	result[0] = v1[1]*v2[2] - v1[2]*v2[1];
	result[1] = v1[2]*v2[0] - v1[0]*v2[2];
	result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

//////////////////////////////////////////////////////////////////////////
// �������������ĵ��
double pointp(double v1[3], double v2[3])
{
	double result = 0.0;
	for(int i = 0; i < 3; i++)
	{
		result += v1[i] * v2[i];
	}
	return result;
}


//////////////////////////////////////////////////////////////////////////
// ����������ת�����۲�����ϵ��
// ǰ���9��������gluLookAt�����Ĳ�����ͬ
bool CRaster::TransToViewCoord(double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz,
							   double pointCoord[3], double pointViewCoord[3])
{
	double m[4][4];
	double forw[3], side[3], up[3];

	forw[0] = centerx - eyex;
	forw[1] = centery - eyey;
	forw[2] = centerz - eyez;
	up[0] = upx;
	up[1] = upy;
	up[2] = upz;

	if (!normalize(forw))
		return false;

	crossp(forw, up, side);
	normalize(side);

	crossp(side, forw, up);

	double eye[3];
	eye[0] = eyex;
	eye[1] = eyey;
	eye[2] = eyez;

	// �õ��������굽�۲�����ı任����
	m[0][0] = side[0];
	m[0][1] = side[1];
	m[0][2] = side[2];
	m[0][3] = -pointp(side, eye);

	m[1][0] = up[0];
	m[1][1] = up[1];
	m[1][2] = up[2];
	m[1][3] = -pointp(up, eye);

	m[2][0] = -forw[0];
	m[2][1] = -forw[1];
	m[2][2] = -forw[2];
	m[2][3] = pointp(forw, eye);

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
	double in[4];
	in[0] = pointCoord[0];
	in[1] = pointCoord[1];
	in[2] = pointCoord[2];
	in[3] = 1;
	// �����;������
	pointViewCoord[0] =    
		m[0][0] * in[0] + m[0][1] * in[1] + m[0][2] * in[2] + m[0][3] * in[3];   
	pointViewCoord[1] =   
		m[1][0] * in[0] + m[1][1] * in[1] + m[1][2] * in[2] + m[1][3] * in[3];   
	pointViewCoord[2] =   
		m[2][0] * in[0] + m[2][1] * in[1] + m[2][2] * in[2] + m[2][3] * in[3]; 
	return true;
}

//////////////////////////////////////////////////////////////////////////
// �õ�����������ƽ��ת�����۲�����ϵ�󣬺�xyƽ���ཻ��ֱ�ߵ�б��
double CRaster::GetTriangleGradientInViewCoord(double worldV[9]
, double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz)
{
	double viewV[9];
	for (int i = 0; i < 3; i++)
	{
		TransToViewCoord(eyex,eyey, eyez,centerx, centery, centerz, upx, upy, upz,
			worldV+i*3, viewV+i*3);
	}
	// ���б��
	// б����[(y2-y1)*(z3-z1)-(y3-y1)*(z2-z1)]/[(x2-x1)*(z3-z1)-(x3-x1)*(z2-z1)]

	return ((viewV[4]-viewV[1])*(viewV[8]-viewV[2])-(viewV[7]-viewV[1])*(viewV[5]-viewV[2]))
		/ ((viewV[3]-viewV[0])*(viewV[8]-viewV[2])-(viewV[6]-viewV[0])*(viewV[5]-viewV[2]));
}

//////////////////////////////////////////////////////////////////////////
// �����߶�ɨ��ת���㷨�ĵõ�����������ƽ��ת�����۲�����ϵ�󣬺�xyƽ���ཻ��ֱ�ߵ�ɨ��ת�����
int* CRaster::GetLinePattern(GLdouble triangleVertex[9], double view[9], int &nPoints, double &fGradient)
{
	fGradient = GetTriangleGradientInViewCoord(triangleVertex, view[0], view[1], view[2], view[3], view[4], view[5], view[6], view[7], view[8]);
	GLint   triangleScreenVertex[6];
	GetScreenCoord(triangleVertex, triangleScreenVertex);
	GetScreenCoord(triangleVertex+3, triangleScreenVertex+2);
	GetScreenCoord(triangleVertex+6, triangleScreenVertex+4);
	// �������������Ļ��ͶӰ�İ�Χ��
	int nL = triangleScreenVertex[0], nT= triangleScreenVertex[1], nR= triangleScreenVertex[0], nB= triangleScreenVertex[1];
	for (int i = 1; i < 3; i++)
	{
		if (triangleScreenVertex[i*2] > nR)
			nR = triangleScreenVertex[i*2];
		else if (triangleScreenVertex[i*2] < nL)
			nL = triangleScreenVertex[i*2];
		if (triangleScreenVertex[i*2+1] > nT)
			nT = triangleScreenVertex[i*2+1];
		else if (triangleScreenVertex[i*2+1] < nB)
			nB = triangleScreenVertex[i*2+1];
	}

	int nStartX = 0, nStartY = 0;
	double nEndX, nEndY;
	int n = 0;
	// ����б���ֵ�ߵ�б��ȷ���߶ε�դ��ģʽ,���С��1�Ļ�,֤���ǰ���x����Ϊ��������,ģʽ�б�洢��Ӧ��yֵ,��֮��
	if (abs(fGradient) > 1.0)
	{
		nEndY = (nT-nB);
		nEndX = nEndY/fGradient; 
		n = (nT - nB)+1;
	}
	else
	{
		nEndX = (nR-nL);
		nEndY = nEndX*fGradient; 
		n = (nR-nL)+1;
	}

	//if (m_pWang->pStep != NULL)
	//	delete []m_pWang->pStep;
	//if (m_pWang->pNearLine != NULL)
	//	delete []m_pWang->pNearLine;

	m_pWang->pStep = new double [n];
	m_pWang->pNearLine = new int [n];
	

	return MidpointLine(nStartX, nStartY, nEndX, nEndY, nPoints, m_pWang->pStep);
}

//////////////////////////////////////////////////////////////////////////
// �����е��㷨���߶ε�դ�񻯽��
int* CRaster::MidpointLine(int x0, int y0, double x1, double y1, int &nP, double *pError)
{
	int x,y,i,s1,s2;
	double d,delta1,delta2,dx,dy,a,b;
	nP = 0;
	a=y0-y1;
	b=x1-x0;

	dx=abs(b);
	dy=abs(a);

	if(b>0)
		s1=1;
	else
		s1=-1;
	if(a<0)
		s2=1;
	else
		s2=-1;
	if(s1<0)
		a=-a;
	if(s2<0)
		b=-b;

	x=x0;
	y=y0;
	int *rasteredP = new int[int((dy<dx)?dx:dy)+1];

	pError[nP] = 0;
	double fStep = (dy<dx)?((y1-y0)/dx):((x1-x0)/dy);

	if(dy<=dx)
	{
		rasteredP[nP++] = y;
		d=a+a+b;
		delta1=a+a;
		delta2=delta1+b+b;
		if(s1==s2)
		{
			for(i=0;i<dx;i++)
			{
				if(d<0)
				{
					x+=s1;
					y+=s2;
					d+=delta2;
				}
				else
				{
					x+=s1;
					d+=delta1;
				}
				rasteredP[nP++] = y;

				pError[nP-1] = y - (i+1)*fStep;
			}
		}
		else
		{
			for(i=0;i<dx;i++)
			{
				if(d>=0)
				{
					x+=s1;
					y+=s2;
					d+=delta2;
				}
				else
				{
					x+=s1;
					d+=delta1;
				}
				rasteredP[nP++] = y;

				pError[nP-1] = y - (i+1)*fStep;
			}
		}
	}
	else
	{
		rasteredP[nP++] = x;
		d=a+b+b;
		delta1=b+b;
		delta2=delta1+a+a;
		if(s1==s2)
		{
			for(i=0;i<dy;i++)
			{
				if(d>0)
				{
					x+=s1;
					y+=s2;
					d+=delta2;
				}
				else
				{
					y+=s2;
					d+=delta1;
				}
				rasteredP[nP++] = x;

				pError[nP-1] = x - (i+1)*fStep;
			}
		}
		else
		{
			for(i=0;i<dy;i++)
			{
				if(d<0)
				{
					x+=s1;
					y+=s2;
					d+=delta2;
				}
				else
				{
					y+=s2;
					d+=delta1;
				}
				rasteredP[nP++] = x;

				pError[nP-1] = x - (i+1)*fStep;
			}
		}
	}
	return rasteredP;
}

// �����Բ�ֵ���
inline void CRaster::LinearInterTexCoord(double fSU, double fSV, double fStepU,double fStepV, int t, double &fU, double &fV)
{
	fU = fSU + t*fStepU;
	fV = fSV + t*fStepV;
}

void CRaster::LinearError(WangTexData *pWang)
{
	double fStep = 1.0/pWang->fRasterLineGap;
	int nIndex = 0;
	for (int i = 0; i < pWang->nPatternNum; i++)
	{
		if (abs(pWang->pStep[i]) < 0.00001)
		{
			pWang->pStep[i] = 0;
		}
		else if (0.5-abs(pWang->pStep[i])<0.00001)
		{
			pWang->pStep[i] = 0.5;
		}
		double error = pWang->pStep[i] * (pWang->nRasterLine+1);
		nIndex = int(error);
		pWang->pStep[i] = error-nIndex;

		// ���������������������Ļ�����Ҫ�ҵ��뵱ǰ���ص������б��ɨ���е��±�
		if (!pWang->bLinearReduceError && abs(pWang->pStep[i]) > 0.5)
			pWang->pNearLine[i] = abs(nIndex)+1;
		else
			pWang->pNearLine[i] = abs(nIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// ����ɨ�����������
// @nRasterLine��ָ������˼���ɨ���ߣ��Ա�������
RasterLineHead** CRaster::CalcBoundBoxData()
{
	int sortedV[6];
	SortVertex(m_pScreenV[0],m_pScreenV[1],m_pScreenV[2],m_pScreenV[3],m_pScreenV[4],m_pScreenV[5], sortedV);
	// ��˳ʱ��,���ϵ��µ�˳��������������
	ClockWizeTriangle(sortedV, sortedV+2, sortedV+4);
	// ��������εİ�Χ��
	int nT, nB, nL, nR;
	nT = sortedV[1];
	nB = (sortedV[3]>sortedV[5])?sortedV[5]:sortedV[3];
	nL = nR = sortedV[0];
	for (int i = 1; i < 3; i++)
	{
		if (sortedV[i*2] < nL)
			nL = sortedV[i*2];
		else if (sortedV[i*2] > nR)
			nR = sortedV[i*2];
	}
	int nStartX, nStartY;			// б��ɨ���е���ʼ������.����ɨ���ڰ�Χ�е�����,����ɨ���ڰ�Χ�е��±�
	int nEndX, nEndY;
	int step = 100;
	if (abs(m_pWang->fGradient) > 1.0)
	{
		m_pWang->dir = RIGHT;
		step = (m_pWang->fGradient > 0)?100:-100;

		// �ҵ�б��ɨ����������Ķ���,�������ж���Ӧ�ö��������ߵ��ұ�

		// б��ɨ���й���һ������
		if (E(sortedV[2], sortedV[3], sortedV[0], sortedV[1], sortedV[0]+step, round(step*m_pWang->fGradient+sortedV[1]))>=0
			&&E(sortedV[4], sortedV[5], sortedV[0], sortedV[1], sortedV[0]+step, round(step*m_pWang->fGradient+sortedV[1]))>=0)
		{
			nStartY = nB;
			nStartX = sortedV[0]-m_pWang->pLinePattern[sortedV[1]-nB];

		}
		else if (E(sortedV[2], sortedV[3], sortedV[0], sortedV[1], sortedV[0]+step, round(step*m_pWang->fGradient+sortedV[1])<=0
			&&E(sortedV[4], sortedV[5], sortedV[0], sortedV[1], sortedV[0]+step, round(step*m_pWang->fGradient+sortedV[1]))<=0))
		{
			nEndY = nB;
			nEndX = sortedV[0]-m_pWang->pLinePattern[sortedV[1]-nB];
		}

		// б��ɨ���й��ڶ�������
		if (E(sortedV[0], sortedV[1], sortedV[2], sortedV[3], sortedV[2]+step, round(step*m_pWang->fGradient+sortedV[3])>=0)
			&&E(sortedV[4], sortedV[5], sortedV[2], sortedV[3], sortedV[2]+step, round(step*m_pWang->fGradient+sortedV[3]))>=0)
		{
			nStartY = nB;
			nStartX = sortedV[2]-m_pWang->pLinePattern[sortedV[3]-nB];
		}
		else if (E(sortedV[0], sortedV[1], sortedV[2], sortedV[3], sortedV[2]+step, round(step*m_pWang->fGradient+sortedV[3]))<=0
			&&E(sortedV[4], sortedV[5], sortedV[2], sortedV[3], sortedV[2]+step, round(step*m_pWang->fGradient+sortedV[3]))<=0)
		{
			nEndY = nB;
			nEndX = sortedV[2]-m_pWang->pLinePattern[sortedV[3]-nB];
		}

		// б��ɨ���й�����������
		if (E(sortedV[0], sortedV[1], sortedV[4], sortedV[5], sortedV[4]+step,round( step*m_pWang->fGradient+sortedV[5]))>=0
			&&E(sortedV[2], sortedV[3], sortedV[4], sortedV[5], sortedV[4]+step, round(step*m_pWang->fGradient+sortedV[5]))>=0)
		{
			nStartY = nB;
			nStartX = sortedV[4]-m_pWang->pLinePattern[sortedV[5]-nB];
		}
		else if (E(sortedV[0], sortedV[1], sortedV[4], sortedV[5], sortedV[4]+step, round(step*m_pWang->fGradient+sortedV[5]))<=0
			&&E(sortedV[2], sortedV[3], sortedV[4], sortedV[5], sortedV[4]+step, round(step*m_pWang->fGradient+sortedV[5]))<=0)
		{
			nEndY = nB;
			nEndX = sortedV[4]-m_pWang->pLinePattern[sortedV[5]-nB];
		}
		nEndX += 1;
		nStartX -= 1;
		// �����ɨ���и���
		m_pWang->nBoundNum = nEndX - nStartX+1;	
	}
	else
	{
		m_pWang->dir = UP;
		// б��ɨ���й���һ������
		if (E(sortedV[2], sortedV[3], sortedV[0], sortedV[1], sortedV[0]+step, round(step*m_pWang->fGradient+sortedV[1]))<=0
			&&E(sortedV[4], sortedV[5], sortedV[0], sortedV[1], sortedV[0]+step, round(step*m_pWang->fGradient+sortedV[1]))<=0)
		{
			nStartX = nL;
			nStartY = sortedV[1]-m_pWang->pLinePattern[sortedV[0]-nL];

		}
		else if (E(sortedV[2], sortedV[3], sortedV[0], sortedV[1], sortedV[0]+step, round(step*m_pWang->fGradient+sortedV[1]))>=0
			&&E(sortedV[4], sortedV[5], sortedV[0], sortedV[1], sortedV[0]+step, round(step*m_pWang->fGradient+sortedV[1]))>=0)
		{
			nEndX = nL;
			nEndY = sortedV[1]-m_pWang->pLinePattern[sortedV[0]-nL];
		}

		// б��ɨ���й��ڶ�������
		if (E(sortedV[0], sortedV[1], sortedV[2], sortedV[3], sortedV[2]+step, round(step*m_pWang->fGradient+sortedV[3]))<=0
			&&E(sortedV[4], sortedV[5], sortedV[2], sortedV[3], sortedV[2]+step, round(step*m_pWang->fGradient+sortedV[3]))<=0)
		{
			nStartX = nL;
			nStartY = sortedV[3]-m_pWang->pLinePattern[sortedV[2]-nL];
		}
		else if (E(sortedV[0], sortedV[1], sortedV[2], sortedV[3], sortedV[2]+step, round(step*m_pWang->fGradient+sortedV[3]))>=0
			&&E(sortedV[4], sortedV[5], sortedV[2], sortedV[3], sortedV[2]+step, round(step*m_pWang->fGradient+sortedV[3]))>=0)
		{
			nEndX = nL;
			nEndY = sortedV[3]-m_pWang->pLinePattern[sortedV[2]-nL];
		}

		// б��ɨ���й�����������
		if (E(sortedV[0], sortedV[1], sortedV[4], sortedV[5], sortedV[4]+step, round(step*m_pWang->fGradient+sortedV[5]))<=0
			&&E(sortedV[2], sortedV[3], sortedV[4], sortedV[5], sortedV[4]+step, round(step*m_pWang->fGradient+sortedV[5]))<=0)
		{
			nStartX = nL;
			nStartY = sortedV[5]-m_pWang->pLinePattern[sortedV[4]-nL];
		}
		else if (E(sortedV[0], sortedV[1], sortedV[4], sortedV[5], sortedV[4]+step, round(step*m_pWang->fGradient+sortedV[5]))>=0
			&&E(sortedV[2], sortedV[3], sortedV[4], sortedV[5], sortedV[4]+step, round(step*m_pWang->fGradient+sortedV[5]))>=0)
		{
			nEndX = nL;
			nEndY = sortedV[5]-m_pWang->pLinePattern[sortedV[4]-nL];
		}
		nEndY += 1;
		nStartY -= 1;
		m_pWang->nBoundNum = nEndY - nStartY +1;
	}

	double fPattern = 1.0/(m_pWang->nPatternNum-1);

	double K, L, M, N, P, Q, R, S, T;
	K = m_texMatrix[0];
	L = m_texMatrix[1];
	M = m_texMatrix[2];
	N = m_texMatrix[3];
	P = m_texMatrix[4];
	Q = m_texMatrix[5];
	R = m_texMatrix[6];
	S = m_texMatrix[7];
	T = m_texMatrix[8];

	RasterLineHead **pBoundHead = new RasterLineHead*[m_pWang->nRasterLine+1];
	for (int i = 0; i <= m_pWang->nRasterLine; i++)
		pBoundHead[i] = new RasterLineHead[m_pWang->nBoundNum];

	RasteredPoints *pHead = new RasteredPoints;
	RasteredPoints *pTail = new RasteredPoints;
	m_pWang->fRasterLineGap = 1.0/(m_pWang->nRasterLine+1);
	
	double fTmp;
	double fGradD = 1.0/ m_pWang->fGradient;

	for (int nLine = 0; nLine <= m_pWang->nRasterLine; nLine++)
	{
		if (m_pWang->dir == UP)
		{
			// ʹ��ustep��vstep����ʾx��yֵ����Ϊ������ֵ�Ǹ�������
			pHead->uStep = nStartX;
			pHead->vStep = nStartY + nLine*m_pWang->fRasterLineGap;

			pTail->uStep = nR;
			pTail->vStep = (nStartY + (nR-nL) * m_pWang->fGradient) + nLine*m_pWang->fRasterLineGap;
		}
		else if (m_pWang->dir == RIGHT)
		{
			// ʹ��ustep��vstep����ʾx��yֵ����Ϊ������ֵ�Ǹ�������
			pHead->vStep = nStartY;
			pHead->uStep = nStartX + nLine*m_pWang->fRasterLineGap;

			pTail->vStep = nT;
			pTail->uStep = (nStartX + (nT-nB) * fGradD) + nLine*m_pWang->fRasterLineGap;
		}
		// ����ÿ��б��ɨ���е���������
		fTmp = 1.0/(R*pHead->uStep + S*pHead->vStep + T);
		pHead->u = (K*pHead->uStep + L*pHead->vStep + M)*fTmp;
		pHead->v = (N*pHead->uStep + P*pHead->vStep + Q)*fTmp;

		fTmp = 1.0/(R*pTail->uStep + S*pTail->vStep + T);
		pTail->u = (K*pTail->uStep + L*pTail->vStep + M)*fTmp;
		pTail->v = (N*pTail->uStep + P*pTail->vStep + Q)*fTmp;

		pBoundHead[nLine][0].uStep = (pTail->u - pHead->u)*fPattern;
		pBoundHead[nLine][0].vStep = (pTail->v - pHead->v)*fPattern;
		pBoundHead[nLine][0].u = pHead->u;
		pBoundHead[nLine][0].v = pHead->v;
		pBoundHead[nLine][0].x = pHead->uStep;
		pBoundHead[nLine][0].y = pHead->vStep;

		if (m_bPerPixelDiv)
		{
			for (int i = 1; i < m_pWang->nBoundNum; i++)
			{
				if (m_pWang->dir == RIGHT)
				{
					pTail->uStep = pTail->uStep+1;
					pHead->uStep = pHead->uStep+1;
				}
				else if (m_pWang->dir == UP)
				{
					pTail->vStep = pTail->vStep+1;
					pHead->vStep = pHead->vStep+1;
				}
				// ����ÿ��б��ɨ���е���������
				fTmp = 1.0/(R*pHead->uStep + S*pHead->vStep + T);
				pHead->u = (K*pHead->uStep + L*pHead->vStep + M)*fTmp;
				pHead->v = (N*pHead->uStep + P*pHead->vStep + Q)*fTmp;

				fTmp = 1.0/(R*pTail->uStep + S*pTail->vStep + T);
				pTail->u = (K*pTail->uStep + L*pTail->vStep + M)*fTmp;
				pTail->v = (N*pTail->uStep + P*pTail->vStep + Q)*fTmp;

				pBoundHead[nLine][i].uStep = (pTail->u - pHead->u)*fPattern;
				pBoundHead[nLine][i].vStep = (pTail->v - pHead->v)*fPattern;
				pBoundHead[nLine][i].u = pHead->u;
				pBoundHead[nLine][i].v = pHead->v;
				pBoundHead[nLine][i].x = pHead->uStep;
				pBoundHead[nLine][i].y = pHead->vStep;
			}
		}
		else
		{
			double u = floor(pHead->u);
			double v = floor(pHead->v);

			pHead->d = 2*(R*pHead->uStep + S*pHead->vStep + T);

			pHead->Au = 2*u*R - 2*K;
			pHead->Bu = 2*u*S - 2*L;
			pHead->E = u*pHead->d - 2*(K*pHead->uStep + L*pHead->vStep + M);

			pHead->Av = 2*v*R - 2*N;
			pHead->Bv = 2*v*S - 2*P;
			pHead->F = v*pHead->d - 2*(N*pHead->uStep + P*pHead->vStep + Q);

			u = floor(pTail->u);
			v = floor(pTail->v);

			pTail->d = 2*(R*pTail->uStep + S*pTail->vStep + T);

			pTail->Au = 2*u*R - 2*K;
			pTail->Bu = 2*u*S - 2*L;
			pTail->E = u*pTail->d - 2*(K*pTail->uStep + L*pTail->vStep + M);

			pTail->Av = 2*v*R - 2*N;
			pTail->Bv = 2*v*S - 2*P;
			pTail->F = v*pTail->d - 2*(N*pTail->uStep + P*pTail->vStep + Q);


			for (int i = 1; i < m_pWang->nBoundNum; i++)
			{
				if (m_pWang->dir == RIGHT)
				{
					pTail->uStep = pTail->uStep+1;
					pHead->uStep = pHead->uStep+1;
				}
				else if (m_pWang->dir == UP)
				{
					pTail->vStep = pTail->vStep+1;
					pHead->vStep = pHead->vStep+1;
				}
				CalcTexCoord(m_texMatrix, pHead, pHead, m_pWang->dir, m_mBit, false);
				CalcTexCoord(m_texMatrix, pTail, pTail, m_pWang->dir, m_mBit, false);

				pBoundHead[nLine][i].uStep = (pTail->u - pHead->u)*fPattern;
				pBoundHead[nLine][i].vStep = (pTail->v - pHead->v)*fPattern;
				pBoundHead[nLine][i].u = pHead->u;
				pBoundHead[nLine][i].v = pHead->v;
				pBoundHead[nLine][i].x = pHead->uStep;
				pBoundHead[nLine][i].y = pHead->vStep;
			}
		}
	}

	delete pTail;
	delete pHead;
	return pBoundHead;
}


//////////////////////////////////////////////////////////////////////////
// ���������ֵ���㵱ǰդ������������
// @*pLinePattern:һ��б�ʵ��߶ε�դ��ģʽ,��(0,0)��λ��ʼ��,ÿ��Ԫ�ش洢������(��б�ʾ���ֵ����1��ʱ��洢���Ǻ�����)
// @*thisPoint:���������ض���
// @*Bound:������б��ɨ���е�ÿ��ɨ������ʼ�����Ϣ(���������Լ��������경��)
// @nPoints:դ��ģʽ����ĳ���
// @dir:ɨ�跽��,������:���ϻ�������(����ͶӰֱ�ߵ�б�ʻ���,б�ʾ���ֵС��1Ϊǰ��,����Ϊ����)
inline void CRaster::CalcTexCoordWANG(RasteredPoints *thisPoint)
{
	int nStartX = int(m_pWang->pBound[0][0].x);
	int nStartY = int(m_pWang->pBound[0][0].y); 	
	int nNearLine = 0;		// ��ǰ����������������ɨ���б��(ÿ�������ڵ�ɨ���б�Ŵ�0��ʼ,����1,2ֱ��ɨ������)
	int nIndex = 0;			// ָ������ɨ����������±�
	int nNextLine = 0;		// ��һ������ɨ���б��
	int nNextIndex = 0;		// ��һ������ɨ����������±�
	int nOffset = 0;		// ��ǰ���ص�λ��(���ڼ������Բ�ֵ��t)

	double uTmp, vTmp;
	if (m_pWang->dir == UP)
	{
		nIndex = thisPoint->y - m_pWang->pLinePattern[thisPoint->x - nStartX]- nStartY;
		nOffset = thisPoint->x - nStartX;
	}
	else if (m_pWang->dir == RIGHT)
	{
		nIndex = thisPoint->x - m_pWang->pLinePattern[thisPoint->y- nStartY] - nStartX;
		nOffset = thisPoint->y - nStartY;
	}
	if (nIndex >= m_pWang->nBoundNum || nIndex < 0)
	{
		thisPoint->u = 0;
		thisPoint->v = 0;
		return;
	}

	nNearLine = m_pWang->pNearLine[nOffset];
	// ���Ҫ��������������Ե����Ļ�����Ҫ��������ɨ���ߵ���Ϣ
	if (m_pWang->bLinearReduceError)		
	{
		// �����ǰ�����С��0�Ļ�,֤����ǰ����������ɨ���е�����,��Ҫ����
	// ���㵱ǰ������������ɨ���е���Ϣ,�Ա����ڽ������Բ�ֵ�������,�����ҵ������ɨ���ж�Ӧλ��
		if (m_pWang->pStep[nOffset] < 0)
		{
			nNextIndex = nIndex - 1;
			if (m_pWang->nRasterLine == 0)
			{
				nNextLine = nNearLine = 0;
			}
			else
			{
				nNextLine = m_pWang->nRasterLine - nNearLine ;
				if (nNearLine != 0)
				{
					nIndex--; 
					nNearLine = m_pWang->nRasterLine - nNearLine+1;
				}
			}
		}
		else
		{			
			// ���û�ж�����ɨ����
			if (m_pWang->nRasterLine == 0)
			{
				nNextIndex = nIndex + 1;
				nNextLine = nNearLine;
			}
			else
			{
				nNextLine = nNearLine + 1;
				nNextIndex = nIndex;
			}
		}
	}
	// ������С��1����������ɨ���߱�Ų�Ϊ0�Ļ���֤����ɨ���е����棬��������ɨ������Ϣ
	else if (m_pWang->pStep[nOffset] < 0 && nNearLine != 0)
	{
		nNearLine = m_pWang->nRasterLine - nNearLine+1;
		nIndex--; 
	}

	thisPoint->u = m_pWang->pBound[nNearLine][nIndex].u + m_pWang->pBound[nNearLine][nIndex].uStep*nOffset;
	thisPoint->v = m_pWang->pBound[nNearLine][nIndex].v + m_pWang->pBound[nNearLine][nIndex].vStep*nOffset;

	// ���������������ֱ��ʹ��б��ɨ���еĶ�Ӧ�������꣬�������õ�ǰ��Ļ����λ����Ϊ���Բ�ֵ��t��������ɨ����֮��������Բ�ֵ�õ���ǰ���ص���������
	// ��Ȼ���������Թ�ϵ�������Խ��ƿ������Թ�ϵ
	if (m_pWang->bLinearReduceError)			
	{
		if (nNextIndex >= 0 && nNextIndex < m_pWang->nBoundNum)
		{
			uTmp = m_pWang->pBound[nNextLine][nNextIndex].u + m_pWang->pBound[nNextLine][nNextIndex].uStep*nOffset;
			vTmp = m_pWang->pBound[nNextLine][nNextIndex].v + m_pWang->pBound[nNextLine][nNextIndex].vStep*nOffset;

			thisPoint->u += abs(m_pWang->pStep[nOffset])*(uTmp - thisPoint->u);
			thisPoint->v += abs(m_pWang->pStep[nOffset])*(vTmp - thisPoint->v);
		}	
	}
}

// �õ��������������������ݣ�����б��ɨ���е�б�ʵ�
WangTexData* CRaster::GetWangTexData(void)
{
	return m_pWang;
}