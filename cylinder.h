#ifndef CYLINDER_H_
#define CYLINDER_H_

class Cylinder 
{


	
	

public:
	~Cylinder();
	Cylinder();
	void init();
	bool isInside(float x, float y, float z);

	float x1;
	float z1;
	float x2;
	float z2;
	float xOrigin;
	float zOrigin;
	float radiusSquared;

	bool hasInit;
};



#endif