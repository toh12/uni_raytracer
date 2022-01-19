#pragma once

#include "ray.h"

//Bounding box for polymeshs to reduce rendering time
class Box {
public:
	//Diagonally opposite corners of the box
	Vertex a;
	Vertex b;

	Box() = default;

	//Returns true if the given ray intersects the box. Adapted from:
	//www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	bool intersection(const Ray& ray) {
		//Get plane intersections
		float tx_min = (a.x - ray.position.x) / ray.direction.x;
		float ty_min = (a.y - ray.position.y) / ray.direction.y;
		float tz_min = (a.z - ray.position.z) / ray.direction.z;
		float tx_max = (b.x - ray.position.x) / ray.direction.x;
		float ty_max = (b.y - ray.position.y) / ray.direction.y;
		float tz_max = (b.z - ray.position.z) / ray.direction.z;
		float temp, t_max, t_min;

		//Swap min and max if needed
		if (tx_min > tx_max) {
			temp = tx_min;
			tx_min = tx_max;
			tx_max = temp;
		}
		if (ty_min > ty_max) {
			temp = ty_min;
			ty_min = ty_max;
			ty_max = temp;
		}
		if (tz_min > tz_max) {
			temp = tz_min;
			tz_min = tz_max;
			tz_max = temp;
		}

		//Check x-y face intersection and find intersection t's
		if ((tx_min > ty_max) || (ty_min > tx_max)) {
			return false;
		}
		if (tx_min > ty_min) {
			t_min = tx_min;
		}
		else {
			t_min = ty_min;
		}
		if (tx_max < ty_max) {
			t_max = tx_max;
		}
		else {
			t_max = ty_max;
		}
		//Check x-y-z surface intersection and find intersection t's
		if ((tz_min > t_max) || (t_min > tz_max)) {
			return false;
		}
		if (tz_min > t_min) {
			t_min = tz_min;
		}
		if (tz_max < t_max) {
			t_max = tz_max;
		}
		//Intersection if hits are infront of or at ray
		if (t_max < 0.0f) {
			return false;
		}
		return true;
	}

	//Updates the bounding box so that it contains the given vertex
	void update(const Vertex& v) {
		if (a.x < v.x) {
			a.x = v.x;
		}
		if (b.x > v.x) {
			b.x = v.x;
		}
		if (a.y < v.y) {
			a.y = v.y;
		}
		if (b.y > v.y) {
			b.y = v.y;
		}
		if (a.z < v.z) {
			a.z = v.z;
		}
		if (b.z > v.z) {
			b.z = v.z;
		}
	}
};