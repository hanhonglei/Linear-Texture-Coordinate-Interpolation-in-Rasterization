/*
* Copyright (c) 2005,DECA LAB
* All rights reserved.
* 文件名称：MainFrm.cpp
* 摘    要：注意我给出注释的地方
* 当前版本：1.0
* 作    者：DECA LAB:韩红雷
* 完成日期：2005年3月17日
*/  
/////////////////////////////////////////////////////////////////
// MyRasterView.cpp : implementation of the CMyRasterView class
//

#include "stdafx.h"
#include "MyRaster.h"
#include "VertexTune.h"

#include "MyRasterDoc.h"
#include "MyRasterView.h"
#include <math.h>
#include <fstream>
using namespace std;

ofstream myfile;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DOUBLE_BUFFER 1
int RENDER_TYPE = OPENGL_RENDER;
/////////////////////////////////////////////////////////////////////////////
// CMyRasterView

IMPLEMENT_DYNCREATE(CMyRasterView, CScrollView)

BEGIN_MESSAGE_MAP(CMyRasterView, CScrollView)
//{{AFX_MSG_MAP(CMyRasterView)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_DESTROY()
ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
ON_COMMAND(ID_CHANGE_VERTEX, ChangeVertex)

ON_WM_TIMER()
ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRasterView construction/destruction

CMyRasterView::CMyRasterView()
{
	// TODO: add construction code here
	// leo : 初始化
	m_pDC = NULL;
	m_nDrawPoints = 0;
	m_nLBDown = 0;
	m_bHaveTriangle = true;
	m_pRaster = new CRaster;
	RENDER_TYPE = OPENGL_RENDER;
	m_view[0] = 20;
	double pi = 3.1415926535;
	m_view[1] = double(pi / 2);
	m_view[2] = 0;
	m_viewCenter[0] = 0;
	m_viewCenter[1] = 0;
	m_viewCenter[2] = 0;
	m_bRefresh = false;
	m_renderTime = 0.0;
	m_rasterType = KIM;
	m_texType = WANGTEX;
	m_nFraction = 2;
	m_nRasterLine = 0;
	m_bPerPixelDiv = false;
	m_bReduceError = false;
	InitRandomVertex();

	myfile.open("OutPut.txt");
}

void CMyRasterView::GetVertexData(double vertex[9], double texCoord[6], RASTERTYPE &rType, int &fraction
								  , bool &bPerPixelDiv, CALTEXCOORDTYPE &tType, bool &bReduceError, int &nRasterLine)
{
	memcpy(vertex, m_WorldV, sizeof(double)*9);
	memcpy(texCoord, m_TextureCoord, sizeof(double)*6);
	rType = m_rasterType;
	fraction = m_nFraction;
	bPerPixelDiv = m_bPerPixelDiv;
	tType = m_texType;
	bReduceError = m_bReduceError;
	nRasterLine = m_nRasterLine;
}

void CMyRasterView::SetVertexData(double vertex[9], double texCoord[6], RASTERTYPE rType, int fraction
								  , bool bPerPixelDiv,  CALTEXCOORDTYPE tType, bool bReduceError, int nRasterLine)
{
	memcpy( m_WorldV, vertex,sizeof(double)*9);
	memcpy(m_TextureCoord, texCoord, sizeof(double)*6);
	if (rType != -1)
		m_rasterType = rType;
	
	m_nFraction = fraction;
	m_bPerPixelDiv = bPerPixelDiv;
	m_bRefresh = true;
	m_texType = tType;
	m_bReduceError = bReduceError;
	m_nRasterLine = nRasterLine;
	Invalidate(true);
}


void CMyRasterView::ChangeVertex()
{
	CVertexTune tune;
	if (tune.DoModal() == IDOK)
	{
		m_bRefresh = true;
		Invalidate(true);
	}
}

void CMyRasterView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	m_nDrawPoints++;
	Invalidate();

	CScrollView::OnTimer(nIDEvent);
}

void CMyRasterView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_LEFT:
		m_view[2] += 0.1f;
		m_bRefresh = true;
		break;
	case VK_RIGHT:
		m_view[2] -= 0.1f;
		m_bRefresh = true;
		break;
	case VK_UP:
		m_view[1] += 0.1f;
		m_bRefresh = true;
		break;
	case VK_DOWN:
		m_view[1] -= 0.1f;
		m_bRefresh = true;
		break;
	case VK_PRIOR:
		m_view[0] = (m_view[0]>3)?(m_view[0] - 2.0f) : 3.0f;
		m_bRefresh = true;
		break;
	case VK_NEXT:
		m_view[0] += 2.0f;
		m_bRefresh = true;
		break;
	case 'W':
		m_viewCenter[1] += 0.1f;
		m_bRefresh = true;
		break;
	case 'S':
		m_viewCenter[1] -= 0.1f;
		m_bRefresh = true;
		break;
	case 'A':
		m_viewCenter[0] -= 0.1f;
		m_bRefresh = true;
		break;
	case 'D':
		m_viewCenter[0] += 0.1f;
		m_bRefresh = true;
		break;
	default:
		break;
	}
	int nWinWidth  = m_wndRect.right-m_wndRect.left;
	int nWinHeight = m_wndRect.bottom-m_wndRect.top;
	if(nWinHeight == 0)
	{
		nWinHeight = 1;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 修改为依靠传入的参数决定透视坐标系
	gluPerspective(/*vertical field of view*/ 45.,
		/*aspect ratio*/ /*(double) viewport.width/viewport.height,*/(double)nWinWidth/nWinHeight,
		/*znear*/ .1, /*zfar*/ 50.);

	//glOrtho(0.0f, nWinWidth, 0.0f, nWinHeight, -1.0f, 1.0f);			// Create Ortho 640x480 View (0,0 At bottom Left)
	/* from here on we're setting modeling transformations */
	gluLookAt(m_view[0]*sin(m_view[1])*sin(m_view[2]), m_view[0]*cos(m_view[1]), m_view[0]*sin(m_view[1])*cos(m_view[2])
		, m_viewCenter[0],  m_viewCenter[1], m_viewCenter[2],0, 1, 0);



	glMatrixMode(GL_MODELVIEW);

	Invalidate();

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMyRasterView::InitRandomVertex()
{
	for (int i = 0; i < 9; i++)
	{
		m_WorldV[i] = 0;
	}
	m_WorldV[0] = -rand() % 10;
	m_WorldV[5] = -rand() % 20;
	m_WorldV[6] = rand() % 10;

	m_WorldV[1] = 5;
	m_WorldV[7] = -4;

	m_WorldV[3] = 5;

	m_TextureCoord[0] = 0.2f;
	m_TextureCoord[1] = 0.0f;
	m_TextureCoord[2] = 0.7f;
	m_TextureCoord[3] = 0.9f;
	m_TextureCoord[4] = 0.9f;
	m_TextureCoord[5] = 0.03f;
	m_nDrawPoints = 0;

}
CMyRasterView::~CMyRasterView()
{
	// 释放指针
	if (m_pDC != NULL)
	{
		delete m_pDC;
	}
	delete m_pRaster;
	myfile.close();
}


/////////////////////////////////////////////////////////////////////////////
// CMyRasterView drawing

void CMyRasterView::OnDraw(CDC* pDC)
{
	CMyRasterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC *pCurDC = pDC;
	// 进行清楚操作，避免产生图像残留
	if (RENDER_TYPE == OPENGL_RENDER)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
		glLoadIdentity();
		pDoc->DrawCoordinate();

		if (m_bHaveTriangle)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			pDoc->DrawTexturedTriangle(m_WorldV, m_WorldV+3, m_WorldV+6, m_TextureCoord);
		}

		SwapBuffers(wglGetCurrentDC());
	}
	else
	{
#ifdef DOUBLE_BUFFER
		CDC   *MemDC = new CDC;   //首先定义一个显示设备对象   
		CBitmap   MemBitmap;//定义一个位图对象   

		//随后建立与屏幕显示兼容的内存显示设备   
		MemDC->CreateCompatibleDC(NULL);   
		//这时还不能绘图，因为没有地方画   ^_^   
		//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小   
		MemBitmap.CreateCompatibleBitmap(pDC,m_wndRect.Width(),m_wndRect.Height());   

		//将位图选入到内存显示设备中   
		//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上   
		CBitmap   *pOldBit=MemDC->SelectObject(&MemBitmap);   

		//先用背景色将位图清除干净，这里我用的是白色作为背景   
		//你也可以用自己应该用的颜色   
		MemDC->FillSolidRect(0,0,m_wndRect.Width(),m_wndRect.Height(), RGB(255,255,255));
		pCurDC = MemDC;
		//pDC->SetPixel(tempPixel, RGB(255, 0, 0));
#endif	

		if (m_bRefresh)
		{
			m_lookArtPara[0] = m_view[0]*sin(m_view[1])*sin(m_view[2]);
			m_lookArtPara[1] = m_view[0]*cos(m_view[1]);
			m_lookArtPara[2] = m_view[0]*sin(m_view[1])*cos(m_view[2]);
			m_lookArtPara[3] =  m_viewCenter[0];
			m_lookArtPara[4] =  m_viewCenter[1];
			m_lookArtPara[5] =  m_viewCenter[2];
			m_lookArtPara[6] = 0;
			m_lookArtPara[7] = 1;
			m_lookArtPara[8] = 0;
			CalcTexAndWriteToFile(pCurDC, false);
			m_bRefresh = false;
		}
		else
		{

			RasteredPoints *p = m_pRaster->GetRasteredPoints();
			while (p != NULL /*&& nDrawPoints < m_nDrawPoints*/)
			{
				if (p->bIsInside)
				{
					if (pCurDC != NULL)
					{
						pCurDC->SetPixel(p->x,m_wndRect.Height()-p->y, RGB(p->color[0], p->color[1], p->color[2]));
					}
				}
				p = p->next;
			}
		}
		CString s;
		s.Format(_T("自行栅格化%s模式.纹理计算采用%s模式. 栅格化所用时间:%f毫秒.")
			, (m_rasterType == KIM)?_T("KIM"):_T("WANG"),(m_texType == KIMTEX)?_T("KIMTEX"):_T("WANGTEX"),m_renderTime);

		pCurDC->TextOut(10, 10, s);

#ifdef DOUBLE_BUFFER
		//将内存中的图拷贝到屏幕上进行显示   
		pDC->BitBlt(0,0,m_wndRect.Width(),m_wndRect.Height(),MemDC,0,0,SRCCOPY);   

		//绘图完成后的清理   
		MemBitmap.DeleteObject();   
		MemDC->DeleteDC();   
		delete MemDC;
#endif
	}
}

// temp
double fErrorUGlobal, fErrorVGlobal;


// 自动使用各种方案进行纹理插值计算
void CMyRasterView::AutoCalcTexData()
{
	myfile << "\n★ 方案1：使用WANG算法，逐像素插值，不采用线性插值调整误差\n";
	for (int nRasterLine = 0; nRasterLine < 5; nRasterLine++)
	{
		SetVertexData(m_WorldV, m_TextureCoord, KIM, m_nFraction, true, WANGTEX, false, nRasterLine);
		CalcTexAndWriteToFile();
		if (nRasterLine == 0)
		{
			double tmp = 0.5;
			int nFraction = 0;
			while (tmp > fErrorUGlobal && tmp > fErrorVGlobal)
			{
				tmp *= 0.5;		
				nFraction++;
			}
			nFraction = (nFraction==0)?0:(nFraction-1);
			SetVertexData(m_WorldV, m_TextureCoord, KIM, nFraction, false, KIMTEX, false, m_nRasterLine);
			CalcTexAndWriteToFile();
		}
	}
	myfile << "\n★ 方案2：使用WANG算法，逐像素插值，并采用线性插值调整误差\n";
	for (int nRasterLine = 0; nRasterLine < 5; nRasterLine++)
	{
		SetVertexData(m_WorldV, m_TextureCoord, KIM, m_nFraction, true, WANGTEX, true, nRasterLine);
		CalcTexAndWriteToFile();
		if (nRasterLine == 0)
		{
			double tmp = 0.5;
			int nFraction = 0;
			while (tmp > fErrorUGlobal && tmp > fErrorVGlobal)
			{
				tmp *= 0.5;		
				nFraction++;
			}
			nFraction = (nFraction==0)?0:(nFraction-1);
			SetVertexData(m_WorldV, m_TextureCoord, KIM, nFraction, false, KIMTEX, false, m_nRasterLine);
			CalcTexAndWriteToFile();
		}
	}
	//myfile << "\n★ 方案3：使用WANG算法，使用KIM计算边界像素纹理(误差等级m=4)，不采用线性插值调整误差\n";
	//for (int nRasterLine = 0; nRasterLine < 5; nRasterLine++)
	//{
	//	SetVertexData(m_WorldV, m_TextureCoord, KIM, 3, false, WANGTEX, false, nRasterLine);
	//	CalcTexAndWriteToFile();
	//}
	//myfile << "\n★ 方案4：使用WANG算法，使用KIM计算边界像素纹理(误差等级m=4)，并采用线性插值调整误差\n";
	//for (int nRasterLine = 0; nRasterLine < 5; nRasterLine++)
	//{
	//	SetVertexData(m_WorldV, m_TextureCoord, KIM, 3, false, WANGTEX, true, nRasterLine);
	//	CalcTexAndWriteToFile();
	//}

	myfile << "\n★ 方案5：使用KIM算法\n";
	for (int nFraction = 0; nFraction < 5; nFraction++)
	{
		SetVertexData(m_WorldV, m_TextureCoord, KIM, nFraction, false, KIMTEX, false, m_nRasterLine);
		CalcTexAndWriteToFile();
	}
}

void CMyRasterView::CalcTexAndWriteToFile(CDC* pDC, bool bWrite)
{
	CMyRasterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	static bool bOutPut = false;
	int nDrawPoints = 0;
	int nTotalPoints = 0;
	//////////////////////////////////////////////////////////////////////////
	// 求取每个像素点的真实纹理坐标，使用逐像素除法计算.计算
	double *texMatrix;	
	int nLarger1 = 0;
	int nLarger05 = 0;
	int nLarger005 = 0;
	double fErrorU = 0.0;
	double fErrorV = 0.0;
	double fMaxErrorU = 0.0, fMaxErrorV = 0.0;

	int nEU = 0, nEV = 0;
	int nEU01 = 0, nEV01 = 0;
	double fEU, fEV;

	if (m_bHaveTriangle)
	{
		//////////////////////////////////////////////////////////////////////////
		// 进行栅格化
		m_renderTime = m_pRaster->RasterCalTex(m_WorldV, m_TextureCoord, m_nFraction, m_lookArtPara, (pDoc->m_isTexLoaded)?(&(pDoc->m_texture)):NULL
			, m_rasterType, m_bPerPixelDiv, m_texType, m_bReduceError,  m_nRasterLine);
		bOutPut = false;
		texMatrix = m_pRaster->GetTexMatrix();
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

		RasteredPoints *p = m_pRaster->GetRasteredPoints();
		while (p != NULL /*&& nDrawPoints < m_nDrawPoints*/)
		{
			if (p->bIsInside)
			{
				if (pDC != NULL)
				{
					pDC->SetPixel(p->x,m_wndRect.Height()-p->y, RGB(p->color[0], p->color[1], p->color[2]));
				}
				nDrawPoints++;

				double uCor = (K*p->x + L*p->y + M)/(R*p->x + S*p->y + T);
				fEU = abs(uCor - p->u);
				fErrorU += fEU;
				if (fEU > fMaxErrorU)
					fMaxErrorU = fEU;

				double vCor = (N*p->x + P*p->y + Q)/(R*p->x + S*p->y + T);
				fEV = abs(vCor - p->v);
				fErrorV += fEV;
				if (fEV > fMaxErrorV)
					fMaxErrorV = fEV;
				if (fEU > 0.1)
				{
					nEU01++;
					if (fEU > 0.3 )
						nEU ++;
				}
				if (fEV > 0.1)
				{
					nEV01++;
					if (fEV > 0.3)
						nEV++;
				}
			}
			p = p->next;
			nTotalPoints++;
		}
	}
	if (!bOutPut && bWrite)
	{
		myfile << "*****************************\n\t";
		// 栅格化类型
		// 纹理坐标计算类型
		if (m_texType == KIMTEX)
			myfile << "KIMTEX";
		else
			myfile << "WANGTEX";
		if (m_rasterType == KIM)
			myfile << "\tKIM";
		else
			myfile << "\tWANG";
		if (m_bPerPixelDiv)
		{
			myfile << "\t,逐像素除法";
		}
		else
		{
			myfile << "\t纹理插值精度:";
			myfile<<m_nFraction;
		}

		if (m_texType == WANGTEX)
		{
			myfile << "\t扫描行斜率:";
			myfile << m_pRaster->GetWangTexData()->fGradient;
			myfile << "\t,增加的扫描行数:";
			myfile << m_nRasterLine;
			if (m_bReduceError)
			{
				myfile << "\t,调整误差";
			}
		}
		myfile<< "\n\t";

		// 渲染的像素数
		myfile<< "有效像素数:" ;
		myfile<< nDrawPoints ;
		myfile<< "\t\t遍历像素数:";
		myfile<< nTotalPoints ;
		// 栅格化时间
		myfile<< "\t\t栅格化所用时间:";
		myfile<< m_renderTime;
		myfile << "毫秒";

		myfile << "\n\t";

		myfile << "----u方向和v方向平均误差:";
		myfile << fErrorU/nDrawPoints;
		myfile << ",";
		myfile << fErrorV/nDrawPoints;
		myfile << "\t\tu方向和v方向最大误差:";
		myfile << fMaxErrorU;
		myfile << ",";
		myfile << fMaxErrorV;

		myfile << " \n\t误差超过0.3，u和v的个数分别是：" << nEU<<","<<nEV;
		myfile << " \n\t误差超过0.1，u和v的个数分别是：" << nEU01<<","<<nEV01;

		myfile<< "\n";

		bOutPut = true;

		fErrorUGlobal = fErrorU/nDrawPoints;
		fErrorVGlobal = fErrorV/nDrawPoints;
		
	}
}

void CMyRasterView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	// 	CSize sizeTotal;
	// TODO: calculate the total size of this view
	// 	sizeTotal.cx = sizeTotal.cy = 100;
	// 	SetScrollSizes(MM_TEXT, sizeTotal);
	// 正确决定滚动条的范围
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
	
}

/////////////////////////////////////////////////////////////////////////////
// CMyRasterView message handlers

int CMyRasterView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// leo : 初始化opengl
	IniOpenGL();

	return 0;
}

void CMyRasterView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	// 调整视窗	
	// TODO: Add your message handler code here
    GetClientRect(&m_wndRect);
	
	int nWinWidth  = m_wndRect.right-m_wndRect.left;
	int nWinHeight = m_wndRect.bottom-m_wndRect.top;
	if(nWinHeight == 0)
	{
		nWinHeight = 1;
	}
	
	glViewport ( 0, 0, nWinWidth, nWinHeight );
	
    /* setup perspective m_camera with OpenGL */
    glMatrixMode(GL_PROJECTION);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glLoadIdentity();
// 修改为依靠传入的参数决定透视坐标系
    gluPerspective(/*vertical field of view*/ 45.,
		/*aspect ratio*/ /*(double) viewport.width/viewport.height,*/(double)nWinWidth/nWinHeight,
		/*znear*/ .1, /*zfar*/ 50.);
	
	//glOrtho(0.0f, nWinWidth, 0.0f, nWinHeight, -1.0f, 1.0f);			// Create Ortho 640x480 View (0,0 At bottom Left)
	/* from here on we're setting modeling transformations */
	gluLookAt(m_view[0]*sin(m_view[1])*sin(m_view[2]), m_view[0]*cos(m_view[1]), m_view[0]*sin(m_view[1])*cos(m_view[2])
	,  m_viewCenter[0], m_viewCenter[1], m_viewCenter[2], 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	// leo:使用后备缓存绘制，避免闪烁
	glDrawBuffer (GL_BACK);

	m_bRefresh = true;
}

void CMyRasterView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	// leo:应该添加这些释放资源的操作
	HGLRC   hrc;
	
	hrc = ::wglGetCurrentContext();
	
	::wglMakeCurrent(NULL,  NULL);
	
	if (hrc)
		::wglDeleteContext(hrc);	
	
}

// leo:不进行背景清除，以免绘制两次，产生闪烁
BOOL CMyRasterView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if (RENDER_TYPE == OPENGL_RENDER)
	{
		return TRUE;
	}
	else
	{
#ifdef DOUBLE_BUFFER
		return TRUE;
#else
		return CScrollView::OnEraseBkgnd(pDC);
#endif
	}

}

// leo : 初始化openggl绘图环境
void CMyRasterView::IniOpenGL()
{
	m_pDC = new CClientDC(this);
	PIXELFORMATDESCRIPTOR pfd;
	int n;
	//定义一个绘制上下文的句柄
	HGLRC   hrc;
	
	//初始化过程中主要就是初始化了一个客户区的设备环境指针
	ASSERT(m_pDC != NULL);
	
	//建立应用所需的像素格式，并与当前设备上下文相关连
	if (!bSetPixelFormat())
		return;
	
	//得到指定设备环境的象素模式索引
	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	
	//根据上面得到的索引值来声明一个象素模式
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	
	//创建一个上下文设备环境
	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	
	//将刚生成的设备上下文指针设为当前环境
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);		
	
	//置黑背景
	glClearColor(0.0,0.0,0.0,0.0);	
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
	glShadeModel(GL_SMOOTH);
	
	// 设置混色函数取得半透明效果
	//	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 	
	//	glEnable(GL_BLEND); 
	
	//平滑线条
    glEnable (GL_LINE_SMOOTH);
	//    glEnable (GL_BLEND);
	
	//初始化反走样为 RGBA 模式，同时包括 alpha 混合、提示的设置
	//		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//		glEnable (GL_BLEND);
	
	// 真正精细的透视修正
	glHint (GL_POLYGON_SMOOTH_HINT|GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	//充许深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); 
}

// leo ：设置适于OpenGL使用的像素格式
BOOL CMyRasterView::bSetPixelFormat()
{
	//定义一种像素格式
	static PIXELFORMATDESCRIPTOR pfd =
	{   
		sizeof(PIXELFORMATDESCRIPTOR),			// size of this pfd
			1,									// version number
			PFD_DRAW_TO_WINDOW |				// support window
			PFD_SUPPORT_OPENGL |				// support OpenGL  支持OpenGL
			PFD_DOUBLEBUFFER,					// double buffered 支持又缓冲
			PFD_TYPE_RGBA,						// RGBA type使用RGBA模式，不用调色板
			24,									// 24-bit color depth  使用24位真彩色
			0, 0, 0, 0, 0, 0,					// color bits ignored
			0,									// no alpha buffer
			0,									// shift bit ignored
			0,									// no accumulation buffer
			0, 0, 0, 0,							// accum bits ignored
			32,									// 32-bit z-buffer   32位Z轴缓冲
			0,									// no stencil buffer
			0,									// no auxiliary buffer
			PFD_MAIN_PLANE,						// main layer
			0,									// reserved
			0, 0, 0								// layer masks ignored
	};
	int pixelformat;
	
	//如果可以得到指定的像素格式
	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == FALSE )
	{
		AfxMessageBox("ChoosePixelFormat failed");
		return false;
	}
	
	//用上面取到的格式设置设备环境
	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		AfxMessageBox("SetPixelFormat failed");
		return false;
	}
	return true;
}


// 通过用户点击的三个点作为三角形的三个顶点。如果用户没有按照顺时针点击，需要重新计算让其为顺时针方向
void CMyRasterView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//m_bHaveTriangle = false;
	//m_nLBDown++;
	//KillTimer(1);
	//CRect aRect;
	//GetClientRect(&aRect);
	//int nWinHeight = aRect.bottom-aRect.top;
	//point.y = nWinHeight - point.y;

	//if (m_nLBDown == 1)
	//{
	//	ScreenV1[0] = point.x;
	//	ScreenV1[1] = point.y;
	//}
	//else if (m_nLBDown == 2)
	//{
	//	ScreenV2[0] = point.x;
	//	ScreenV2[1] = point.y;
	//}
	//else if (m_nLBDown >= 3)
	//{
	//	ScreenV3[0] = point.x;
	//	ScreenV3[1] = point.y;
	//	if (!m_pRaster->ClockWizeTriangle(ScreenV1, ScreenV2, ScreenV3))	// 非三角形
	//	{
	//		m_nLBDown = 2;
	//		return;		
	//	}
	//	m_bHaveTriangle = true;
	//	m_nLBDown = 0;
	//	// 对用户绘制的三角形进行栅格化
	//	//m_pRaster->TriangleRasterization(ScreenV1[0], ScreenV1[1],	ScreenV2[0],	ScreenV2[1],	ScreenV3[0] ,	ScreenV3[1]);

	//	//GLdouble triangleVertex[9];
	//	//triangleVertex[0] = ScreenV1[0];
	//	//triangleVertex[1] = ScreenV1[1];
	//	//triangleVertex[2] = 18;
	//	//triangleVertex[3] = ScreenV2[0];
	//	//triangleVertex[4] = ScreenV2[1];
	//	//triangleVertex[5] = -20;
	//	//triangleVertex[6] = ScreenV3[0];
	//	//triangleVertex[7] = ScreenV3[1];
	//	//triangleVertex[8] = 10.5;
	//	//GLfloat vetexTex[6];
	//	//vetexTex[0] = 0.1;
	//	//vetexTex[1] = 0.1;
	//	//vetexTex[2] = 0.9;
	//	//vetexTex[3] = 0.1;
	//	//vetexTex[4] = 0.5;
	//	//vetexTex[5] = 0.5;

	//	//GLdouble modleViewMatrix[16];
	//	//glGetDoublev(GL_MODELVIEW_MATRIX, modleViewMatrix);

	//	////m_pRaster->InterTexCoord(triangleVertex, vetexTex, modleViewMatrix, 3);

	//	//WorldV1[0] = -3;
	//	//WorldV1[1] = 0;
	//	//WorldV1[2] = 0;

	//	//WorldV2[0] = 0;
	//	//WorldV2[1] = 0;
	//	//WorldV2[2] = 10;

	//	//WorldV3[0] = 3;
	//	//WorldV3[1] = 0;
	//	//WorldV3[2] = 0;
	//	//m_pRaster->GetScreenCoord(WorldV1, ScreenV1);
	//	//m_pRaster->GetScreenCoord(WorldV2, ScreenV2);
	//	//m_pRaster->GetScreenCoord(WorldV3, ScreenV3);
	//	//if (!m_pRaster->ClockWizeTriangle(ScreenV1, ScreenV2, ScreenV3))	// 非三角形
	//	//{
	//	//	m_nLBDown = 2;
	//	//	return;		
	//	//}
	//	//m_pRaster->TriangleRasterization(ScreenV1[0], ScreenV1[1],	ScreenV2[0],	ScreenV2[1],	ScreenV3[0] ,	ScreenV3[1], modleViewMatrix, 2);
	//	//// 设定计时器的目的是为了在屏幕上更好地显示栅格过程
	//	//SetTimer(1, 5, 0);
	//	//m_nDrawPoints = 0;
	//	//Invalidate(true);
	//}

	CScrollView::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////


BOOL CMyRasterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}
/////////////////////////////////////////////////////////////////////////////
// CMyRasterView printing

BOOL CMyRasterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyRasterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyRasterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyRasterView diagnostics

#ifdef _DEBUG
void CMyRasterView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyRasterView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMyRasterDoc* CMyRasterView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyRasterDoc)));
	return (CMyRasterDoc*)m_pDocument;
}
#endif //_DEBUG

