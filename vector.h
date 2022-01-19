/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include <math.h>

//Class to store and manipulate vectors
class Vector {
public:
	float x;
	float y;
	float z;

	Vector(float px, float py, float pz) {
		x = px;
		y = py;
		z = pz;
	}

	Vector() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	void normalise() {
		float len = sqrtf(x*x + y*y + z*z);
		x = x / len;
		y = y / len;
		z = z / len;
	}

	float len_sqr() const {
		return(x*x + y*y + z*z);
	}

	float length() const {
		return sqrtf(x*x + y*y + z*z);
	}

	float dot(const Vector& other) const {
		return x*other.x + y*other.y + z*other.z;
	}


	void reflection(const Vector& initial, Vector &reflect) const {
		float d;

		d = dot(initial);
		d = d * 2.0f;

		reflect.x = initial.x - d * x;
		reflect.y = initial.y - d * y;
		reflect.z = initial.z - d * z;
	}

	void refraction(const Vector& initial, float index, Vector& refract) const {
		float cosi = dot(initial);
		if (cosi < 0.0f) {
			cosi = -cosi;
			float inv = 1.0f / index;
			float cost = 1.0f - ((inv * inv) * (1.0f - (cosi * cosi)));
			if (cost < 0.0f) {
				refract.x = 0;
				refract.y = 0;
				refract.z = 0;
			}
			else {
				cost = sqrtf(cost);
				refract.x = (inv * initial.x) - ((cost - (inv * cosi)) * x);
				refract.y = (inv * initial.y) - ((cost - (inv * cosi)) * y);
				refract.z = (inv * initial.z) - ((cost - (inv * cosi)) * z);
			}
		}
		else {
			float inv = index;
			float cost = 1.0f - ((inv * inv) * (1.0f - (cosi * cosi)));
			if (cost < 0.0f) {
				refract.x = 0;
				refract.y = 0;
				refract.z = 0;
			}
			else {
				cost = sqrtf(cost);
				refract.x = (inv * initial.x) - ((cost - (inv * cosi)) * (-x));
				refract.y = (inv * initial.y) - ((cost - (inv * cosi)) * (-y));
				refract.z = (inv * initial.z) - ((cost - (inv * cosi)) * (-z));
			}
		}
	}

	float fresnel(const Vector& initial, float index) const {
		float inv = 1.0f / index;
		float cosi = dot(initial);
		if (cosi < 0.0f) {
			cosi = -cosi;
		}
		float cost = 1.0f - ((inv * inv) * (1.0f - (cosi * cosi)));
		if (cost < 0.0f) {
			return 0;
		}
		cost = sqrtf(cost);
		float par = ((index * cosi) - cost) / ((index * cosi) + cost);
		float per = (cosi - (index * cost)) / (cosi + (index * cost));
		return ((par * par) + (per * per)) / 2.0f;
	}

	void negate() {
		x = -x;
		y = -y;
		z = -z;
	}

	void cross(const Vector &other, Vector &result) const {
		result.x = y*other.z - z*other.y;
		result.y = z*other.x - x*other.z;
		result.z = x*other.y - y*other.x;
	}

	void cross(Vector &other) {
		Vector result;
		result.x = y*other.z - z*other.y;
		result.y = z*other.x - x*other.z;
		result.z = x*other.y - y*other.x;
		x = result.x;
		y = result.y;
		z = result.z;
	}

	void add(const Vector &other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void sub(const Vector &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	Vector& operator=(Vector other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	friend Vector operator+(const Vector &a, const Vector &b) {
		Vector t;
		t.x = a.x + b.x;
		t.y = a.y + b.y;
		t.z = a.z + b.z;
		return t;
	}

	friend Vector operator-(const Vector &a, const Vector &b) {
		Vector t;
		t.x = a.x - b.x;
		t.y = a.y - b.y;
		t.z = a.z - b.z;
		return t;
	}

    friend Vector operator*(const Vector &a, const Vector &b) {
		Vector t;
		t.x = a.x * b.x;
		t.y = a.y * b.y;
		t.z = a.z * b.z;
		return t;
	}

	friend Vector operator*(const float a, const Vector &b) {
		Vector t;
		t.x = a * b.x;
		t.y = a * b.y;
		t.z = a * b.z;
		return t;
	}
};