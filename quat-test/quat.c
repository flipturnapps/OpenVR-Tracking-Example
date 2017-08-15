#include <stdio.h>
#include <math.h>

struct _quat_t
{
	float w;
	float v[3];
};
typedef struct _quat_t quat_t;

float qmag(quat_t q)
{
	float m = q.w * q.w;
	for(int n = 0; n < 3; n++)
		m += q.v[n] * q.v[n];
	return sqrt(m);
}

quat_t qnorm(quat_t q)
{
	float m = qmag(q);
	q.w = q.w/m;
	for(int n = 0; n < 3; n++)
		q.v[n] = q.v[n]/m;
	return q;
}

float vmag(float* v)
{
	float m = 0;
	for(int n = 0; n < 3; n++)
		m += v[n] * v[n];
	return sqrt(m);
}

void vnorm(float* v)
{
	float m = vmag(v);
	for(int n = 0; n < 3; n++)
		v[n] = v[n]/m;
}

quat_t qinv(quat_t q)
{
	q = qnorm(q);
	for(int n = 0; n < 3; n++)
		q.v[n] = -q.v[n];
	return q;
}

void vcross( float* out, float* v, float* u)
{
	out[0] = (v[1] * u[2]) - (v[2] * u[1]);
	out[1] = -( (v[0] * u[2]) - (v[2] * u[0]) ) ;
	out[2] = (v[0] * u[1]) - (v[1] * u[0]);
}

float vdot(float* v, float* u)
{
	float dot = v[0] * u[0] +
		v[1] * u[1] +
		v[2] * u[2];
	return dot;
}

quat_t qmult (quat_t p, quat_t q)
{
	quat_t out;
	out.w = p.w*q.w - vdot(p.v,q.v);
	float v1[3];
	float v2[3];
	float v3[3];
	for(int n = 0; n < 3; n++)
		v1[n] = q.v[n] * p.w;
	for(int n = 0; n < 3; n++)
		v2[n] = p.v[n] * q.w;
	vcross(v3,p.v,q.v);
	for(int n = 0; n < 3; n++)
		out.v[n] = v1[n] + v2[n] + v3[n];
	return out;
}

void printQuat(char* label, quat_t q)
{
	printf("Quat \"%s\" (w:%.3f i:%.3f j:%.3f k:%.3f)\n",label,q.w,q.v[0],q.v[1],q.v[2]);
}

int main()
{
	printf("Quat Test: \n");
	float theta = M_PI;
	float axis[3];
	axis[0] = 0;
	axis[1] = 0;
	axis[2] = 1;

	quat_t rotation;
	rotation.w = cos(theta/(2+0.0));
	for(int n = 0; n < 3; n++)
		rotation.v[n] = axis[n] * sin(theta/(2+0.0));

	rotation = qnorm(rotation);
	quat_t rotinv = qinv(rotation);
	quat_t point;
	point.v[0] = 5;
	point.v[1] = 5;
	point.v[2] = 0;

	//rotates the xy point 5,5 around the z axis
	quat_t out = qmult(qmult(rotation,point),rotinv);
	printQuat("qreg",rotation);	
	printQuat("point",point);
	printQuat("qinv",rotinv);
	printQuat("qout",out);

	//printf("Cross normed (i:%.3f j:%.3f k:%.3f)\n", out[0],out[1],out[2]);
	//printf("Quat (w:%.3f i:%.3f j:%.3f k:%.3f)\n",q.w,q.v[0],q.v[1],q.v[2]);
	//printf("Magnitude: %.3f\n", vmag(out));
}