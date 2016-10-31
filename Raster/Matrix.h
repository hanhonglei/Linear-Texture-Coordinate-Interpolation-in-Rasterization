// Matrix1.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX1_H__35A217E3_A5E0_492C_B345_A1DE28E8658C__INCLUDED_)
#define AFX_MATRIX1_H__35A217E3_A5E0_492C_B345_A1DE28E8658C__INCLUDED_

//#include "BpNNet.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMatrix  
{
public:
	CMatrix();
	CMatrix(int nRows, int nCols);
	CMatrix(int nRows, int nCols, double value[]);
	CMatrix(int nSize);
	CMatrix(int nSize, double value[]);
	CMatrix(const CMatrix& other);
	CMatrix GetMean(int mode,BOOL vec);
	void SetCol(int nCol,double value[]);
	void SetRow(int nRow,double value[]);
	BOOL Init(int nRows, int nCols);
	BOOL MakeUnitMatrix(int nSize);
	//BOOL FromString(CString s, const CString& sDelim = " ", BOOL bLineBreak = TRUE);
	CString ToString(const CString& sDelim = " ", BOOL bLineBreak = TRUE) const;
	CString RowToString(int nRow, const CString& sDelim = " ") const;
	CString ColToString(int nCol, const CString& sDelim = " ") const;
	void SetData(double value[]);
	BOOL SetElement(int nRow, int nCol, double value);
	double GetElement(int nRow, int nCol) const;
	int GetNumColumns() const;
	int GetNumRows() const;
	double * GetData() const;
	int GetRowVector(int nRow, double* pVector) const;
	int GetColVector(int nCol, double* pVector) const;
	CMatrix & operator=(const CMatrix& other);
	BOOL operator==(const CMatrix& other) const;
	BOOL operator!=(const CMatrix& other) const;
	CMatrix operator+(const CMatrix& other) const;
	CMatrix operator+(double value) const;
	CMatrix operator^(double value) const;
	CMatrix operator-(const CMatrix& other) const;
	CMatrix operator*(double value) const;
	CMatrix operator%(int value) const;
	CMatrix operator/(double value) const;
	CMatrix operator*(const CMatrix& other) const;
	
	//CMul(const CMatrix& AR, const CMatrix& AI, const CMatrix& BR, const CMatrix& BI, CMatrix& CR, CMatrix& CI) const;
	CMatrix Transpose() const;
	BOOL InvertGaussJordan();
	BOOL InvertGaussJordan(CMatrix& mtxImag);
	BOOL InvertSsgj();
	BOOL InvertTrench();
	double DetGauss();
	int RankGauss();
	BOOL DetCholesky(double* dblDet);
	BOOL SplitLU(CMatrix& mtxL, CMatrix& mtxU);
	BOOL SplitQR(CMatrix& mtxQ);
	BOOL SplitUV(CMatrix& mtxU, CMatrix& mtxV, double eps = 0.000001);
	void ppp(double a[], double e[], double s[], double v[], int m, int n);
	void sss(double fg[2], double cs[2]);
	BOOL GInvertUV(CMatrix& mtxAP, CMatrix& mtxU, CMatrix& mtxV, double eps = 0.000001);
	BOOL MakeSymTri(CMatrix& mtxQ, CMatrix& mtxT, double dblB[], double dblC[]);
	BOOL SymTriEigenv(double dblB[], double dblC[], CMatrix& mtxQ, int nMaxIt = 60, double eps = 0.000001);
	void MakeHberg();
	BOOL HBergEigenv(double dblU[], double dblV[], int nMaxIt = 60, double eps = 0.000001);
	BOOL JacobiEigenv(double dblEigenValue[], CMatrix& mtxEigenVector, int nMaxIt = 100, double eps = 0.0000001);
	BOOL JacobiEigenv2(double dblEigenValue[], CMatrix& mtxEigenVector, double eps = 0.0000001);
	virtual ~CMatrix();
public:
	double CalcIntegralImage(LONG x,LONG y);
	double CalcIntegralImageSquare(LONG x,LONG y);
	CMatrix LessThan(double value);
	CMatrix GraterOrEqual(double value);
	void SetCols(CMatrix Tmp,LONG nCols1,LONG nCols2);
	void SetRows(CMatrix Tmp,LONG nRows1,LONG nRows2);
	CMatrix Ones(int mode,int nSize1,int nSize2=0);
	CMatrix KronecherProd(CMatrix Y);
	CMatrix Find(const char *Expr);
	CMatrix GetCols(LONG nCols1,LONG nCols2);
	CMatrix GetRows(LONG nRows1,LONG nRows2);
	BOOL MakeDiag(int nSize,double *v);
	void GenIdentity(int nRows,int nColumns);
	CMatrix Dot(const CMatrix& other) const;
	int m_nNumColumns;
	int m_nNumRows;
	double * m_pData;
	BOOL m_Abs;
public:
	LONG GetNumFourRectFeature();
	LONG GetNumThreeRectFeature();
	LONG GetNumTwoRectFeature();
	double CalcIntegralImageRect(LONG x,LONG y,LONG width,LONG height);
	CMatrix CalcIntegralImageSquare();
	CMatrix CalcIntegralImage();
	CMatrix ChangeSize(LONG height,LONG width);
	void SortEigen(double EigenValue[],CMatrix& EigenVector,int mode=0,int left=0, int right=0,BOOL Abs=FALSE);
private:
	double CalcCumulativeSum(LONG x,LONG y);
	double CalcCumulativeSumSquare(LONG x,LONG y);
	void SortEigenVal(double EigenValue[],CMatrix& EigenVector,int mode=0,int left=0, int right=0);
};

#endif // !defined(AFX_MATRIX1_H__35A217E3_A5E0_492C_B345_A1DE28E8658C__INCLUDED_)
