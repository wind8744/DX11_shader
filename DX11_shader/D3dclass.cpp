#include "D3dclass.h"
#include <iostream>
using namespace std;

D3dclass::D3dclass()
{
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;
}

D3dclass::D3dclass(const D3dclass& other)
{
}

D3dclass::~D3dclass()
{
}

//
//이 함수에 제공되는 screenWidth 및 screenHeight 변수는 SystemClass에서 생성한 창의 너비와 높이입니다.
// Direct3D는 이를 사용하여 동일한 창 크기를 초기화하고 사용합니다.
// hwnd 변수는 창에 대한 핸들입니다.Direct3D에서는 이전에 생성된 창에 액세스하려면 이 핸들이 필요합니다.
// 전체 화면 변수는 창 모드 또는 전체 화면에서 실행 중인지 여부입니다.
// Direct3D에서는 올바른 설정으로 창을 생성하기 위해 이 정보도 필요합니다.
// screenDepth 및 screenNear 변수는 창에 렌더링될 3D 환경에 대한 깊이 설정입니다.
// vsync 변수는 Direct3D가 사용자 모니터 새로 고침 빈도에 따라 렌더링할지 아니면 최대한 빠르게 렌더링할지 여부를 나타냅니다.
//
bool D3dclass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView, screenAspect;

	// Store the vsync setting.
	m_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	//이제 시스템에서 새로 고침 빈도를 얻었으므로 DirectX 초기화를 시작할 수 있습니다. 
	// 가장 먼저 할 일은 스왑 체인에 대한 설명을 작성하는 것입니다. 
	// 스왑 체인은 그래픽이 그려지는 전면 및 후면 버퍼입니다. 
	// 일반적으로 단일 백 버퍼를 사용하고 거기에 모든 그림을 그린 다음 이를 프론트 버퍼로 교체하여 사용자 화면에 표시합니다. 
	// 이것이 바로 스왑 체인이라고 불리는 이유입니다.
	// 
	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//스왑 체인 설명의 다음 부분은 새로 고침 빈도입니다. 
	// 새로 고침 빈도는 백 버퍼를 프론트 버퍼로 그리는 초당 횟수입니다. 
	// applicationclass.h 헤더에서 vsync가 true로 설정된 경우 새로 고침 빈도가 시스템 설정(예: 60hz)으로 고정됩니다.
	//  즉, 초당 60회(또는 시스템 새로 고침 빈도가 60보다 높으면 그 이상)만 화면을 그립니다. 
	// 그러나 vsync를 false로 설정하면 초당 가능한 횟수만큼 화면이 그려지지만 이로 인해 일부 시각적 아티팩트가 발생할 수 있습니다.
	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		std::cout << "D3D11CreateDeviceAndSwapChain() error" << std::endl;
		return false;
	}

	// 백버퍼 포인터 가져와서 렌더타겟 뷰 생성
	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;



	//또한 깊이 버퍼 설명을 설정해야 합니다. 이를 사용하여 다각형이 3D 공간에서 적절하게 렌더링될 수 있도록 깊이 버퍼를 만듭니다. 
	// 동시에 스텐실 버퍼를 깊이 버퍼에 연결합니다. 스텐실 버퍼를 사용하여 모션 블러, 체적 그림자 등과 같은 효과를 얻을 수 있습니다.
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//이제 해당 설명을 사용하여 깊이/스텐실 버퍼를 만듭니다. 버퍼를 만들기 위해 CreateTexture2D 함수를 사용한다는 것을 알 수 있습니다.
	// 따라서 버퍼는 단지 2D 텍스처입니다. 그 이유는 다각형이 정렬되고 래스터화되면 결국 이 2D 버퍼에서 컬러 픽셀이 되기 때문입니다. 그런 다음 이 2D 버퍼가 화면에 그려집니다.
	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 스텐실 상태 생성
	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//생성된 내용으로 이제 OMSetRenderTargets를 호출할 수 있습니다.
	// 그러면 렌더 대상 뷰와 깊이 스텐실 버퍼가 출력 렌더 파이프라인에 바인딩됩니다.
	// 이렇게 하면 파이프라인이 렌더링하는 그래픽이 이전에 만든 백 버퍼에 그려집니다.
	// 후면 버퍼에 그래픽이 기록되면 이를 전면으로 교체하고 사용자 화면에 그래픽을 표시할 수 있습니다.
	// 
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	//이제 렌더 타겟이 설정되었으므로 향후 튜토리얼에서 장면을 더 잘 제어할 수 있는 몇 가지 추가 기능을 계속할 수 있습니다.
	// 먼저 우리가 만들 것은 래스터라이저 상태입니다.이를 통해 다각형이 렌더링되는 방식을 제어할 수 있습니다.
	// 장면을 와이어프레임 모드로 렌더링하거나 DirectX가 다각형의 앞면과 뒷면을 모두 그리도록 하는 등의 작업을 수행할 수 있습니다.
	// 기본적으로 DirectX에는 이미 래스터라이저 상태가 설정되어 있고 아래 상태와 정확히 동일하게 작동하지만 직접 설정하지 않는 한 이를 변경할 수 없습니다.
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	//또한 Direct3D가 클립 공간 좌표를 렌더링 대상 공간에 매핑할 수 있도록 뷰포트를 설정해야 합니다. 이를 창의 전체 크기로 설정합니다.
	// Setup the viewport for rendering.
	m_viewport.Width = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);

	//이제 투영 행렬을 생성하겠습니다. 투영 행렬은 3D 장면을 이전에 만든 2D 뷰포트 공간으로 변환하는 데 사용됩니다. 
	// 장면을 렌더링하는 데 사용할 셰이더에 전달할 수 있도록 이 행렬의 복사본을 보관해야 합니다.
	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// 투영 행렬 만들기
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// 또한 월드 매트릭스라는 또 다른 매트릭스를 생성할 것입니다.
	// 이 행렬은 객체의 정점을 3D 장면의 정점으로 변환하는 데 사용됩니다.
	// 이 행렬은 3D 공간에서 개체를 회전, 이동 및 크기 조정하는 데에도 사용됩니다.
	// 처음부터 행렬을 단위 행렬로 초기화하고 이 객체에 복사본을 보관합니다.렌더링을 위해 복사본을 셰이더에 전달해야 합니다.
	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();


	// 뷰 매트릭스를 생성하는 곳입니다.
	// 뷰 매트릭스는 우리가 장면을 보고 있는 위치를 계산하는 데 사용됩니다.카메라라고 생각하면 되고,
	// 이 카메라를 통해서만 장면을 볼 수 있습니다  => 카메라 클래스에서 생성.
	

	// 초기화 함수에서 설정할 마지막 것은 직교 투영 행렬입니다.
	// 이 매트릭스는 화면의 사용자 인터페이스와 같은 2D 요소를 렌더링하는 데 사용되므로 3D 렌더링을 건너뛸 수 있습니다.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}


void D3dclass::Shutdown()
{
	// 포인터를 해제하기 전에 먼저 스왑 체인을 창 모드로 강제 전환하는 호출을 넣었습니다. 
	// 이 작업이 완료되지 않은 상태에서 전체 화면 모드로 스왑 체인을 해제하려고 하면 몇 가지 예외가 발생합니다. 
	// 따라서 이러한 일이 발생하지 않도록 Direct3D를 종료하기 전에 항상 창 모드를 강제 실행합니다.
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}

//각 프레임의 시작 부분에 새로운 3D 장면을 그릴 때마다 BeginScene이 호출됩니다. 
// 그것이 하는 일은 버퍼를 초기화하여 비어 있고 그릴 준비가 되도록 하는 것입니다. 
// 다른 기능은 Endscene으로, 각 프레임의 끝에서 모든 드로잉이 완료되면 스왑 체인에 3D 장면을 표시하도록 지시합니다.
void D3dclass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void D3dclass::EndScene()
{
	// 렌더링이 완료되었으므로 백 버퍼를 화면에 표시합니다. 
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

	return;
}


void D3dclass::GetProjectionMatrix(XMMATRIX& projectionMatrix) const
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void D3dclass::GetWorldMatrix(XMMATRIX& worldMatrix) const
{
	worldMatrix = m_worldMatrix;
	return;
}


void D3dclass::GetOrthoMatrix(XMMATRIX& orthoMatrix) const
{
	orthoMatrix = m_orthoMatrix;
	return;
}


void D3dclass::GetVideoCardInfo(char* cardName, int& memory) const
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}


void D3dclass::SetBackBufferRenderTarget()
{
	// 렌더 대상 뷰와 깊이 스텐실 버퍼를 출력 렌더 파이프라인에 바인딩합니다. 
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return;
}


void D3dclass::ResetViewport()
{
	// Set the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);

	return;
}