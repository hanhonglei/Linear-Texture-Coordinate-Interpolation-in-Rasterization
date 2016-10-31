/*
* Copyright (c) 2004,DECA LAB
* All rights reserved.
* �ļ����ƣ�Texture.h
* ժ    Ҫ��Texture���ļ�˵��
* ��ǰ�汾��1.0
* ��    �ߣ�DECA LAB:�ѹ��� ������
* ������ڣ�2004��7��15��
* ȡ���汾��1.0 
* ԭ����  ��DECA LAB:�ѹ��� ������
* ������ڣ�2004��7��15��
*/  
/////////////////////////////////////////////////////////////////

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

// #pragma comment(lib, "Opengl32.lib")					//Link to OpenGL32.lib so we can use OpenGL stuff

#include <gl\gl.h>
#include <gl\glu.h>


typedef	struct									
{
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;											// Image Type (GL_RGB, GL_RGBA)
} Texture;	

#endif