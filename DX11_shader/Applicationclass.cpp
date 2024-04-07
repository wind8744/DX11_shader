#include "Applicationclass.h"

Applicationclass::Applicationclass()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_ColorShader = nullptr;
}

Applicationclass::Applicationclass(const Applicationclass& other)
{
}

Applicationclass::~Applicationclass()
{
}

bool Applicationclass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	// Create and initialize the Direct3D object.
	m_Direct3D = new D3dclass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	// 카메라 객체를 생성합니다.
	m_Camera = new  Cameraclass;
	// 카메라의 초기 위치를 설정합니다.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// 모델 객체를 생성하고 초기화합니다.
	m_Model = new Modelclass;

	result = m_Model->Initialize(m_Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"모델 개체를 초기화할 수 없습니다.", L"Error", MB_OK);
		return false;
	}

	// 컬러 셰이더 객체를 생성하고 초기화합니다.
	m_ColorShader = new Colorshaderclass;

	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"색상 셰이더 개체를 초기화할 수 없습니다.", L"오류", MB_OK);
		return false;
	}
	return true;
}

void Applicationclass::Shutdown()
{
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

bool Applicationclass::Frame()
{
	bool result;
	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool Applicationclass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// 버퍼 지우기
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라 위치를 기반으로 뷰 매트릭스 생성
	m_Camera->Render();

	// 카메라와 d2d 객체에서 뷰, 월드, 투영 행렬을 가져오기
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 정점 및 인덱스 버퍼를 그래픽 파이프라인에 배치. 그리기 준비
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 3d 모델을 컬러쉐이더로 렌더링
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// 렌더링된 장면을 화면에 표시
	m_Direct3D->EndScene();
	return true;
}
