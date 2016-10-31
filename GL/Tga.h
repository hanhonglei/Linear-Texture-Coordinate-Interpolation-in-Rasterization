/*
* Copyright (c) 2004,DECA LAB
* All rights reserved.
* �ļ����ƣ�Tga.h
* ժ    Ҫ��Tga���ļ�˵���ͺ�������
* ��ǰ�汾��1.0
* ��    �ߣ�DECA LAB:�ѹ��� ������
* ������ڣ�2004��7��15��
* ȡ���汾��1.0 
* ԭ����  ��DECA LAB:�ѹ��� ������
* ������ڣ�2004��7��15��
*/  
/////////////////////////////////////////////////////////////////

#ifndef __TGA_H__
#define __TGA_H__

#pragma comment(lib, "Opengl32.lib")					//Link to OpenGL32.lib so we can use OpenGL stuff

#include "gl\gl.h"
#include "texture.h"



typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;


typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;

bool LoadTGA(Texture * , char * );				// Load a TGA file
bool LoadUncompressedTGA(Texture *, char *, FILE *);	// Load an Uncompressed file
bool LoadCompressedTGA(Texture *, char *, FILE *);		// Load a Compressed file

#endif






