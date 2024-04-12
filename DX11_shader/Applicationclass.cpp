#include "Applicationclass.h"
#include "Inputclass.h"

Applicationclass::Applicationclass()
{
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	//m_ColorShader = nullptr;
	//m_TextureShader = 0;
	m_LightShader = nullptr;
	m_Light = nullptr;
}

Applicationclass::Applicationclass(const Applicationclass& other)
{
}

Applicationclass::~Applicationclass()
{
}

bool Applicationclass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char textureFilename[128];
	char modelFilename[129];
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
	m_Camera->SetPosition(0.0f, 10.0f, -50.0f);

	// �� ��ü�� �����ϰ� �ʱ�ȭ�մϴ�.
	m_Model = new Modelclass;

	strcpy_s(modelFilename, "../resource/Car.obj");
	auto ret = Modelclass::ReadObjModel(modelFilename);
	//m_Model->LoadObjModel(modelFilename); // make obj to txt

	//strcpy_s(modelFilename, "../resource/objTest.txt"); //strcpy_s(modelFilename, "../resource/cube.txt");
	strcpy_s(textureFilename, "../resource/stone01.tga");
	//result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext()
	//	,modelFilename, textureFilename);

	if (!result)
	{
		MessageBox(hwnd, L"�� ��ü�� �ʱ�ȭ�� �� �����ϴ�.", L"Error", MB_OK);
		return false;
	}

	//�ؽ��� ���̴� ��ü ����
	m_LightShader = new Lightshaderclass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	m_Light = new Lightclass;

	m_Light->SetDiffuseColor(1.f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	// �÷� ���̴� ��ü�� �����ϰ� �ʱ�ȭ�մϴ�.
	//m_ColorShader = new Colorshaderclass;

	//result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	//if(!result)
	//{
	//	MessageBox(hwnd, L"���� ���̴� ��ü�� �ʱ�ȭ�� �� �����ϴ�.", L"����", MB_OK);
	//	return false;
	//}

	return true;
}

void Applicationclass::Shutdown()
{
	if (m_Light)
	{
		delete(m_Light);
		m_Light = nullptr;
	}
	// Release the texture shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	//// Release the color shader object.
	//if (m_ColorShader)
	//{
	//	m_ColorShader->Shutdown();
	//	delete m_ColorShader;
	//	m_ColorShader = 0;
	//}

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

void Applicationclass::Input()
{
	auto Pos = m_Camera->GetPosition();
	float MoveSpeed = 0.1f;
	float fDT = 1;
	if (KEY_CHECK(VK_UP))
	{
		Pos.y += MoveSpeed * fDT;
	}
	if (KEY_CHECK(VK_DOWN))
	{
		Pos.y -= MoveSpeed * fDT;
	}
	if (KEY_CHECK(VK_LEFT))
	{
		Pos.x -= MoveSpeed * fDT;
	}
	if (KEY_CHECK(VK_RIGHT))
	{
		Pos.x += MoveSpeed * fDT;
	}
	if (KEY_CHECK(VK_SPACE))
	{
		Pos.z -= MoveSpeed * fDT;
	}

	m_Camera->SetPosition(Pos.x,Pos.y,Pos.z);
}

bool Applicationclass::Frame()
{
	static float rotation = 0.0f;
	bool result;

	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.9f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool Applicationclass::Render(float rotation)
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

	// ȸ������ŭ �ﰢ���� ȸ���ϵ��� �����Ʈ���� ȸ��
	worldMatrix = XMMatrixRotationY(rotation);

	// ���� �� �ε��� ���۸� �׷��� ���������ο� ��ġ. �׸��� �غ�
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 3d ���� �÷����̴��� ������
	//result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	//if (!result)
	//{
	//	return false;
	//}
	
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// �������� ����� ȭ�鿡 ǥ��
	m_Direct3D->EndScene();
	return true;
}
