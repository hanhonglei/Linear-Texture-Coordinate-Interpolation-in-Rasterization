/*
* Copyright (c) 2005,DECA LAB
* All rights reserved.
* �ļ����ƣ�MainFrm.cpp
* ժ    Ҫ��ע���Ҹ���ע�͵ĵط�
* ��ǰ�汾��1.0
* ��    �ߣ�DECA LAB:������
* ������ڣ�2005��3��17��
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
	// leo : ��ʼ��
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
	// �޸�Ϊ��������Ĳ�������͸������ϵ
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
	// �ͷ�ָ��
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
	// ��������������������ͼ�����
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
		CDC   *MemDC = new CDC;   //���ȶ���һ����ʾ�豸����   
		CBitmap   MemBitmap;//����һ��λͼ����   

		//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸   
		MemDC->CreateCompatibleDC(NULL);   
		//��ʱ�����ܻ�ͼ����Ϊû�еط���   ^_^   
		//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С   
		MemBitmap.CreateCompatibleBitmap(pDC,m_wndRect.Width(),m_wndRect.Height());   

		//��λͼѡ�뵽�ڴ���ʾ�豸��   
		//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��   
		CBitmap   *pOldBit=MemDC->SelectObject(&MemBitmap);   

		//���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ����   
		//��Ҳ�������Լ�Ӧ���õ���ɫ   
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
		s.Format(_T("����դ��%sģʽ.����������%sģʽ. դ������ʱ��:%f����.")
			, (m_rasterType == KIM)?_T("KIM"):_T("WANG"),(m_texType == KIMTEX)?_T("KIMTEX"):_T("WANGTEX"),m_renderTime);

		pCurDC->TextOut(10, 10, s);

#ifdef DOUBLE_BUFFER
		//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ   
		pDC->BitBlt(0,0,m_wndRect.Width(),m_wndRect.Height(),MemDC,0,0,SRCCOPY);   

		//��ͼ��ɺ������   
		MemBitmap.DeleteObject();   
		MemDC->DeleteDC();   
		delete MemDC;
#endif
	}
}

// temp
double fErrorUGlobal, fErrorVGlobal;


// �Զ�ʹ�ø��ַ������������ֵ����
void CMyRasterView::AutoCalcTexData()
{
	myfile << "\n�� ����1��ʹ��WANG�㷨�������ز�ֵ�����������Բ�ֵ�������\n";
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
	myfile << "\n�� ����2��ʹ��WANG�㷨�������ز�ֵ�����������Բ�ֵ�������\n";
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
	//myfile << "\n�� ����3��ʹ��WANG�㷨��ʹ��KIM����߽���������(���ȼ�m=4)�����������Բ�ֵ�������\n";
	//for (int nRasterLine = 0; nRasterLine < 5; nRasterLine++)
	//{
	//	SetVertexData(m_WorldV, m_TextureCoord, KIM, 3, false, WANGTEX, false, nRasterLine);
	//	CalcTexAndWriteToFile();
	//}
	//myfile << "\n�� ����4��ʹ��WANG�㷨��ʹ��KIM����߽���������(���ȼ�m=4)�����������Բ�ֵ�������\n";
	//for (int nRasterLine = 0; nRasterLine < 5; nRasterLine++)
	//{
	//	SetVertexData(m_WorldV, m_TextureCoord, KIM, 3, false, WANGTEX, true, nRasterLine);
	//	CalcTexAndWriteToFile();
	//}

	myfile << "\n�� ����5��ʹ��KIM�㷨\n";
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
	// ��ȡÿ�����ص����ʵ�������꣬ʹ�������س�������.����
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
		// ����դ��
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
		// դ������
		// ���������������
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
			myfile << "\t,�����س���";
		}
		else
		{
			myfile << "\t�����ֵ����:";
			myfile<<m_nFraction;
		}

		if (m_texType == WANGTEX)
		{
			myfile << "\tɨ����б��:";
			myfile << m_pRaster->GetWangTexData()->fGradient;
			myfile << "\t,���ӵ�ɨ������:";
			myfile << m_nRasterLine;
			if (m_bReduceError)
			{
				myfile << "\t,�������";
			}
		}
		myfile<< "\n\t";

		// ��Ⱦ��������
		myfile<< "��Ч������:" ;
		myfile<< nDrawPoints ;
		myfile<< "\t\t����������:";
		myfile<< nTotalPoints ;
		// դ��ʱ��
		myfile<< "\t\tդ������ʱ��:";
		myfile<< m_renderTime;
		myfile << "����";

		myfile << "\n\t";

		myfile << "----u�����v����ƽ�����:";
		myfile << fErrorU/nDrawPoints;
		myfile << ",";
		myfile << fErrorV/nDrawPoints;
		myfile << "\t\tu�����v����������:";
		myfile << fMaxErrorU;
		myfile << ",";
		myfile << fMaxErrorV;

		myfile << " \n\t����0.3��u��v�ĸ����ֱ��ǣ�" << nEU<<","<<nEV;
		myfile << " \n\t����0.1��u��v�ĸ����ֱ��ǣ�" << nEU01<<","<<nEV01;

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
	// ��ȷ�����������ķ�Χ
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
	
}

/////////////////////////////////////////////////////////////////////////////
// CMyRasterView message handlers

int CMyRasterView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// leo : ��ʼ��opengl
	IniOpenGL();

	return 0;
}

void CMyRasterView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	// �����Ӵ�	
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
// �޸�Ϊ��������Ĳ�������͸������ϵ
    gluPerspective(/*vertical field of view*/ 45.,
		/*aspect ratio*/ /*(double) viewport.width/viewport.height,*/(double)nWinWidth/nWinHeight,
		/*znear*/ .1, /*zfar*/ 50.);
	
	//glOrtho(0.0f, nWinWidth, 0.0f, nWinHeight, -1.0f, 1.0f);			// Create Ortho 640x480 View (0,0 At bottom Left)
	/* from here on we're setting modeling transformations */
	gluLookAt(m_view[0]*sin(m_view[1])*sin(m_view[2]), m_view[0]*cos(m_view[1]), m_view[0]*sin(m_view[1])*cos(m_view[2])
	,  m_viewCenter[0], m_viewCenter[1], m_viewCenter[2], 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	// leo:ʹ�ú󱸻�����ƣ�������˸
	glDrawBuffer (GL_BACK);

	m_bRefresh = true;
}

void CMyRasterView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	// leo:Ӧ�������Щ�ͷ���Դ�Ĳ���
	HGLRC   hrc;
	
	hrc = ::wglGetCurrentContext();
	
	::wglMakeCurrent(NULL,  NULL);
	
	if (hrc)
		::wglDeleteContext(hrc);	
	
}

// leo:�����б������������������Σ�������˸
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

// leo : ��ʼ��openggl��ͼ����
void CMyRasterView::IniOpenGL()
{
	m_pDC = new CClientDC(this);
	PIXELFORMATDESCRIPTOR pfd;
	int n;
	//����һ�����������ĵľ��
	HGLRC   hrc;
	
	//��ʼ����������Ҫ���ǳ�ʼ����һ���ͻ������豸����ָ��
	ASSERT(m_pDC != NULL);
	
	//����Ӧ����������ظ�ʽ�����뵱ǰ�豸�����������
	if (!bSetPixelFormat())
		return;
	
	//�õ�ָ���豸����������ģʽ����
	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	
	//��������õ�������ֵ������һ������ģʽ
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	
	//����һ���������豸����
	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	
	//�������ɵ��豸������ָ����Ϊ��ǰ����
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);		
	
	//�úڱ���
	glClearColor(0.0,0.0,0.0,0.0);	
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
	glShadeModel(GL_SMOOTH);
	
	// ���û�ɫ����ȡ�ð�͸��Ч��
	//	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 	
	//	glEnable(GL_BLEND); 
	
	//ƽ������
    glEnable (GL_LINE_SMOOTH);
	//    glEnable (GL_BLEND);
	
	//��ʼ��������Ϊ RGBA ģʽ��ͬʱ���� alpha ��ϡ���ʾ������
	//		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//		glEnable (GL_BLEND);
	
	// ������ϸ��͸������
	glHint (GL_POLYGON_SMOOTH_HINT|GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); 
}

// leo ����������OpenGLʹ�õ����ظ�ʽ
BOOL CMyRasterView::bSetPixelFormat()
{
	//����һ�����ظ�ʽ
	static PIXELFORMATDESCRIPTOR pfd =
	{   
		sizeof(PIXELFORMATDESCRIPTOR),			// size of this pfd
			1,									// version number
			PFD_DRAW_TO_WINDOW |				// support window
			PFD_SUPPORT_OPENGL |				// support OpenGL  ֧��OpenGL
			PFD_DOUBLEBUFFER,					// double buffered ֧���ֻ���
			PFD_TYPE_RGBA,						// RGBA typeʹ��RGBAģʽ�����õ�ɫ��
			24,									// 24-bit color depth  ʹ��24λ���ɫ
			0, 0, 0, 0, 0, 0,					// color bits ignored
			0,									// no alpha buffer
			0,									// shift bit ignored
			0,									// no accumulation buffer
			0, 0, 0, 0,							// accum bits ignored
			32,									// 32-bit z-buffer   32λZ�Ỻ��
			0,									// no stencil buffer
			0,									// no auxiliary buffer
			PFD_MAIN_PLANE,						// main layer
			0,									// reserved
			0, 0, 0								// layer masks ignored
	};
	int pixelformat;
	
	//������Եõ�ָ�������ظ�ʽ
	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == FALSE )
	{
		AfxMessageBox("ChoosePixelFormat failed");
		return false;
	}
	
	//������ȡ���ĸ�ʽ�����豸����
	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		AfxMessageBox("SetPixelFormat failed");
		return false;
	}
	return true;
}


// ͨ���û��������������Ϊ�����ε��������㡣����û�û�а���˳ʱ��������Ҫ���¼�������Ϊ˳ʱ�뷽��
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
	//	if (!m_pRaster->ClockWizeTriangle(ScreenV1, ScreenV2, ScreenV3))	// ��������
	//	{
	//		m_nLBDown = 2;
	//		return;		
	//	}
	//	m_bHaveTriangle = true;
	//	m_nLBDown = 0;
	//	// ���û����Ƶ������ν���դ��
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
	//	//if (!m_pRaster->ClockWizeTriangle(ScreenV1, ScreenV2, ScreenV3))	// ��������
	//	//{
	//	//	m_nLBDown = 2;
	//	//	return;		
	//	//}
	//	//m_pRaster->TriangleRasterization(ScreenV1[0], ScreenV1[1],	ScreenV2[0],	ScreenV2[1],	ScreenV3[0] ,	ScreenV3[1], modleViewMatrix, 2);
	//	//// �趨��ʱ����Ŀ����Ϊ������Ļ�ϸ��õ���ʾդ�����
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

