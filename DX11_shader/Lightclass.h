#pragma once

#ifndef _Lightclass_H_
#define _Lightclass_H_

#include <directxmath.h>
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// 빛의 방향과 색상을 결정하는 클래스 (광원)
////////////////////////////////////////////////////////////////////////////////
class Lightclass
{
public:
	Lightclass();
	Lightclass(const Lightclass&);
	~Lightclass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
};

#endif