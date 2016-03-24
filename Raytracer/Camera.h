#pragma once
#include "GenVector.h"
class Camera
{
public:
	Camera(Vector3 &look,Vector3 &up, Vector3 &pos);
	~Camera();
	Vector3 w, u, v,e;
private:

};

Camera::Camera(Vector3 &look, Vector3 &up, Vector3 &pos):e(pos),
w(-(look-pos).normalize()),u(up.cross(w)),v(w.cross(u))
{//-(look - pos)
}

Camera::~Camera()
{
}