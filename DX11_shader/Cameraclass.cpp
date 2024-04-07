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

//Render 함수는 카메라의 위치와 회전을 사용하여 뷰 매트릭스를 만들고 업데이트합니다.
// 먼저 위쪽, 위치, 회전 등에 대한 변수를 설정합니다. 
// 그런 다음 장면의 원점에서 먼저 카메라의 x, y, z 회전을 기준으로 카메라를 회전합니다.
//  적절하게 회전되면 카메라를 3D 공간의 위치로 변환합니다.
//  위치, lookAt 및 up의 올바른 값을 사용하여 XMMatrixLookAtLH 함수를 사용하여 
// 현재 카메라 회전 및 이동을 나타내는 뷰 매트릭스를 생성할 수 있습니다.
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

	// 월드에서 카메라의 위치 설정
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// 기본적으로 카메라가 바라보는 위치 설정
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// pyr 값으로 회전 행렬 생성
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

	// 뷰행렬 생성
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}


void Cameraclass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}