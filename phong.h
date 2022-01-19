/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "material.h"
#include "colour.h"
#include "vector.h"

// Phong is a child class of Material and implement the simple Phong
// surface illumination model.
class Phong : public Material {
public:
	Colour ambient;
	Colour diffuse;
	Colour specular;
	float  power{ 0.0f };

	virtual void compute_base_colour(Colour &result) override;
	virtual void compute_light_colour(const Vector &viewer, const Vector &normal, const Vector &ldir, Colour &result) override; 
	virtual void gen_reflect_photon(const Photon& photon, const Vector& normal, Photon& result) override;
};
