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
		XMFLOAT4 color;
	};

public:
	Modelclass();
	Modelclass(const Modelclass&);
	~Modelclass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	// 인덱스 버퍼와 각 버퍼의 크기를 알 수 있는 두개의 정수가 사ㅛㅇ된다
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif