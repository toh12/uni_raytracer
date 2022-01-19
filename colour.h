/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

 // Colour is a class to store and maniplulate an rgb colour.
class Colour {
public:
	float r{ 0.0f };
	float g{ 0.0f };
	float b{ 0.0f };

	Colour() = default;
	Colour(float r, float g, float b) : r{ r }, g{ g }, b{ b } {}

	void scale(const Colour &scaling) {
		r *= scaling.r;
		g *= scaling.g;
		b *= scaling.b;
	}

	void scale(float scaling) {
		r *= scaling;
		g *= scaling;
		b *= scaling;
	}

	void add(const Colour &adjust) {
		r += adjust.r;
		g += adjust.g;
		b += adjust.b;
	}
};
