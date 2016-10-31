#pragma once

#include <gl/GL.h>
#include "..\gl\Texture.h"

enum DIR{LEFT, RIGHT, DOWN, UP, SELF};
enum RASTERTYPE{KIM, WANG};
enum CALTEXCOORDTYPE{KIMTEX, WANGTEX};

typedef struct RasteredPointsStruct
{
	int x;									// դ��������
	int y;

	bool bIsInside;							// ָ����ǰ���ص��Ƿ�����������

	double u;								// դ�����������꣬����������㷽����ȡ���������͸����ȷ�Ľ����㷨
	double v;
	double d;
	double E;
	double F;
	double Au;								
	double Av;
	double Bu;
	double Bv;

	RasteredPointsStruct *next;				// ��һ��դ���
	RasteredPointsStruct *last;				// ָ��դ�������β���
	RasteredPointsStruct *firstInvalidate;	// ����ָ�룬���ڱ��浱ǰդ���еĵ�һ����Ч��
	RasteredPointsStruct *firstValidate;	// ����ָ��������Wangդ�񻯷�ʽ�У���¼���е�һ����Ч�����ص�

	GLubyte color[4];						// �����ص����ɫֵ

	double uStep, vStep;					// ��WANG�㷨�¼������������ʱ��,�洢���Բ�ֵ�Ĳ���
}RasteredPoints;

typedef struct VertexStruct
{
	double x, y, z, w;									// դ��������
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
	int *pLinePattern;						// б��ɨ������ɢ��������꣨��б��С��1��ʱ��ֻ����y���꣬����ֻ����x���꣩
	int nPatternNum;						// б��ɨ������ɢ�����������
	RasterLineHead **pBound;				// ÿ��б��ɨ���������ɵ����飬���а������������Լ����Բ�ֵ����
	int nBoundNum;							// �������ĳ���
	DIR dir;								// б��ɨ���е��ƶ����򣬵�б��С��1��ʱ�������ϣ���������
	double fGradient;						// б��ɨ���е�б��
	bool bLinearReduceError;				// �Ƿ�����������
	int nRasterLine;						// ÿ�����ص�λ���ӵ�б��ɨ������Ŀ�����Ϊ0�Ļ���֤��ֻ��ÿ�����ش���ɨ���У����Ϊ1�Ļ���ÿ0.5������һ��ɨ���У��Դ�����
	double *pStep;							// ��ɢ���غ������ɨ���еľ���ռ����ɨ���о���ı���
	int *pNearLine;							// ��ÿ����ɢ���������ɨ�����±�(0Ϊ���������ش�ɨ����1Ϊ)
	double fRasterLineGap;					// ����ɨ���еľ���
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

	WangTexData *m_pWang;				// ��WANG�㷨�������������ʱ������Ҫ����Ϣ
	int m_pScreenV[6];					// ��ɨ��ת��������������Ļ�ϵ����꣬��ʱ��洢
	double m_texMatrix[9];				// ��ɨ��ת���������ε�����ӳ�����
	int m_mBit;							// ʹ��KIM�㷨�������������ʱ��ľ���
	RASTERTYPE m_rType;					// ��ǰʹ������դ���㷨
	bool m_bPerPixelDiv;				// �Ƿ������س�����ȷ������������
	CALTEXCOORDTYPE m_tType;			// ��ǰ�����������ʹ���ĸ��㷨
	RasteredPoints *m_pRasteredPoints;	// ɨ��ת����������ε���������


	// ��ͨ����
	void InitParams(int screenV[6],double texMatrix[9],int m, RASTERTYPE rType, bool bPerPixelDiv, CALTEXCOORDTYPE texType);
	void Release();
	void GetTexData(RasteredPoints *rasteredPoints, Texture *tex);
	double GetScreenCoord(double worldCoord[3], int screenCoord[2]);
	void SortVertex(int v1x, int v1y,int v2x,int v2y,int v3x,int v3y, int sortedV[6]);

	// ���β�������
	bool ClockWizeTriangle(int V1[2], int V2[2], int V3[2]);
	void ViewProject(double pos3D[3], int &xi, int &yi, double &depth);
	void ViewUnProject(int xi, int yi, double depth, double pos3D[3]);
	inline int E(int vx, int vy,int v0x,int v0y,int v1x,int v1y);
	inline int GetTtri(int vx, int vy, int v1x, int v1y,int v2x,int v2y,int v3x,int v3y);
	void transform_point(GLdouble out[4], const GLdouble m[16], const GLdouble in[4])   ;
	GLint Project(GLdouble objx, GLdouble objy, GLdouble objz, const GLdouble  modelMatrix[16], const GLdouble projMatrix[16], const GLint viewport[4]
	, GLdouble *winx, GLdouble *winy, GLdouble *winz)   ;
	void CustomProject(double pos3D[3], int &xi, int &yi, double &depth);

	// KIMդ�񻯼�����������㷽������
	inline void CalcTexCoord(double texMatrix[9], RasteredPoints *thisPixel, RasteredPoints *prePixel, DIR moveDir, int m, bool bPerPixelDiv);
	void Rasterization(DIR dir, int unit, RasteredPoints *pPrePixel, RasteredPoints *allPixels, int x, int y,int *sortedV = NULL);
	void TriangleRasterization();

	// ����WANG���������㷨
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
	// �õ��������������������ݣ�����б��ɨ���е�б�ʵ�
	WangTexData* GetWangTexData(void);
};
