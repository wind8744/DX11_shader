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
	// color shader class ���̾ƿ��� ��ġ�ؾ� ��
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

	// ���� �׸��� ���̴��� ��ü �ؽ�ó ���ҽ� ����
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// for textrue
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	// �ε��� ���ۿ� �� ������ ũ�⸦ �� �� �ִ� �ΰ��� ������ ��ˤ��ȴ�
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

	// 3d���� �ؽ��� ����
	Textureclass* m_Texture;
};

#endif