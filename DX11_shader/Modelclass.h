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
	// color shader class ���̾ƿ��� ��ġ�ؾ� ��
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
	// �ε��� ���ۿ� �� ������ ũ�⸦ �� �� �ִ� �ΰ��� ������ ��ˤ��ȴ�
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif