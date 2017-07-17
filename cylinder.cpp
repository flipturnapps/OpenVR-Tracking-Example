#include "cylinder.h"

Cylinder::~Cylinder()
{

}

Cylinder::Cylinder()
{
	hasInit = false;
}

void Cylinder::init()
{
	xOrigin = (x1 + x2)/2;
	zOrigin = (z1 + z2)/2;
	float deltaX = x1-x2;
	float deltaZ = z1-z2;
	radiusSquared = (deltaX*deltaX) + (deltaZ*deltaZ);
	hasInit = true;
}

bool Cylinder::isInside(float x, float y, float z)
{
	float dX = x-xOrigin;
	float dZ = z-zOrigin;
	float testDist = (dX*dX) + (dZ*dZ);
	return (testDist <= radiusSquared);
}