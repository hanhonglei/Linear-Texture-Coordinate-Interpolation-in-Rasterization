// MyRasterView.h : interface of the CMyRasterView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyRasterVIEW_H__CFE84EEE_1B32_43D5_A07D_560FC1D43DFB__INCLUDED_)
#define AFX_MyRasterVIEW_H__CFE84EEE_1B32_43D5_A07D_560FC1D43DFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\Raster\raster.h"

#define OPENGL_RENDER 1
#define MFC_RENDER 2
extern int RENDER_TYPE;

class CMyRasterView : public CScrollView
{
protected: // create from serialization only
	CMyRasterView();
	DECLARE_DYNCREATE(CMyRasterView)
		
		// Attributes
protected:
	// 指向显示设备
	CClientDC *m_pDC;

	double m_WorldV[9];
	double m_TextureCoord[6];

	int m_nDrawPoints;
	int m_nLBDown;
	bool m_bHaveTriangle;
	CRaster *m_pRaster;
	CRect m_wndRect;
	bool m_bRefresh;
	bool m_bPerPixelDiv;
	bool m_bReduceError;
	int m_nRasterLine;

	double m_view[3];		// 第一个参数是球面坐标的半径、第二个是和y的夹角，第三个是平面投影和z的夹角
	double m_viewCenter[3];	// 视线的中心，默认是原点
	double m_lookArtPara[9];// glulookat 的几个参数，用以上两个数组计算得到

	double m_renderTime;
	RASTERTYPE m_rasterType;
	CALTEXCOORDTYPE m_texType;

	int m_nFraction;

	void InitRandomVertex();
	void CalcTexAndWriteToFile(CDC* pDC = NULL, bool bWrite = true);

	
public:
	CMyRasterDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRasterView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL bSetPixelFormat();
	void IniOpenGL();
	void ChangeVertex();
	virtual ~CMyRasterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyRasterView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void GetVertexData(double vertex[9], double texCoord[6], RASTERTYPE &rType, int &fraction, bool &bPerPixelDiv
		, CALTEXCOORDTYPE &tType, bool &bReduceError, int &nRasterLine);
	void SetVertexData(double vertex[9], double texCoord[6], RASTERTYPE rType, int fraction, bool bPerPixelDiv
		,  CALTEXCOORDTYPE tType, bool bReduceError, int nRasterLine);
	void AutoCalcTexData();	// 自动使用各种方案进行纹理插值计算

};

#ifndef _DEBUG  // debug version in MyRasterView.cpp
inline CMyRasterDoc* CMyRasterView::GetDocument()
   { return (CMyRasterDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyRasterVIEW_H__CFE84EEE_1B32_43D5_A07D_560FC1D43DFB__INCLUDED_)
