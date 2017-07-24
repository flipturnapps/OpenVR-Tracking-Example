#include "cylinder.h"

//Destructor
Cylinder::~Cylinder()
{

}

//Constructor
Cylinder::Cylinder()
{
	hasInit = false;
	s1 = new float[3];
	s2 = new float[3];
	s1[1] = std::numeric_limits<float>::max();
	s2[1] = -std::numeric_limits<float>::max();
	yMin = -std::numeric_limits<float>::max();
	yMax = std::numeric_limits<float>::max();
	radius = 0;
}

void Cylinder::init()
{
	//The center is set to the midpoint between the two trigger button events
	xOrigin = (s1[0] + s2[0])/2;
	zOrigin = (s1[2] + s2[2])/2;

	//Calculates the radius based on the distance between the two base points
	float deltaX = s1[0]-s2[0];
	float deltaZ = s1[2]-s2[2];
	radius= std::sqrt((deltaX*deltaX) + (deltaZ*deltaZ)) / 2;

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
	float testDist = std::sqrt(((dX*dX) + (dZ*dZ)));
	return (testDist <= radius);
}