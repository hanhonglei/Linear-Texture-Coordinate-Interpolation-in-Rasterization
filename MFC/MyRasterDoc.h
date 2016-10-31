// MyRasterDoc.h : interface of the CMyRasterDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyRasterDOC_H__D5FAB48C_9158_4BBF_9874_AE81401D7725__INCLUDED_)
#define AFX_MyRasterDOC_H__D5FAB48C_9158_4BBF_9874_AE81401D7725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\gl\Texture.h"

class CMyRasterDoc : public CDocument
{
protected: // create from serialization only
	CMyRasterDoc();
	DECLARE_DYNCREATE(CMyRasterDoc)

// Attributes
public:
	bool	m_isTexLoaded;
	Texture m_texture;

protected:
	CSize	m_docSize;
	void FreeTexData(Texture tex);


// Operations
public:
	CSize	GetDocSize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRasterDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawTriangleVertex(CPoint upLeft, CPoint downRight);
	void DrawTriangleVertex(double v1x, double v1y,double v2x,double v2y,double v3x,double v3y);
	void DrawRasteredPoints(int x, int y, double pointColor[3], double pointSize = 2.0f);
	void DrawCoordinate();
	void DrawTexturedQuad();
	void DrawTexturedTriangle(double V1[3], double V2[3], double V3[3], double texCoord[6]);



	virtual ~CMyRasterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyRasterDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyRasterDOC_H__D5FAB48C_9158_4BBF_9874_AE81401D7725__INCLUDED_)
