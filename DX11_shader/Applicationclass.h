#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
// 
// ������. �׷��� ���� Ŭ����
// The application class is the other object that is created by the system class. 
// All the graphics functionality in this application will be encapsulated in this class. 
// I will also use the header in this file for all the graphics related global settings 
// that we may want to change such as full screen or windowed mode. 
// 
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <windows.h>

#include "D3dclass.h"
#include "Cameraclass.h"
#include "Modelclass.h"
#include "Colorshaderclass.h"
#include "Textureshaderclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class Applicationclass
{
public:
	Applicationclass();
	Applicationclass(const Applicationclass&);
	~Applicationclass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3dclass* m_Direct3D;
	Cameraclass* m_Camera;
	Modelclass* m_Model;
	Colorshaderclass* m_ColorShader;

	Textureshaderclass* m_TextureShader;
};

#endif