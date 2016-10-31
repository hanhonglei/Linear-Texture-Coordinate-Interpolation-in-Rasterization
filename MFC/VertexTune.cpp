// VertexTune.cpp : implementation file
//

#include "stdafx.h"
#include "MyRaster.h"
#include "MainFrm.h"
#include "VertexTune.h"
#include "MyRasterDoc.h"
#include "MyRasterView.h"



// CVertexTune dialog

IMPLEMENT_DYNAMIC(CVertexTune, CDialog)

CVertexTune::CVertexTune(CWnd* pParent /*=NULL*/)
	: CDialog(CVertexTune::IDD, pParent)
{

}

CVertexTune::~CVertexTune()
{
}

void CVertexTune::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVertexTune, CDialog)
END_MESSAGE_MAP()


// CVertexTune message handlers

BOOL CVertexTune::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame *pfm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	CMyRasterView *pView = (CMyRasterView*) pfm->GetActiveView(); 
	double vertex[9];
	double tex[6];
	RASTERTYPE rType;
	CALTEXCOORDTYPE tType;
	int nFraction = 0;
	bool bPerPixelDiv = false;
	bool bReduceError = false;
	int nRasterLine = 0;
	pView->GetVertexData(vertex, tex, rType, nFraction, bPerPixelDiv, tType, bReduceError, nRasterLine);
	CString s;
	for(int i = 0; i < 9; i++)
	{
		CEdit* pEdit   =   (CEdit*) GetDlgItem(IDC_V1X+i);   
		s.Format(_T("%lf"), vertex[i]);
		pEdit->SetWindowText(s);   
	}
	for (int i = 0; i < 6; i++)
	{
		CEdit* pEdit   =   (CEdit*) GetDlgItem(IDC_V1U+i);   
		s.Format(_T("%lf"), tex[i]);
		pEdit->SetWindowText(s);   
	}
	CComboBox* pCombo = (CComboBox*) GetDlgItem(IDC_RASTER_TYPE);
	pCombo->SetCurSel(rType);	

	pCombo = (CComboBox*) GetDlgItem(IDC_TEX_TYPE);
	pCombo->SetCurSel(tType);	

	CEdit* pEdit   =   (CEdit*) GetDlgItem(IDC_FRACTION);   
	s.Format(_T("%d"), nFraction);
	pEdit->SetWindowText(s);   

	pEdit = (CEdit*) GetDlgItem(IDC_RASTER_LINE_NUM);
	s.Format(_T("%d"), nRasterLine);
	pEdit->SetWindowText(s);   

	((CButton *)GetDlgItem(IDC_PERPIXEL_DIV))->SetCheck(bPerPixelDiv);

	((CButton *)GetDlgItem(IDC_REDUCE_ERROR))->SetCheck(bReduceError);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CVertexTune::OnOK()
{

	CMainFrame *pfm = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	CMyRasterView *pView = (CMyRasterView*) pfm->GetActiveView(); 
	bool bAuto = ((CButton *)GetDlgItem(IDC_AUTO_CALC))->GetCheck();
	if (bAuto)
	{
		CDialog::OnOK();
		pView->AutoCalcTexData();
		return;
	}
	double vertex[9];
	double tex[6];
	CString s;
	int nFraction = 2;
	int nRasterLine = 0;
	for(int i = 0; i < 9; i++)
	{
		CEdit* pEdit   =   (CEdit*) GetDlgItem(IDC_V1X+i);   
		pEdit->GetWindowText(s);  
		vertex[i] = atof(s.GetString());
	}
	for (int i = 0; i < 6; i++)
	{
		CEdit* pEdit   =   (CEdit*) GetDlgItem(IDC_V1U+i);   
		pEdit->GetWindowText(s);  
		tex[i] = (double)atof(s.GetString());
	}
	RASTERTYPE rType;
	CComboBox* pCombo = (CComboBox*) GetDlgItem(IDC_RASTER_TYPE);
	rType = (RASTERTYPE)pCombo->GetCurSel();	

	CALTEXCOORDTYPE tType;
	pCombo = (CComboBox*) GetDlgItem(IDC_TEX_TYPE);
	tType = (CALTEXCOORDTYPE)pCombo->GetCurSel();	


	CEdit* pEdit   =   (CEdit*) GetDlgItem(IDC_FRACTION);   
	pEdit->GetWindowText(s);   
	nFraction = atoi(s.GetString());

	pEdit = (CEdit*) GetDlgItem(IDC_RASTER_LINE_NUM);   
	pEdit->GetWindowText(s);   
	nRasterLine = atoi(s.GetString());

	bool bPerPixelDiv = ((CButton *)GetDlgItem(IDC_PERPIXEL_DIV))->GetCheck();

	bool bReduceError = ((CButton *)GetDlgItem(IDC_REDUCE_ERROR))->GetCheck();

	pView->SetVertexData(vertex, tex, rType, nFraction, bPerPixelDiv, tType, bReduceError, nRasterLine);

	CDialog::OnOK();
}
