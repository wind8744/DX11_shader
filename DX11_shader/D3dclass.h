#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: D3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3dclass_H_
#define _D3dclass_H_

///
//������� ���� ���� �ؾ� �� ���� �� ��ü ����� ����� �� ������ ���̺귯���� �����ϴ� ���Դϴ�.
// ù ��° ���̺귯������ DirectX 11���� 3D �׷����� �����ϰ� �׸��� ���� ��� Direct3D ����� ���ԵǾ� �ֽ��ϴ�.
// �� ��° ���̺귯������ ��ǻ���� �ϵ����� �������̽��Ͽ� ������� ���� ��ħ ��, ��� ���� ���� ī�� � ���� ������ ��� ������ ���ԵǾ� �ֽ��ϴ�.
// �� ��° ���̺귯������ ���̴� ������ ����� ���ԵǾ� �ֽ��ϴ�.
//

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: D3dclass
////////////////////////////////////////////////////////////////////////////////
class D3dclass
{
public:
	D3dclass();
	D3dclass(const D3dclass&);
	~D3dclass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice()	{ return m_device; }
    ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }

	void GetProjectionMatrix(XMMATRIX&) const;
	void GetWorldMatrix(XMMATRIX&) const ;
	void GetOrthoMatrix(XMMATRIX&) const ;

	void GetVideoCardInfo(char*, int&) const;

	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	D3D11_VIEWPORT m_viewport;
};

#endif