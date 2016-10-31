// MyRaster.h : main header file for the MyRaster application
//

#if !defined(AFX_MyRaster_H__58399E43_3D1C_4DF0_88CE_F6E7531FED8F__INCLUDED_)
#define AFX_MyRaster_H__58399E43_3D1C_4DF0_88CE_F6E7531FED8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "..\resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyRasterApp:
// See MyRaster.cpp for the implementation of this class
//

class CMyRasterApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMyRasterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRasterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyRasterApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyRaster_H__58399E43_3D1C_4DF0_88CE_F6E7531FED8F__INCLUDED_)
