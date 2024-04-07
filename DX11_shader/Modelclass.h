#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Modelclass_H_
#define _Modelclass_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;
#include "Textureclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Modelclass
////////////////////////////////////////////////////////////////////////////////
class Modelclass
{
private:
	// color shader class 레이아웃과 일치해야 함
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;//XMFLOAT4 color;
		XMFLOAT3 normal;
	};

public:
	Modelclass();
	Modelclass(const Modelclass&);
	~Modelclass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	// 모델을 그리는 쉐이더에 자체 텍스처 리소스 전달
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// for textrue
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	// 인덱스 버퍼와 각 버퍼의 크기를 알 수 있는 두개의 정수가 사ㅛㅇ된다
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

	// 3d모델의 텍스쳐 변수
	Textureclass* m_Texture;
};

#endif