/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "light.h"

 // DirectionaLight is a child class of Light and implements a light
 // with constant value in a given direction. The light has no position
 // and can be treated as infinitely far away. Doesn't emit photons.
class DirectionalLight : public Light {
public:
	Vector direction;
	Colour intensity;

	DirectionalLight();
	DirectionalLight(Vector dir, Colour col);

	bool get_direction(Vertex &surface, Vector &dir);
	void get_intensity(Vertex &surface, Colour &intensity);

};
