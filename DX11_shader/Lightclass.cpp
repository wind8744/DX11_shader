#include "Lightclass.h"
Lightclass::Lightclass()
{
}
Lightclass::Lightclass(const Lightclass& other)
{
}
Lightclass::~Lightclass()
{
}

void Lightclass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void Lightclass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

XMFLOAT4 Lightclass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 Lightclass::GetDirection()
{
	return m_direction;
}
