/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "photon.h"

 // Material is the base class for materials.
class Material {
public:
	float p_reflect { 0.0f };
	float p_transmit { 0.0f };
	float p_absorb{ 1.0f };
	bool isDiffuse{ true };
	bool isSpecular{ false };
	bool isReflective{ false };
	bool isRefractive{ false };
	float refraction_index{ 1.0 };
	float reflective_index{ 1.0 };

	//Returns the base colour to be used in the absense of photon mapping
	virtual void compute_base_colour(Colour &result) {
		result.r = 0.0f;
		result.g = 0.0f;
		result.b = 0.0f;
	}

	//Calculates the reflected colour scaling based on the BRDF
	virtual void compute_light_colour(const Vector &viewer, const Vector &normal, const Vector &ldir, Colour &result) {
		result.r = 0.0f;
		result.g = 0.0f;
		result.b = 0.0f;
	}

	//Diffusely reflects a photon in a random direction around a given normal
	virtual void gen_reflect_photon(const Photon& photon, const Vector& normal, Photon& result) {
	}
};
