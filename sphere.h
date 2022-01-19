/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "vertex.h"
#include "object.h"

//Sphere object to render in the scene
class Sphere : public Object {
	Vertex center;
	float  radius;
public:
	Sphere(Vertex c, float r);
	void intersection(const Ray& ray, Hit &hit);
	void randomSurfacePoint(Vertex& point) override;
};
