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
	// ī�޶� ��ü�� �����մϴ�.
	m_Camera = new  Cameraclass;
	// ī�޶��� �ʱ� ��ġ�� �����մϴ�.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// �� ��ü�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_Model = new Modelclass;

	result = m_Model->Initialize(m_Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"�� ��ü�� �ʱ�ȭ�� �� �����ϴ�.", L"Error", MB_OK);
		return false;
	}

	// �÷� ���̴� ��ü�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_ColorShader = new Colorshaderclass;

	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"���� ���̴� ��ü�� �ʱ�ȭ�� �� �����ϴ�.", L"����", MB_OK);
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

	// ���� �����
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶� ��ġ�� ������� �� ��Ʈ���� ����
	m_Camera->Render();

	// ī�޶�� d2d ��ü���� ��, ����, ���� ����� ��������
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// ���� �� �ε��� ���۸� �׷��� ���������ο� ��ġ. �׸��� �غ�
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 3d ���� �÷����̴��� ������
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// �������� ����� ȭ�鿡 ǥ��
	m_Direct3D->EndScene();
	return true;
}
