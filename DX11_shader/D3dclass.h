#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: D3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3dclass_H_
#define _D3dclass_H_

///
//헤더에서 가장 먼저 해야 할 일은 이 개체 모듈을 사용할 때 연결할 라이브러리를 지정하는 것입니다.
// 첫 번째 라이브러리에는 DirectX 11에서 3D 그래픽을 설정하고 그리기 위한 모든 Direct3D 기능이 포함되어 있습니다.
// 두 번째 라이브러리에는 컴퓨터의 하드웨어와 인터페이스하여 모니터의 새로 고침 빈도, 사용 중인 비디오 카드 등에 대한 정보를 얻는 도구가 포함되어 있습니다.
// 세 번째 라이브러리에는 셰이더 컴파일 기능이 포함되어 있습니다.
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