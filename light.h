/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "photon.h"

// Light is the base class for lights.
class Light {
public:
	bool emitsPhotons{ false };
	Light* next{ nullptr };

	Light() = default;
	Light(bool emitsPhotons) : emitsPhotons{ emitsPhotons } {}

	// Get the direction towards the light at the point on the surface
	// should return true if the surface is in front of the light
	// and false if it's behind and not illuminated.
	virtual bool get_direction(Vertex &surface, Vector &dir) {
		return false;
	}

	// Get the intensity of the light in the direction of the surface
	virtual void get_intensity(Vertex &surface, Colour &intensity) {}

	// Generates a photon to be photon mapped in any lit direction.
	virtual void gen_photon(Photon& photon) {}

	// Generates a photon to be photon mapped in a specific direction.
	virtual void gen_photon_at(Photon& photon, Vertex& point) {}
};
