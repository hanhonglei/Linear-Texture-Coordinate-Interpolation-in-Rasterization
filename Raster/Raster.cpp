/********************************************************************
实现三角形栅格化的功能，主要分为两个部分：栅格化、在栅格化过程中求每个像素的透视正确的纹理坐标
实现原理如下：

一、栅格化算法
	***KIM算法：
	从三角形的顶点在屏幕上的最上面顶点开始栅格化，逐层下移。计算每个点的Ttri值，并按照Ttri的不同决定下一步走向：
	1.	如果当前的Ttri为00，证明当前点在三角形内，保存当前点右边点信息到RightSave中，然后分别遍历这个点的左右两边
	2.	如果当前点的Ttri为10，向左遍历，直到Ttri为01或者11，然后下移一行；
	3.	如果当前点的Ttri为01，向右遍历，直到Ttri为10或者11，然后下一行；
	4.	如果当前点的Ttri为11，结束本行，直接下移一行。
	5.	直到没有点被绘制
	Ttri的计算方法如下：
	Ttri是一个两位的二进制数，有4个状态：00，01，10，11
	Ttri的状态是由当前点和三角形的三条边（顺时针方向）的关系T决定的，它是三个T取“或”的结果。
	T的状态是由E的正负以及当前边的Δy决定的,具体算法是（以当前像素点和边0的关系为例）：
	T0 = 00 if E0(x, y) >= 0 
	T0 = 01 if E0(x, y) < 0 and Δy >= 0
	T0 = 10 if E0(x, y) < 0 and Δy < 0
	***WANG算法
	首先按照y方向顺序对三个定点的屏幕坐标进行排序，最上顶点和最下顶点组成的边（起始边）在每行上的像素作为每行的出发顶点
	按照中间顶点和这个边的关系决定每行是向左还是向右遍历（如果中间点在起始边的右边，则向右遍历，反之向左）直到出界
	判断出界的条件是当前像素和除起始边外其他两条边的关系（而不是求Ttri，这样省去了至少1/3的计算量）
二、纹理坐标插值
	和“一”中的栅格化算法紧密相关，每个像素点的纹理坐标的求取和其相邻点相关（栅格化过程是顺序进行的，从上到下，从左到右或从右到左）
	设下列变量：
	1、屏幕坐标：				screen coord:(x, y)
	2、屏幕坐标的齐次形式：		object coord:(x',y',w')
	3、纹理映射矩阵：
								Matrix = 
								| K  L  M |
								| N  P  Q |
								| R  S  T |
	4、三者之间的关系为：(其中(u, v)为纹理坐标)
							(x, y) = (x' / w', y' / w')
							| u | = | K  L  M | |x'|
							| v | = | N  P  Q | |y'|
							| 1 | = | R  S  T | |w'|
	5、可以得到，坐标和屏幕坐标之间的关系是：
					(u, v) = ((Kx + Ly + M)/(Rx + Sy + T), (Nx + Py +Q)/(Rx + Sy + T))

	为了避免上式中逐像素的除法，可以利用栅格化过程中相邻像素的相关性（当前像素中这些变量的求解是通过上一个像素得到的，而不是重新计算）。
	求解一个像素的纹理坐标分为求整数部分和小数部分两步。
	A、求纹理坐标的整数部分（和文中提出的MidPoint Algorithm.方法基本相同，只有A和E的计算有区别）：
		设定几个公式
				对于求解u
						E(x, y, u) = ud(x, y) - 2*(Kx + Ly + M)
						A(u) = 2uR - 2K
						B(u) = 2uS - 2L
				对于求解v
						F(x, y, v) = vd(x, y) - 2*(Nx +Py +Q)
						A(v) = 2vR - 2N
						B(v) = 2vs - 2P
		每个像素计算得到的u需要满足的条件（假设d>0,否则相反）
						-d(xi, yi) < E(xi, yi, ui) <= 0   -----------------  *
		计算过程如下：
		每个像素的d、E、F、A、B都依靠上一个已计算的像素得到。
		如果*式成立，则当前的u或者v满足条件，否则u和v加1或者减1。
		迭代关系如下：
			(1)求解u
				当x前进1的时候
						d(x+1, y) = d(x, y) + 2R
						E(x+1, y, u) = E(x, y, u) + A(u)
				当y前进1的时候
						d(x, y+1) = d(x, y) + 2S
						E(x, y+1, u) = E'(x, y, u) + B'(u)
				当u前进1的时候
						E(x, y, u+1) = E'(x, y, u) + d(x, y)
						A(u + 1) = A'(u) + 2R
						B(u + 1) = B'(u) + 2S

			(2)求解v
				当x前进1的时候
						d(x+1, y) = d(x, y ) + 2R
						F(x+1, y, v) = F(x, y, u) + A(v)
				当y前进1的时候
						d(x, y+1) = d(x, y) + 2S
						F(x, y+1, v) = F(x, y, v) + B'(v)
				当v前进1的时候
						F(x, y, v+1) = F'(x, y, v) + d(x, y)
						A(v + 1) = A(v) + 2R
						B(v + 1) = B(v) + 2S
	B.求解纹理坐标的小数部分，以求u为例。（小数部分的精度可控，通过m控制小数点后几位）
		小数部分可以表示为:1/pow(2.0, 1)?+1/pow(2.0, 1)?+1/pow(2.0, 1)?+...+1/pow(2.0, m)?
		其中“？”表示当前这一位是否保留，保留与否决定于利用其计算的E是否满足条件。
		算法伪代码如下：
					En = thisPixel->E;
					for (int i = 1; i <= m; i++)
					{
						if (En <= 0 && En > (d/pow(2.0, i)))    
							第1位不保留
						else if (En <= (-thisPixel->d/pow(2.0, i)) && En > (-thisPixel->d/pow(2.0, i-1)))
							第i位保留
						En = En + thisPixel->d*kSetU[i]/pow(2.0f, i);	// 计算下一位的En值
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
// 计算一个浮点数四舍五入后的结果
int round(double f)
{
	if (f < 0)
		return int(f-0.5);
	else
		return int(f+0.5);
}


//////////////////////////////////////////////////////////////////////////
// 开放给外部调用的三角形栅格化以及纹理映射的函数
// @GLdouble triangleVertex[9]：三角形三个顶点的世界坐标
// @double vertexTexCoord[6]:三个顶点对应的纹理坐标值
// @m:纹理坐标计算的精度
// @tex:纹理数据指针
// @rasterType:栅格化方式，有两种：一种是文章提出的Z字型，另外一种是王老师提出的
double CRaster::RasterCalTex(GLdouble triangleVertex[9], double vertexTexCoord[6], int m, double view[9], Texture *tex
							 , RASTERTYPE rasterType, bool bPerPixelDiv, CALTEXCOORDTYPE texType, bool bLinearReduceError, int nRasterLine)
{

	// 删除已有的像素链表
	Release();
	// 记录栅格化所用时间
	LONGLONG   t1 = 0,t2 = 0;   
	LONGLONG   persecond = 0;   
	QueryPerformanceFrequency((LARGE_INTEGER   *)&persecond);//询问系统一秒钟的频率   
	double perms = persecond / 1000.0;
	QueryPerformanceCounter((LARGE_INTEGER   *)&t1);   
	// 计算三角形三个顶点的屏幕坐标
	GLint   triangleScreenVertex[6];
	GetScreenCoord(triangleVertex, triangleScreenVertex);
	GetScreenCoord(triangleVertex+3, triangleScreenVertex+2);
	GetScreenCoord(triangleVertex+6, triangleScreenVertex+4);

	// 求出纹理映射矩阵
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

	// 对成员变量进行参数化，减少调用函数时的参数传递
	InitParams(triangleScreenVertex, texMatrix, m, rasterType, bPerPixelDiv, texType);
//////////////////////////////////////////////////////////////////////////
	// 如果是WANG模式得到纹理坐标的话，需要计算用于纹理坐标运算的数据
	if (texType == WANGTEX)
	{
		m_pWang = new WangTexData;
		m_pWang->bLinearReduceError = bLinearReduceError;
		m_pWang->nRasterLine = nRasterLine;
		// 得到在观察坐标系下三角形所在平面和xy平面交线的斜率；得到斜率以后按照midpoint算法求得该斜率的直线栅格化后的结果。
		m_pWang->pLinePattern = GetLinePattern(triangleVertex, view, m_pWang->nPatternNum, m_pWang->fGradient);
		// 以该斜率为扫描模式，逐层扫描三角形。计算每个斜向扫描行的起点信息（包括纹理坐标和纹理插值步长）
		m_pWang->pBound = CalcBoundBoxData();
		LinearError(m_pWang);
	}

	// 利用三角形栅格化算法对屏幕坐标下的三角形进行栅格化
	// 在栅格化的过程当中利用文中提出的纹理坐标计算方法计算每个像素点的纹理坐标值
	TriangleRasterization();

	QueryPerformanceCounter((LARGE_INTEGER   *)&t2);   
	double   time=double(t2-t1)/perms;			// 求得栅格化所用时间

	// 利用得到的纹理坐标求得最终的每个像素颜色值
	if (tex != NULL)
	{
		GetTexData(m_pRasteredPoints, tex);
	}

	return time;
}

void CRaster::InitParams(int screenV[6],double texMatrix[9],int m, RASTERTYPE rType, bool bPerPixelDiv, CALTEXCOORDTYPE texType)
{
	memcpy(m_pScreenV, screenV, 6*sizeof(int));
	// 调整三个顶点为顺时针顺序
	ClockWizeTriangle(m_pScreenV, m_pScreenV+2, m_pScreenV+4);

	memcpy(m_texMatrix, texMatrix, 9*sizeof(double));
	m_mBit = m;
	m_rType = rType;
	m_bPerPixelDiv = bPerPixelDiv;
	m_tType = texType;
}

//-----------------------------三角形栅格化相关函数----------------------------------//

//////////////////////////////////////////////////////////////////////////
// 计算函数E(x, y) = (x - x0) × (y1 - y0) - (y - y0) × (x1 - x0)
// E(x,y) > 0 :点(x, y) 在有向边(V0, V1)的右边
// E(x,y) < 0 :点(x, y) 在有向边(V0, V1)的左边
// E(x,y) = 0 :点(x, y) 在有向边(V0, V1)上面
// @vx, vy:待判定的点
// @v0x,v0y,v1x,v1y:线段的起点和终点
inline int CRaster::E(int vx, int vy,int v0x,int v0y,int v1x,int v1y)
{
	return ((vx - v0x) * (v1y - v0y) - (vy - v0y) * (v1x - v0x));
}

//////////////////////////////////////////////////////////////////////////
// 按照文章的方法计算一个点的Ttri
// Ttri是一个两位的二进制数，有4个状态：00，01，10，11
// Ttri的状态是由当前点和三角形的三条边（顺时针方向）的关系T决定的，它是三个T取“或”的结果。
// T的状态是由E的正负以及当前边的Δy决定的,具体算法是：
// T0 = 00 if E0(x, y) >= 0 
// T0 = 01 if E0(x, y) < 0 and Δy >= 0
// T0 = 10 if E0(x, y) < 0 and Δy < 0
// 其他两个T的算法和上面的算法一样
inline int CRaster::GetTtri(int vx, int vy, int v1x, int v1y,int v2x,int v2y,int v3x,int v3y)
{
	// 求得当前像素点和三条边的编码，然后求“或”，得到最终的编码并返回
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
// 栅格化算法的递归函数
// 原理:判断当前像素点的编码,进而决定当前点和三角形的关系:
// 1 如果当前的Ttri为00，证明当前点在三角形内，记录当前点.接着按照已有的遍历方向继续调用当前函数遍历下一个像素点
// 2 如果当前点的Ttri为10，则当前点不在三角形内,继续调用该函数向左遍历，直到Ttri为01或者11，然后结束；
// 3 如果当前点的Ttri为01，则当前点不在三角形内,继续调用该函数向右遍历，直到Ttri为10或者11，然后结束；
// 在遍历的过程中,利用CalcTexCoord函数以及相邻点的信息求得当前点的纹理坐标值
// WANG栅格化模式下，多数情况只判断当前像素和另外两条非起始边的关系

// @dir：当前像素点和前一个点的位置关系
// @unit：像素大小（一般为1）
// @pPrePixel：前一个像素点的信息
// @allPixels：栅格后的像素点列表指针。当前点在此函数中将像素点信息初始化，然后插入这个列表中
// @x, y：当前像素点的坐标值
// @v1x, 1y,v2x,v2y,v3x,v3y：三角形的顶点坐标值，用来计算编码。必须按照顺指针顺序传入
// @texMatrix[9]：纹理映射矩阵
// @m：纹理坐标精度控制
// @RASTERTYPE rType:栅格化类型，默认为KIM
// @double *sortedV：按照从上到下的顺序排列好的三个顶点坐标，在WANG模式下使用，用于找到非起始边
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
	// 如果当前是WANG模式，并且已经找到每行的起始点的话，计算当前点和两条非起始边的位置关系
	if (allPixels->firstValidate != NULL && m_rType == WANG)
	{
		fE1 = E(x, y, sortedV[0], sortedV[1], sortedV[2], sortedV[3]);
		fE2 = E(x, y, sortedV[2], sortedV[3], sortedV[4], sortedV[5]);
	}
	else
	{
		// 计算当前栅格点的编码
		Ttri = GetTtri(x, y, v1x, v1y, v2x, v2y, v3x, v3y);
	}

	// 新建当前栅格点的结构体,并将其插入栅格点列表中
	RasteredPoints * newPoint = new RasteredPoints;

	newPoint->x = x;
	newPoint->y = y;
	newPoint->next = NULL;
	newPoint->bIsInside = false;
	allPixels->last->next = newPoint;
	allPixels->last = newPoint;

	// 利用上一个相邻栅格点计算当前栅格点的纹理坐标值
	if (m_tType == KIMTEX)
		CalcTexCoord(m_texMatrix, newPoint, pPrePixel, dir, m_mBit, m_bPerPixelDiv);

	if (dir == LEFT)
	{
		if (allPixels->firstValidate == NULL || m_rType == KIM)		// 如果是KIM模式或者在WANG模式下没有找到起始像素的话，按照编码判断当前像素的类型
		{
			if (Ttri == 0x01 || Ttri == 0x03)
				// 记录本行第一个无效点
			{
				allPixels->firstInvalidate = newPoint;
				return;
			}
			else if (Ttri == 0x02)
				// 继续向左遍历
			{
				Rasterization(dir, unit, newPoint, allPixels, x-unit, y ,sortedV);
			}
			else if (Ttri == 0x00)
				// 在三角形内则记录当前点
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
		else														// 在WANG模式下按照点和非起始边的关系判断当前像素点的位置
		{
			// E(x,y) > 0 :点(x, y) 在有向边(V0, V1)的右边
			// E(x,y) < 0 :点(x, y) 在有向边(V0, V1)的左边
			// E(x,y) = 0 :点(x, y) 在有向边(V0, V1)上面
			if (fE1 <= 0 && fE2 <= 0)							// 当前像素点在三角形内
			{
				newPoint->bIsInside = true;

				if (m_tType == WANGTEX)
					CalcTexCoordWANG( newPoint);

				Rasterization(dir, unit, newPoint,allPixels,x-unit, y ,sortedV);	
			}
			else													// 当前像素点不在三角形内
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
				// 记录本行第一个无效点
			{
				allPixels->firstInvalidate = newPoint;
				return;
			}
			else if (Ttri == 0x01)
				// 继续向右遍历
			{
				Rasterization(dir, unit, newPoint, allPixels,x+unit, y,sortedV);
			}
			else if (Ttri == 0x00)
				// 在三角形内则记录当前点
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
			// E(x,y) > 0 :点(x, y) 在有向边(V0, V1)的右边
			// E(x,y) < 0 :点(x, y) 在有向边(V0, V1)的左边
			// E(x,y) = 0 :点(x, y) 在有向边(V0, V1)上面
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
// 从上到下排列三角形的三个顶点
// @GLdouble v1x, GLdouble v1y,GLdouble v2x,GLdouble v2y,GLdouble v3x,GLdouble v3y:待排序的三个顶点坐标
// @GLdouble sortedV[6]:输出，从上到下的顺序
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
// 三角形栅格化函数，通过调用Rasterization函数完成功能
// ***KIM栅格化过程：
// 从最上面的顶点开始,逐层按照算法遍历像素,找到三角形内的像素点,并对其做标记.换行的时候同时计算下一行对应像素的纹理坐标
// 1 如果当前的Ttri为00，证明当前点在三角形内，需要遍历这个点的左右两边，直至Ttri不为11。右边到头以后下移一行；
// 2 如果当前点的Ttri为10，向左遍历，直到Ttri为01或者11，然后下移一行；
// 3 如果当前点的Ttri为01，向右遍历，直到Ttri为10或者11，然后下一行；
// 4 如果当前点的Ttri为11，结束栅格过程（文中当Ttri为11时，直接下移一行，结束的条件是：当没有点绘制）
// ***WANG栅格化过程：
// 首先按照y方向顺序对三个定点的屏幕坐标进行排序，最上顶点和最下顶点组成的边（起始边）在每行上的像素作为每行的出发顶点
// 按照中间顶点和这个边的关系决定每行是向左还是向右遍历（如果中间点在起始边的右边，则向右遍历，反之向左）直到出界
// 判断出界的条件是当前像素和除起始边外其他两条边的关系（而不是求Ttri，这样省去了至少1/3的计算量）

// @GLdouble v1x, GLdouble v1y,GLdouble v2x,GLdouble v2y,GLdouble v3x,GLdouble v3y:三角形的三个顶点坐标。必须按照顺指针顺序传入
// @texMatrix[9]：纹理映射矩阵
// @m：纹理坐标精度控制
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

	DIR Vdir = DOWN, Hdir = LEFT;		// 栅格化过程的纵向和横向移动方向
	if (m_rType == WANG)					// 如果是WANG模式的话，需要按照分隔边的斜率以及中间顶点相对于分隔边的位置决定栅格化顺序
	{
		double fE = E(fMidx, fMidy, fBotx, fBoty, fTopx, fTopy);
		// E(x,y) > 0 :点(x, y) 在有向边(V0, V1)的右边
		// E(x,y) < 0 :点(x, y) 在有向边(V0, V1)的左边
		// E(x,y) = 0 :点(x, y) 在有向边(V0, V1)上面
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
	}									// 如果是KIM模式的话，找到最上面的顶点，从上往下的顺序栅格化
	else if (m_rType == KIM)
		Vdir = DOWN;

	RasteredPoints *RPoint = new RasteredPoints;

	// 初始化这个点的所有和纹理坐标计算相关的变量值
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
	RPoint->bIsInside = true;			// 标记当前点为三角形内,是栅格点

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

	// 将这个像素点插入栅格点列表中
	RPoint->next = NULL;
	RPoint->firstInvalidate = RPoint;
	m_pRasteredPoints = RPoint;
	m_pRasteredPoints->last = RPoint;

	RasteredPoints *pPreRowPixel = RPoint;

	int Ttri = 0;
	int bRastered = true;

	do
	{
		// 初始化一个新的栅格点,处于上一个栅格点的下一行,同一列
		RasteredPoints *pCurPoint = new RasteredPoints;

		pCurPoint->next = NULL;
		pCurPoint->bIsInside = false;
		m_pRasteredPoints->firstValidate = NULL;
		// 将新创建的栅格点插入栅格点列表中
		m_pRasteredPoints->last->next = pCurPoint;
		m_pRasteredPoints->last = pCurPoint;

		pCurPoint->x = pPreRowPixel->x;
		if (Vdir == DOWN)
			pCurPoint->y = pPreRowPixel->y - UNIT;
		else 
			pCurPoint->y = pPreRowPixel->y + UNIT;

		if (m_tType == KIMTEX)
			// 利用上一行的栅格点计算新建栅格点的纹理坐标值
			CalcTexCoord(m_texMatrix, pCurPoint, pPreRowPixel, Vdir, m_mBit, m_bPerPixelDiv);

		// 求得新建的栅格点的编码
		Ttri = GetTtri(pCurPoint->x, pCurPoint->y, m_pScreenV[0], m_pScreenV[1], m_pScreenV[2], m_pScreenV[3], m_pScreenV[4], m_pScreenV[5]);

		if (m_rType == KIM)			// KIM模式进行栅格化
		{
			if (Ttri == 0x00)
				// 当前点在三角形内，记录当前点，然后向左、向右遍历，直到出三角形外
			{
				pCurPoint->bIsInside = true;
				if (m_tType == WANGTEX)
					CalcTexCoordWANG(pCurPoint);

				Rasterization(LEFT, UNIT, pCurPoint, m_pRasteredPoints, pCurPoint->x - UNIT, pCurPoint->y, sortedV);
				Rasterization(RIGHT, UNIT, pCurPoint, m_pRasteredPoints, pCurPoint->x + UNIT, pCurPoint->y,sortedV);
			}
			else if (Ttri == 0x01)
				// 当前点在三角形左边，需要向右遍历
			{
				Rasterization(RIGHT, UNIT, pCurPoint, m_pRasteredPoints,pCurPoint->x + UNIT, pCurPoint->y, sortedV);
			}

			else if(Ttri == 0x02)
				// 当前点在三角形右边，需要向左遍历
			{
				Rasterization(LEFT, UNIT, pCurPoint, m_pRasteredPoints,pCurPoint->x - UNIT, pCurPoint->y, sortedV);
			}
			else if (Ttri == 0x03 )
				// 当前点已经处于三角形右下角，结束
			{
				bRastered = false;
				continue;
			}
			if (pCurPoint->y < fBoty)	// 防止死循环，有可能当前点在三角形左下角
			{
				bRastered = false;
				continue;
			}
			// 下移一行，同时计算下面一行像素点纹理坐标相关的参数
			pPreRowPixel = m_pRasteredPoints->firstInvalidate;
		}
		else if (m_rType == WANG)		// WANG模式进行栅格化
		{
			if (Ttri == 0x00)		// 移动一行后的像素点在三角形内
			{
				pCurPoint->bIsInside = true;
				if (m_tType == WANGTEX)
					CalcTexCoordWANG(pCurPoint);

				m_pRasteredPoints->firstValidate = pCurPoint;
			}	
			else if (Ttri == 0x03 )	// 已经没有像素点处于三角形内
				// 当前点已经处于三角形右下角，结束
			{
				bRastered = false;
				continue;
			}
			// 依据栅格化方向决定下一个点的x坐标
			int nextX = (Hdir == LEFT)? (pCurPoint->x - UNIT):(pCurPoint->x + UNIT);
			// 进入循环，栅格化本行的像素
			Rasterization(Hdir, UNIT, pCurPoint, m_pRasteredPoints, nextX, pCurPoint->y, sortedV);
			// 取得这一行处于三角形内的第一个像素
			pPreRowPixel = m_pRasteredPoints->firstValidate;

			// 如果本行没有像素处于三角形内，则栅格化完成
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

//-----------------------纹理映射相关函数---------------------//

//////////////////////////////////////////////////////////////////////////
// 纹理坐标求解函数
// 利用相邻像素点的信息迭代求取当前像素点的纹理坐标值：
// 实现原理(分为求取纹理坐标整数部分和小数部分):
// 整数部分:
// d(x, y) = 2(Rx + Sy + T)
// -d(xi, yi) < E(xi, yi, ui) <= 0
// (1)求解u
// E'(x, y, u) = ud(x, y) - 2*(Kx + Ly + M)
// A'(u) = 2uR - 2K
// B'(u) = 2uS - 2L
// 当x前进1的时候
// d(x+1, y) = d(x, y) + 2R
// E'(x+1, y, u) = E'(x, y, u) + A'(u)
// 当y前进1的时候
// d(x, y+1) = d(x, y) + 2S
// E'(x, y+1, u) = E'(x, y, u) + B'(u)
// 当u前进1的时候
// E'(x, y, u+1) = E'(x, y, u) + d(x, y)
// A'(u + 1) = A'(u) + 2R
// B'(u + 1) = B'(u) + 2S
// (2)求解v
// F'(x, y, v) = vd(x, y) - 2*(Nx +Py +Q)
// A'(v) = 2vR - 2N
// B'(v) = 2vs - 2P
// 当x前进1的时候
// d(x+1, y) = d(x, y ) + 2R
// F'(x+1, y, v) = F'(x, y, u) + A'(v)
// 当y前进1的时候
// d(x, y+1) = d(x, y) + 2S
// F'(x, y+1, v) = F'(x, y, v) + B'(v)
// 当v前进1的时候
// F'(x, y, v+1) = F'(x, y, v) + d(x, y)
// A'(v + 1) = A'(v) + 2R
// B'(v + 1) = B'(v) + 2S
// 小数部分:
// 算法伪代码如下：
// En = thisPixel->E;
// for (int i = 1; i <= m; i++)
// {
// 		if (En <= 0 && En > (d/pow(2.0, i)))    
// 			第1位不保留
// 		else if (En <= (-thisPixel->d/pow(2.0, i)) && En > (-thisPixel->d/pow(2.0, i-1)))
// 			第i位保留
// 		En = En + thisPixel->d*kSetU[i]/pow(2.0f, i);	// 计算下一位的En值
// }

// @texMatrix[9]:矩阵
// @thisPixel,prePixel:当前点和前一个点的信息
// @moveDir:当前点相对前一个点的移动方向
// @m:纹理坐标计算的精度
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
	// 利用传入的相邻像素初始化当前像素的信息
	bool bFoundU = false;
	bool bFoundV = false;

	thisPixel->u = floor(prePixel->u);	// 计算整数部分
	thisPixel->v = floor(prePixel->v);
	
	thisPixel->Au = prePixel->Au;
	thisPixel->Bu = prePixel->Bu;
	thisPixel->Av = prePixel->Av;
	thisPixel->Bv = prePixel->Bv;
	thisPixel->d = prePixel->d ;
	thisPixel->E = prePixel->E;
	thisPixel->F = prePixel->F;

	// 利用传入的相邻像素和当前像素的位置关系求得某些信息的值（这些信息不需要重新计算，只需要做加法即可）
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

	// 求得像素纹理坐标的整数部分
	if (thisPixel->d > 0)		// 如果d大于0的话
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
	else					// 如果d小于0
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


	// 求得像素纹理坐标的小数部分，精度是m
	double uFraction = 0.0;
	double vFraction = 0.0;
	double En = thisPixel->E;
	double Fn = thisPixel->F;
	int kSetU = 0;
	int kSetV = 0;

	// 避免使用除法操作
	double fFactor = 0.5;
	double fFactorUp = 1;
	double fFactorDown = 1;
	if (thisPixel->d > 0)
	{
		for (int i = 1; i <= m; i++)
		{
			fFactorUp *= fFactor;

			// 计算u
			if (En <= 0 && En > (-thisPixel->d*fFactorUp))
				kSetU = 0;
			else if (En <= (-thisPixel->d*fFactorUp) && En > (-thisPixel->d*fFactorDown))
				kSetU = 1;
			else					// 错误情况
				kSetU = 0;
			En = En + thisPixel->d*kSetU*fFactorUp;		// 计算i精度下的E
			uFraction += kSetU*fFactorUp;
			// 计算v
			if (Fn <= 0 && Fn > (-thisPixel->d*fFactorUp))
				kSetV = 0;
			else if (Fn <= (-thisPixel->d*fFactorUp) && Fn > (-thisPixel->d*fFactorDown))
				kSetV = 1;
			else					// 错误情况
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

			// 计算u
			if (En >= 0 && En < (-thisPixel->d*fFactorUp))
				kSetU = 0;
			else if (En >= (-thisPixel->d*fFactorUp) && En < (-thisPixel->d*fFactorDown))
				kSetU = 1;
			else					// 错误情况
				kSetU = 0;
			En = En + thisPixel->d*kSetU*fFactorUp;
			uFraction += kSetU*fFactorUp;
			// 计算v
			if (Fn >= 0 && Fn < (-thisPixel->d*fFactorUp))
				kSetV = 0;
			else if (Fn >= (-thisPixel->d*fFactorUp) && Fn < (-thisPixel->d*fFactorDown))
				kSetV = 1;
			else					// 错误情况
				kSetV = 0;
			Fn = Fn + thisPixel->d*kSetV*fFactorUp;
			vFraction += kSetV*fFactorUp;

			fFactorDown *= fFactor;

		}
	}
	// 得到估算的纹理坐标值
	thisPixel->u += uFraction; 
	thisPixel->v += vFraction;
}

//////////////////////////////////////////////////////////////////////////
// 计算纹理映射矩阵M
// 利用屏幕坐标（x, y），得到同质坐标(x', y', w')，然后利用各个顶点的纹理坐标计算得到纹理映射矩阵
	// (x, y) = (x' / w', y' / w')

	// | u | = | K  L  M | |x'|
	// | v | = | N  P  Q | |y'|
	// | 1 | = | R  S  T | |w'|
	// 由于三角形顶点处的纹理坐标为已知，所以可以求得矩阵各个元素的值
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

// 利用纹理坐标索引到纹理颜色值。使用距离作为权重，在四个相邻纹理像素求和
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
		// 得到当前像素点周围相邻的四个像素的索引
		nIndex11 = (uFloor + vFloor * tex->width)*nbpp;
		nIndex12 = (uFloor + vNext* tex->width)*nbpp;
		nIndex21 = (uNext + vFloor* tex->width )*nbpp;
		nIndex22 = (uNext + vNext * tex->width)*nbpp;

		// 利用双线性滤波得到对应的纹理像素值
		for(int i = 0; i < nbpp; i++)
		{
			point->color[i] = unsigned int((tex->imageData[nIndex11+i] * (1-uWeight) + tex->imageData[nIndex21+i]*uWeight)*(1-vWeight)
				+(tex->imageData[nIndex12+i]*(1-uWeight) + tex->imageData[nIndex22+i]*uWeight)*vWeight + 0.5f);
		}
		// 寻找下一个栅格点
		point = point->next;
	}
}

//----------------坐标变换相关函数------------------------//

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

// gluProject source code (说明见OpenGL API文档)   
// 韩红雷修改：winz不再是规整后的深度值，而是投影完成后齐次坐标的w值
GLint  CRaster::Project(GLdouble objx, GLdouble objy, GLdouble objz, const GLdouble  modelMatrix[16], const GLdouble projMatrix[16], const GLint viewport[4], GLdouble *winx, GLdouble *winy, GLdouble *winz)   
{   
	// matrice transformation   
	GLdouble in[4], out[4];   
	//initialize matrice and column vector as a transformer   
	in[0] = objx;   
	in[1] = objy;   
	in[2] = objz;   
	in[3] = 1.0;   
	transform_point(out, modelMatrix, in);  //乘以模型视图矩阵   
	transform_point(in, projMatrix, out);   //乘以投影矩阵   
	//齐次向量的第四项不能为0   
	if(in[3] == 0.0)   
		return GL_FALSE;   
	//向量齐次化标准化   
	in[0] /= in[3];   
	in[1] /= in[3];   
	in[2] /= in[3];   
	//视口向量的作用   
	*winx = viewport[0] + (1 + in[0]) * viewport[2] / 2;   
	*winy = viewport[1] + (1 + in[1]) * viewport[3] / 2;   
	//*winz = (1 + in[2]) / 2;   

	// 韩红雷测试 [1/14/2010 Leo Han]
	*winz = in[3];

	return GL_TRUE;   
}  

//////////////////////////////////////////////////////////////////////////
// 从三维空间投影到二维坐标上的自行计算函数(非调用gluProject)
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
// 从二维坐标映射到三维空间
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
// 从三维空间投影到二维坐标上
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

// 得到世界坐标中的某个点在屏幕上的投影
double CRaster::GetScreenCoord(double worldCoord[3], int screenCoord[2])
{
	double h;
	CustomProject/*ViewProject*/(worldCoord, screenCoord[0], screenCoord[1], h);
	return h;
}

//////////////////////////////////////////////////////////////////////////
// 将三个顶点按照顺时针顺序排列
bool CRaster::ClockWizeTriangle(int V1[2], int V2[2], int V3[2])
{
	int nE = E(V3[0], V3[1], V1[0], V1[1],	V2[0],	V2[1]);
	if (nE < 0)				// 第三个点在边V12的左边，是逆时针，需要调整为顺时针
	{
		int temp = V3[0];
		V3[0] = V2[0];
		V2[0] = temp;

		temp = V3[1];
		V3[1] = V2[1];
		V2[1] = temp;
	}
	else if (nE == 0)		// 不符合规定，需要删除
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 将三角形的三个顶点按照顺时针排序
//bool CRaster::ClockWizeTriangle(double V1[2], double V2[2], double V3[2])
//{
//	int nE = E(V3[0], V3[1], V1[0], V1[1],	V2[0],	V2[1]);
//	if (nE < 0)				// 第三个点在边V12的左边，是逆时针，需要调整为顺时针
//	{
//		double temp = V3[0];
//		V3[0] = V2[0];
//		V2[0] = temp;
//
//		temp = V3[1];
//		V3[1] = V2[1];
//		V2[1] = temp;
//	}
//	else if (nE == 0)		// 不符合规定，需要删除
//		return false;
//
//	return true;
//}

//----------------类相关函数------------------------//

//////////////////////////////////////////////////////////////////////////
// 释放栅格点列表allPoints所占的空间
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
// 外部调用的函数。返回栅格化以后的像素点序列
RasteredPoints*  CRaster::GetRasteredPoints()
{
	return m_pRasteredPoints;
}

//////////////////////////////////////////////////////////////////////////
// 用于WANG纹理坐标插值算法的函数

//////////////////////////////////////////////////////////////////////////
// 对一个向量进行单位化
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
// 计算两个向量叉乘
void  crossp(double v1[3], double v2[3], double result[3])
{

	result[0] = v1[1]*v2[2] - v1[2]*v2[1];
	result[1] = v1[2]*v2[0] - v1[0]*v2[2];
	result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

//////////////////////////////////////////////////////////////////////////
// 计算两个向量的点乘
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
// 将世界坐标转换到观察坐标系下
// 前面的9个参数和gluLookAt函数的参数相同
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

	// 得到世界坐标到观察坐标的变换矩阵
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
	// 向量和矩阵相乘
	pointViewCoord[0] =    
		m[0][0] * in[0] + m[0][1] * in[1] + m[0][2] * in[2] + m[0][3] * in[3];   
	pointViewCoord[1] =   
		m[1][0] * in[0] + m[1][1] * in[1] + m[1][2] * in[2] + m[1][3] * in[3];   
	pointViewCoord[2] =   
		m[2][0] * in[0] + m[2][1] * in[1] + m[2][2] * in[2] + m[2][3] * in[3]; 
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 得到三角形所在平面转换到观察坐标系后，和xy平面相交的直线的斜率
double CRaster::GetTriangleGradientInViewCoord(double worldV[9]
, double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz)
{
	double viewV[9];
	for (int i = 0; i < 3; i++)
	{
		TransToViewCoord(eyex,eyey, eyez,centerx, centery, centerz, upx, upy, upz,
			worldV+i*3, viewV+i*3);
	}
	// 求解斜率
	// 斜率是[(y2-y1)*(z3-z1)-(y3-y1)*(z2-z1)]/[(x2-x1)*(z3-z1)-(x3-x1)*(z2-z1)]

	return ((viewV[4]-viewV[1])*(viewV[8]-viewV[2])-(viewV[7]-viewV[1])*(viewV[5]-viewV[2]))
		/ ((viewV[3]-viewV[0])*(viewV[8]-viewV[2])-(viewV[6]-viewV[0])*(viewV[5]-viewV[2]));
}

//////////////////////////////////////////////////////////////////////////
// 利用线段扫描转换算法的得到三角形所在平面转换到观察坐标系后，和xy平面相交的直线的扫描转换结果
int* CRaster::GetLinePattern(GLdouble triangleVertex[9], double view[9], int &nPoints, double &fGradient)
{
	fGradient = GetTriangleGradientInViewCoord(triangleVertex, view[0], view[1], view[2], view[3], view[4], view[5], view[6], view[7], view[8]);
	GLint   triangleScreenVertex[6];
	GetScreenCoord(triangleVertex, triangleScreenVertex);
	GetScreenCoord(triangleVertex+3, triangleScreenVertex+2);
	GetScreenCoord(triangleVertex+6, triangleScreenVertex+4);
	// 求得三角形在屏幕上投影的包围盒
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
	// 按照斜向插值边的斜率确定线段的栅格化模式,如果小于1的话,证明是按照x方向为主动变量,模式列表存储对应的y值,反之则反
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
// 利用中点算法求线段的栅格化结果
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

// 求线性插值结果
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

		// 如果不进行线性误差修正的话，需要找到离当前像素点最近的斜向扫描行的下标
		if (!pWang->bLinearReduceError && abs(pWang->pStep[i]) > 0.5)
			pWang->pNearLine[i] = abs(nIndex)+1;
		else
			pWang->pNearLine[i] = abs(nIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// 计算扫描行起点数组
// @nRasterLine：指名多出了几条扫描线，以便减少误差
RasterLineHead** CRaster::CalcBoundBoxData()
{
	int sortedV[6];
	SortVertex(m_pScreenV[0],m_pScreenV[1],m_pScreenV[2],m_pScreenV[3],m_pScreenV[4],m_pScreenV[5], sortedV);
	// 按顺时针,从上到下的顺序排列三个顶点
	ClockWizeTriangle(sortedV, sortedV+2, sortedV+4);
	// 求得三角形的包围盒
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
	int nStartX, nStartY;			// 斜向扫描行的起始点坐标.纵向扫描在包围盒的左侧边,横向扫描在包围盒的下边
	int nEndX, nEndY;
	int step = 100;
	if (abs(m_pWang->fGradient) > 1.0)
	{
		m_pWang->dir = RIGHT;
		step = (m_pWang->fGradient > 0)?100:-100;

		// 找到斜向扫描边所经过的顶点,其他所有顶点应该都在这条边的右边

		// 斜向扫描行过第一个顶点
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

		// 斜向扫描行过第二个顶点
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

		// 斜向扫描行过第三个顶点
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
		// 所需的扫描行个数
		m_pWang->nBoundNum = nEndX - nStartX+1;	
	}
	else
	{
		m_pWang->dir = UP;
		// 斜向扫描行过第一个顶点
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

		// 斜向扫描行过第二个顶点
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

		// 斜向扫描行过第三个顶点
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
			// 使用ustep和vstep来表示x和y值，因为这两个值是浮点类型
			pHead->uStep = nStartX;
			pHead->vStep = nStartY + nLine*m_pWang->fRasterLineGap;

			pTail->uStep = nR;
			pTail->vStep = (nStartY + (nR-nL) * m_pWang->fGradient) + nLine*m_pWang->fRasterLineGap;
		}
		else if (m_pWang->dir == RIGHT)
		{
			// 使用ustep和vstep来表示x和y值，因为这两个值是浮点类型
			pHead->vStep = nStartY;
			pHead->uStep = nStartX + nLine*m_pWang->fRasterLineGap;

			pTail->vStep = nT;
			pTail->uStep = (nStartX + (nT-nB) * fGradD) + nLine*m_pWang->fRasterLineGap;
		}
		// 计算每个斜向扫描行的纹理坐标
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
				// 计算每个斜向扫描行的纹理坐标
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
// 利用纹理插值计算当前栅格点的纹理坐标
// @*pLinePattern:一定斜率的线段的栅格化模式,以(0,0)点位起始点,每个元素存储纵坐标(当斜率绝对值大于1的时候存储的是横坐标)
// @*thisPoint:待求解的像素对象
// @*Bound:保存了斜向扫描行的每个扫描行起始点的信息(纹理坐标以及纹理坐标步长)
// @nPoints:栅格化模式数组的长度
// @dir:扫描方向,有两个:向上或者向右(按照投影直线的斜率划分,斜率绝对值小于1为前者,否者为后者)
inline void CRaster::CalcTexCoordWANG(RasteredPoints *thisPoint)
{
	int nStartX = int(m_pWang->pBound[0][0].x);
	int nStartY = int(m_pWang->pBound[0][0].y); 	
	int nNearLine = 0;		// 当前像素中心离得最近的扫描行编号(每个像素内的扫描行编号从0开始,依次1,2直到扫描行数)
	int nIndex = 0;			// 指明相邻扫描行数组的下标
	int nNextLine = 0;		// 另一个相邻扫描行编号
	int nNextIndex = 0;		// 另一个相邻扫描行数组的下标
	int nOffset = 0;		// 当前像素的位置(用于计算线性插值的t)

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
	// 如果要进行纹理误差线性调整的话，需要计算相邻扫描线的信息
	if (m_pWang->bLinearReduceError)		
	{
		// 如果当前的误差小于0的话,证明当前像素在相邻扫描行的下面,需要调整
	// 计算当前像素两个相邻扫描行的信息,以便用于进行线性插值调整误差,或者找到最近的扫描行对应位置
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
			// 如果没有多增加扫描行
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
	// 如果误差小于1，并且相邻扫描线编号不为0的话，证明在扫描行的下面，调整相邻扫描行信息
	else if (m_pWang->pStep[nOffset] < 0 && nNearLine != 0)
	{
		nNearLine = m_pWang->nRasterLine - nNearLine+1;
		nIndex--; 
	}

	thisPoint->u = m_pWang->pBound[nNearLine][nIndex].u + m_pWang->pBound[nNearLine][nIndex].uStep*nOffset;
	thisPoint->v = m_pWang->pBound[nNearLine][nIndex].v + m_pWang->pBound[nNearLine][nIndex].vStep*nOffset;

	// 进行误差修正，不直接使用斜向扫描行的对应纹理坐标，而是利用当前屏幕像素位置作为线性插值的t，在相邻扫描行之间进行线性插值得到当前像素的纹理坐标
	// 虽然并不是线性关系，但可以近似看做线性关系
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

// 得到用于纹理坐标计算的数据，比如斜向扫描行的斜率等
WangTexData* CRaster::GetWangTexData(void)
{
	return m_pWang;
}