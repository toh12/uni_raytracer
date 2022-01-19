#pragma once

#include "ray.h"
#include "colour.h"

//Class to store information on a mapped photon
class Photon {
public:
	enum class Type { direct, indirect, shadow };
	Ray ray;
	Colour intensity;
	Type type;
};