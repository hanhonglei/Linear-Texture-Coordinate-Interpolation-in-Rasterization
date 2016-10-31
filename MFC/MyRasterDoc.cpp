/*
* Copyright (c) 2005,DECA LAB
* All rights reserved.
* 文件名称：MainFrm.cpp
* 摘    要：按照mfc要求，在serialize中实现文件的载入
* 当前版本：1.0
* 作    者：DECA LAB:韩红雷
* 完成日期：2005年3月17日
*/  
/////////////////////////////////////////////////////////////////
// MyRasterDoc.cpp : implementation of the CMyRasterDoc class
//

#include "stdafx.h"
#include "MyRaster.h"

#include "MyRasterDoc.h"

#include "..\gl\Tga.h"
#include <gl\gl.h>
#include <gl\glu.h>												// Header File For The GLu32 Library

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRasterDoc

IMPLEMENT_DYNCREATE(CMyRasterDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyRasterDoc, CDocument)
	//{{AFX_MSG_MAP(CMyRasterDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRasterDoc construction/destruction

CMyRasterDoc::CMyRasterDoc()
{
	// TODO: add one-time construction code here
	m_texture.imageData = NULL;
	m_isTexLoaded = FALSE;
	m_docSize = CSize(0, 0);
}

CMyRasterDoc::~CMyRasterDoc()
{
	FreeTexData(m_texture);
}

BOOL CMyRasterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}
void CMyRasterDoc::FreeTexData(Texture tex)
{
	if (tex.imageData)						// If Texture Image Exists ( CHANGE )
	{
		free(tex.imageData);					// Free The Texture Image Memory ( CHANGE )
		tex.imageData = NULL;
	}
}



/////////////////////////////////////////////////////////////////////////////
// CMyRasterDoc serialization

void CMyRasterDoc::Serialize(CArchive& ar)
{
	CFile *file = ar.GetFile();
	int fileLen = (int)file->GetLength();
	char *sFileName = new char [fileLen+1];
	strcpy(sFileName, file->GetFilePath());
	static bool bFilter = false;
	
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		glEnable(GL_TEXTURE_2D);
		if (LoadTGA(&m_texture, sFileName))
		{
			//FreeTexData(m_texture);
			glGenTextures(1, &m_texture.texID);				// Create The Texture ( CHANGE )
			glBindTexture(GL_TEXTURE_2D, m_texture.texID);
			glTexImage2D(GL_TEXTURE_2D, 0, m_texture.bpp / 8, m_texture.width, m_texture.height
				, 0, m_texture.type, GL_UNSIGNED_BYTE, m_texture.imageData);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,bFilter?GL_NEAREST:GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,bFilter?GL_NEAREST:GL_LINEAR);
			bFilter = !bFilter;
			// 获得文档的大小，以便确定滚动条的范围
			m_docSize = CSize(m_texture.width, m_texture.height);
			
			// 不删除颜色数据，使用栅格化后的纹理坐标值直接索引这个数据
			//if (m_texture.imageData)						// If Texture Image Exists ( CHANGE )
			//{
			//	free(m_texture.imageData);					// Free The Texture Image Memory ( CHANGE )
			//	m_texture.imageData = NULL;
			//}
			m_isTexLoaded = TRUE;
		}
	}	
	// 释放指针
	delete sFileName;
}

CSize CMyRasterDoc::GetDocSize()
{
	return m_docSize;
}

/////////////////////////////////////////////////////////////////////////////
// CMyRasterDoc diagnostics

#ifdef _DEBUG
void CMyRasterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyRasterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyRasterDoc commands

// 完成绘制.第一个参数是绘制图像左上角的坐标，后一个是右下角的坐标
void CMyRasterDoc::DrawTriangleVertex(CPoint upLeft, CPoint downRight)
{
	glPushMatrix();
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture.texID);
	
	glBegin(GL_QUADS);											// 利用所选定的关键点坐标绘制关键点
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(upLeft.x, upLeft.y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(downRight.x, upLeft.y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(downRight.x, downRight.y);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(upLeft.x, downRight.y);
	glEnd();
	glPopMatrix();
	
}

void CMyRasterDoc::DrawTriangleVertex(double v1x, double v1y,double v2x,double v2y,double v3x,double v3y)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f, 0.0f, 0.0f);
	
	glBegin(GL_LINE_LOOP);											// 利用所选定的关键点坐标绘制关键点
		glVertex2d(v1x, v1y);
		glVertex2d(v2x, v2y);
		glVertex2d(v3x, v3y);
	glEnd();
	glPopMatrix();
	
}

void CMyRasterDoc::DrawCoordinate()
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	
	double lineLength = 5.0f;
	glLineWidth(3.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);											// 利用所选定的关键点坐标绘制关键点
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3d(lineLength, 0.0f, 0.0f);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);											// 利用所选定的关键点坐标绘制关键点
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3d(0.0f, lineLength, 0.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);											// 利用所选定的关键点坐标绘制关键点
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3d(0.0f, 0.0f, lineLength);
	glEnd();
	glPopMatrix();

}
void CMyRasterDoc::DrawTexturedQuad()
{
	if (!m_isTexLoaded)
		return;

	glPushMatrix();

	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture.texID);
	double fQuadWidth = 5.0f;

	glBegin(GL_QUADS);											// 利用所选定的关键点坐标绘制关键点
	glTexCoord2f(0.0f, 1.0f);
	glVertex3d(-fQuadWidth, 0.0f, -fQuadWidth);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3d(fQuadWidth, 0.0f, -fQuadWidth);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3d(fQuadWidth, 0.0f, fQuadWidth);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3d(-fQuadWidth,.0f, fQuadWidth);
	glEnd();
	glPopMatrix();
}

void CMyRasterDoc::DrawTexturedTriangle(double V1[3], double V2[3], double V3[3], double texCoord[6])
{
	glPushMatrix();

	glLoadIdentity();
	double fQuadWidth = 5.0f;
	if (!m_isTexLoaded)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_texture.texID);
	}

	glBegin(GL_TRIANGLES);											// 利用所选定的关键点坐标绘制关键点
	glTexCoord2dv(texCoord);
	glVertex3dv(V1);
	glTexCoord2dv(texCoord+2);
	glVertex3dv(V2);
	glTexCoord2dv(texCoord+4);
	glVertex3dv(V3);
	glEnd();
	glPopMatrix();
}
void CMyRasterDoc::DrawRasteredPoints(int x, int y, double pointColor[3], double pointSize)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	if (pointColor == NULL)
		glColor3f(0.0f, 1.0f, 1.0f);

	else
		glColor3dv(pointColor );
	glPointSize(float(pointSize));

	glBegin(GL_POINTS);											// 利用所选定的关键点坐标绘制关键点

		glVertex2i(x, y);
	glEnd();
	glPopMatrix();

}
