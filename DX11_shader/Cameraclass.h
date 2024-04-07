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

//DirectX 11이 장면을 어디서 어떻게 보고 있는지 알려주는 카메라 클래스가 필요합니다.
//  카메라 클래스는 카메라의 위치와 현재 회전을 추적합니다. 
// 위치 및 회전 정보를 사용하여 렌더링을 위해 HLSL 셰이더에 전달될 뷰 매트릭스를 생성합니다.
////////////////////////////////////////////////////////////////////////////////
// Class name: Cameraclass
////////////////////////////////////////////////////////////////////////////////
class Cameraclass
{
public:
	Cameraclass();
	Cameraclass(const Cameraclass&);
	~Cameraclass();

	// 카메라의 위치와 회전 설정
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();

	// 쉐이더가 렌더링에 사용할 수 있도록 카메라클래스에서 뷰 매트릭스를 얻어가게 함
	void GetViewMatrix(XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
};

#endif