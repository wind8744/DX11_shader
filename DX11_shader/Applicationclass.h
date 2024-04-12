#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
// 
// 렌더링. 그래픽 관련 클래스
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
//#include "Colorshaderclass.h"
//#include "Textureshaderclass.h"
#include "Lightshaderclass.h"
#include "Lightclass.h"

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

	void Input();
	bool Frame();

private:
	bool Render(float);

private:
	D3dclass* m_Direct3D;
	Cameraclass* m_Camera;
	Modelclass* m_Model;
	//Colorshaderclass* m_ColorShader;
	//Textureshaderclass* m_TextureShader;
	
	Lightshaderclass* m_LightShader;
	Lightclass* m_Light;
};

#endif