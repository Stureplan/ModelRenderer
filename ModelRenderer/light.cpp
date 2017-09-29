#include "light.h"

Light::Light()
{

}

Light::~Light()
{

}

LIGHT Light::GetLight()
{
	return light;
}

void Light::Create(XMFLOAT4 position, XMFLOAT4 color, float intensity)
{
	light.POS_X = position.x; light.COL_R = color.x;
	light.POS_Y = position.y; light.COL_G = color.y;
	light.POS_Z = position.z; light.COL_B = color.z;
	light.POS_W = position.w; light.COL_A = color.w;

	light.INTENSITY = intensity;
}

void Light::Move(XMFLOAT4 position)
{
	light.POS_X = position.x;
	light.POS_Y = position.y;
	light.POS_Z = position.z;
	light.POS_W = position.w;

}

XMMATRIX Light::Matrix()
{
	return XMMatrixIdentity() * XMMatrixTranslation(light.POS_X, light.POS_Y, light.POS_Z);
}