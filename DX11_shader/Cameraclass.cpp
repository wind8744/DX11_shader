#include "Cameraclass.h"
Cameraclass::Cameraclass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


Cameraclass::Cameraclass(const Cameraclass& other)
{
}


Cameraclass::~Cameraclass()
{
}


void Cameraclass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void Cameraclass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


XMFLOAT3 Cameraclass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


XMFLOAT3 Cameraclass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

//Render �Լ��� ī�޶��� ��ġ�� ȸ���� ����Ͽ� �� ��Ʈ������ ����� ������Ʈ�մϴ�.
// ���� ����, ��ġ, ȸ�� � ���� ������ �����մϴ�. 
// �׷� ���� ����� �������� ���� ī�޶��� x, y, z ȸ���� �������� ī�޶� ȸ���մϴ�.
//  �����ϰ� ȸ���Ǹ� ī�޶� 3D ������ ��ġ�� ��ȯ�մϴ�.
//  ��ġ, lookAt �� up�� �ùٸ� ���� ����Ͽ� XMMatrixLookAtLH �Լ��� ����Ͽ� 
// ���� ī�޶� ȸ�� �� �̵��� ��Ÿ���� �� ��Ʈ������ ������ �� �ֽ��ϴ�.
void Cameraclass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// ���忡�� ī�޶��� ��ġ ����
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// �⺻������ ī�޶� �ٶ󺸴� ��ġ ����
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// pyr ������ ȸ�� ��� ����
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// ����� ����
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


void Cameraclass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}