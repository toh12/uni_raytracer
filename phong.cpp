/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "phong.h"

#include <math.h>

// A simple Phong based lighting model

void Phong::compute_base_colour(Colour &result) {
	result.r = ambient.r;
	result.g = ambient.g;
	result.b = ambient.b;
}

void Phong::compute_light_colour(const Vector &viewer, const Vector &normal, const Vector &ldir, Colour &result) {
	float diff;
	Vector tolight;
	Vector toviewer;

	result.r=0.0f;
	result.g=0.0f;
	result.b=0.0f;

	tolight = ldir;
	tolight.negate();

	toviewer = viewer;
	toviewer.negate();

	diff = normal.dot(tolight);
	
	if (diff < 0.0f) { // light is behind surface
		return;
	}

	// diffuse
	result.r += diffuse.r * diff;
	result.g += diffuse.g * diff;
	result.b += diffuse.b * diff;

	// the specular component
	Vector r;
	
	normal.reflection(tolight, r);
	r.normalise();

	float h;
	h = r.dot(toviewer);

	if (h > 0.0f) {
		float p = (float)pow(h, power);

		result.r += specular.r * p;
		result.g += specular.g * p;
		result.b += specular.b * p;
	}
}

//Generates a photon in a random direction within 90 degrees of the normal
void Phong::gen_reflect_photon(const Photon& photon, const Vector& normal, Photon& result) {
	result.type = Photon::Type::indirect;
	do {
		result.ray.direction.x = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		result.ray.direction.y = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		result.ray.direction.z = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
	} while (normal.dot(result.ray.direction) < 0.0001f);
	result.ray.direction.normalise();
	compute_light_colour(result.ray.direction, normal, photon.ray.direction, result.intensity);
	result.ray.position.x = photon.ray.position.x + (0.0001f * result.ray.direction.x);
	result.ray.position.y = photon.ray.position.y + (0.0001f * result.ray.direction.y);
	result.ray.position.z = photon.ray.position.z + (0.0001f * result.ray.direction.z);
	result.intensity.scale(photon.intensity);
}

