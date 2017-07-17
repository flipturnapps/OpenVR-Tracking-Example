#include "cylinder.h"

Cylinder::~Cylinder()
{

}

Cylinder::Cylinder()
{
	hasInit = false;
	s1 = new float[3];
	s2 = new float[3];
	s1[1] = std::numeric_limits<float>::max();
	s2[1] = std::numeric_limits<float>::min();
	yMin = std::numeric_limits<float>::min();
	yMax = std::numeric_limits<float>::max();
}

void Cylinder::init()
{
	xOrigin = (s1[0] + s2[0])/2;
	zOrigin = (s1[2] + s2[2])/2;
	float deltaX = s1[0]-s2[0];
	float deltaZ = s1[2]-s2[2];
	radiusSquared = (deltaX*deltaX) + (deltaZ*deltaZ);
	yMax = std::max(s1[1],s2[1]);
	yMin = std::min(s1[1],s2[1]);
	hasInit = true;
}

bool Cylinder::isInside(float x, float y, float z)
{
	if(y > yMax || y < yMin)
		return false;
	float dX = x-xOrigin;
	float dZ = z-zOrigin;
	float testDist = (dX*dX) + (dZ*dZ);
	return (testDist <= radiusSquared);
}