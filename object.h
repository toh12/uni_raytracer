/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "ray.h"
#include "hit.h"
#include "material.h"

// Object is the base class for objects.
class Object {
public:
	Object* next{ nullptr };
	Material* material{ nullptr };

	Object() = default;
	
	//Calculates if and how a ray hits this object
	virtual void intersection(const Ray& ray, Hit &hit) {}

	//Picks a random point on this object's surface
	virtual void randomSurfacePoint(Vertex& point) {}
};
