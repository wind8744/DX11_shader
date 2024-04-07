#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Cameraclass_H_
#define _Cameraclass_H_

//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
using namespace DirectX;

//DirectX 11�� ����� ��� ��� ���� �ִ��� �˷��ִ� ī�޶� Ŭ������ �ʿ��մϴ�.
//  ī�޶� Ŭ������ ī�޶��� ��ġ�� ���� ȸ���� �����մϴ�. 
// ��ġ �� ȸ�� ������ ����Ͽ� �������� ���� HLSL ���̴��� ���޵� �� ��Ʈ������ �����մϴ�.
////////////////////////////////////////////////////////////////////////////////
// Class name: Cameraclass
////////////////////////////////////////////////////////////////////////////////
class Cameraclass
{
public:
	Cameraclass();
	Cameraclass(const Cameraclass&);
	~Cameraclass();

	// ī�޶��� ��ġ�� ȸ�� ����
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();

	// ���̴��� �������� ����� �� �ֵ��� ī�޶�Ŭ�������� �� ��Ʈ������ ���� ��
	void GetViewMatrix(XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
};

#endif