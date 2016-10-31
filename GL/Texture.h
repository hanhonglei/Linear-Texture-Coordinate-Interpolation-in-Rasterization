/*
* Copyright (c) 2004,DECA LAB
* All rights reserved.
* 文件名称：Texture.h
* 摘    要：Texture的文件说明
* 当前版本：1.0
* 作    者：DECA LAB:费广正 韩红雷
* 完成日期：2004年7月15日
* 取代版本：1.0 
* 原作者  ：DECA LAB:费广正 韩红雷
* 完成日期：2004年7月15日
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