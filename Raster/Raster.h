#pragma once

#include <gl/GL.h>
#include "..\gl\Texture.h"

enum DIR{LEFT, RIGHT, DOWN, UP, SELF};
enum RASTERTYPE{KIM, WANG};
enum CALTEXCOORDTYPE{KIMTEX, WANGTEX};

typedef struct RasteredPointsStruct
{
	int x;									// 栅格点的坐标
	int y;

	bool bIsInside;							// 指名当前像素点是否在三角形内

	double u;								// 栅格点的纹理坐标，纹理坐标计算方法采取文章提出的透视正确的近似算法
	double v;
	double d;
	double E;
	double F;
	double Au;								
	double Av;
	double Bu;
	double Bv;

	RasteredPointsStruct *next;				// 下一个栅格点
	RasteredPointsStruct *last;				// 指向栅格点序列尾结点
	RasteredPointsStruct *firstInvalidate;	// 特殊指针，用于保存当前栅格行的第一个无效点
	RasteredPointsStruct *firstValidate;	// 特殊指正，用于Wang栅格化方式中，记录本行第一个有效的像素点

	GLubyte color[4];						// 该像素点的颜色值

	double uStep, vStep;					// 在WANG算法下计算纹理坐标的时候,存储线性插值的步长
}RasteredPoints;

typedef struct VertexStruct
{
	double x, y, z, w;									// 栅格点的坐标
	double u, v;
}Vertex;

typedef struct RasterLineHeadStruct
{
	double x, y;
	double u, v;
	double uStep, vStep;
}RasterLineHead;


typedef struct WangTexDataStruct
{
	int *pLinePattern;						// 斜向扫描行离散化后的坐标（当斜率小于1的时候只保存y坐标，否则只保存x坐标）
	int nPatternNum;						// 斜向扫描行离散化后的像素数
	RasterLineHead **pBound;				// 每个斜向扫描行起点组成的数组，其中包括纹理坐标以及线性插值步长
	int nBoundNum;							// 起点数组的长度
	DIR dir;								// 斜向扫描行的移动方向，当斜率小于1的时候方向向上，否则向右
	double fGradient;						// 斜向扫描行的斜率
	bool bLinearReduceError;				// 是否进行误差修正
	int nRasterLine;						// 每个像素单位增加的斜向扫描行数目。如果为0的话，证明只在每个像素处有扫描行，如果为1的话，每0.5个像素一个扫描行，以此类推
	double *pStep;							// 离散像素和最近的扫描行的距离占相邻扫描行距离的比例
	int *pNearLine;							// 离每个离散像素最近的扫描行下标(0为正常的像素处扫描行1为)
	double fRasterLineGap;					// 相邻扫描行的距离
}WangTexData;

class CRaster
{
public:
	CRaster(void);
	~CRaster(void);

	RasteredPoints*  GetRasteredPoints();
	double RasterCalTex(GLdouble triangleVertex[9], double vertexTexCoord[6], int m, double view[9],Texture *tex
		,  RASTERTYPE rasterType = KIM, bool bPerPixelDiv = false, CALTEXCOORDTYPE texType = KIMTEX, bool bLinearReduceError = false, int nRasterLine = 0);
	void CalcTexMatrix(GLdouble triangleVertex[9], double vertexTexCoord[6], double texMatrix[9]);
	double *CRaster::GetTexMatrix();

private:

	WangTexData *m_pWang;				// 用WANG算法计算纹理坐标的时候所需要的信息
	int m_pScreenV[6];					// 待扫描转换的三角形在屏幕上的坐标，逆时针存储
	double m_texMatrix[9];				// 带扫描转换的三角形的纹理映射矩阵
	int m_mBit;							// 使用KIM算法计算纹理坐标的时候的精度
	RASTERTYPE m_rType;					// 当前使用哪种栅格化算法
	bool m_bPerPixelDiv;				// 是否逐像素除法精确计算纹理坐标
	CALTEXCOORDTYPE m_tType;			// 当前纹理坐标计算使用哪个算法
	RasteredPoints *m_pRasteredPoints;	// 扫描转换后的三角形的像素序列


	// 普通函数
	void InitParams(int screenV[6],double texMatrix[9],int m, RASTERTYPE rType, bool bPerPixelDiv, CALTEXCOORDTYPE texType);
	void Release();
	void GetTexData(RasteredPoints *rasteredPoints, Texture *tex);
	double GetScreenCoord(double worldCoord[3], int screenCoord[2]);
	void SortVertex(int v1x, int v1y,int v2x,int v2y,int v3x,int v3y, int sortedV[6]);

	// 几何操作函数
	bool ClockWizeTriangle(int V1[2], int V2[2], int V3[2]);
	void ViewProject(double pos3D[3], int &xi, int &yi, double &depth);
	void ViewUnProject(int xi, int yi, double depth, double pos3D[3]);
	inline int E(int vx, int vy,int v0x,int v0y,int v1x,int v1y);
	inline int GetTtri(int vx, int vy, int v1x, int v1y,int v2x,int v2y,int v3x,int v3y);
	void transform_point(GLdouble out[4], const GLdouble m[16], const GLdouble in[4])   ;
	GLint Project(GLdouble objx, GLdouble objy, GLdouble objz, const GLdouble  modelMatrix[16], const GLdouble projMatrix[16], const GLint viewport[4]
	, GLdouble *winx, GLdouble *winy, GLdouble *winz)   ;
	void CustomProject(double pos3D[3], int &xi, int &yi, double &depth);

	// KIM栅格化及纹理坐标计算方法函数
	inline void CalcTexCoord(double texMatrix[9], RasteredPoints *thisPixel, RasteredPoints *prePixel, DIR moveDir, int m, bool bPerPixelDiv);
	void Rasterization(DIR dir, int unit, RasteredPoints *pPrePixel, RasteredPoints *allPixels, int x, int y,int *sortedV = NULL);
	void TriangleRasterization();

	// 关于WANG纹理坐标算法
	bool TransToViewCoord(double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz,
		double pointCoord[3], double pointViewCoord[3]);
	double GetTriangleGradientInViewCoord(double worldV[9]
	, double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz);
	int* GetLinePattern(GLdouble triangleVertex[9], double view[9], int &nPoints, double &fGradient);
	int* MidpointLine(int x0, int y0, double x1, double y1, int &nP, double *pError);
	inline void LinearInterTexCoord(double fSU, double fSV, double fStepU,double fStepV, int t, double &fU, double &fV);
	RasterLineHead** CalcBoundBoxData();
	inline void CalcTexCoordWANG(RasteredPoints *thisPoint);
	void LinearError(WangTexData *pWang);

public:
	// 得到用于纹理坐标计算的数据，比如斜向扫描行的斜率等
	WangTexData* GetWangTexData(void);
};
