/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "point_light.h"
#include <stdlib.h>

PointLight::PointLight() : Light{ true } {}

PointLight::PointLight(Vertex pos, Colour col) : Light{ true } {
	position = pos;
	intensity = col;
}

bool PointLight::get_direction(Vertex& surface, Vector& dir) {
	dir.x = surface.x - position.x;
	dir.y = surface.y - position.y;
	dir.z = surface.z - position.z;
	dir.normalise();
	return true;
}

void PointLight::get_intensity(Vertex& surface, Colour& level) {
	level = intensity;
}

//Generates a random photon in any direction originating at this light
void PointLight::gen_photon(Photon& photon) {
	photon.ray.position = position;
	photon.ray.direction.x = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
	photon.ray.direction.y = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
	photon.ray.direction.z = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
	photon.ray.direction.normalise();
	photon.type = Photon::Type::direct;
	photon.intensity = intensity;
}


void PointLight::gen_photon_at(Photon& photon, Vertex& point) {
	photon.ray.position = position;
	photon.ray.direction.x = point.x - position.x;
	photon.ray.direction.y = point.y - position.y;
	photon.ray.direction.z = point.z - position.z;
	photon.ray.direction.normalise();
	photon.type = Photon::Type::direct;
	photon.intensity = intensity;
}