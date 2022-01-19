/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "vertex.h"
#include "vector.h"
#include <iostream>

 // Ray is a class to store and maniplulate 3D rays.
class Ray {
public:
	Vertex position;
	Vector direction;

	Ray() = default;	
	Ray(Vertex p, Vector d) : position{ p }, direction{ d } {}

	friend std::ostream& operator<<(std::ostream& os, const Ray& r) {
		os << "Ray{[" << r.position.x << "," << r.position.y << "," << r.position.z << "],[" << r.direction.x << "," << r.direction.y << "," << r.direction.z << "]}\n"; 
        return os;
	}
};
