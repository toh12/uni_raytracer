/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "directional_light.h"

DirectionalLight::DirectionalLight() : Light{} {}

DirectionalLight::DirectionalLight(Vector dir, Colour col) : Light{} {
	direction = dir;
	direction.normalise();
	intensity = col;
}

bool DirectionalLight::get_direction(Vertex &surface, Vector &dir) {
	dir = direction;
	return true;
}

void DirectionalLight::get_intensity(Vertex &surface, Colour &level) {
	level = intensity;
}
