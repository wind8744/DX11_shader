#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Colorshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Colorshaderclass_H_
#define _Colorshaderclass_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: Colorshaderclass
// gpu에 잇는 3d모델을 그리기 위해 hlsl 쉐이더를 호출함
////////////////////////////////////////////////////////////////////////////////
class Colorshaderclass
{
private:
	// 정점 쉐이더와 함께 사용되는 cbuffer 의 정의
	// 구조체가 쉐이더의 정의와 일치해야함.
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	Colorshaderclass();
	Colorshaderclass(const Colorshaderclass&);
	~Colorshaderclass();

	// 렌더링은 쉐이더 매겨변수 설정 후 
	// 쉐이더를 사용하여 준비된 모델의 정점을 그림
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif