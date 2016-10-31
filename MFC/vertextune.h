#pragma once


// CVertexTune dialog

class CVertexTune : public CDialog
{
	DECLARE_DYNAMIC(CVertexTune)

public:
	CVertexTune(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVertexTune();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
