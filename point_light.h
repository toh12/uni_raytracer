

#pragma once
#include "light.h"

//Light that emits an equal intensity at all directions from a point
class PointLight : public Light {
public:
	Vertex position;
	Colour intensity;

	PointLight();
	PointLight(Vertex pos, Colour col);

	virtual bool get_direction(Vertex& surface, Vector& dir) override;
	virtual void get_intensity(Vertex& surface, Colour& intensity) override;
	virtual void gen_photon(Photon& photon) override;
	virtual void gen_photon_at(Photon& photon, Vertex& point) override;
};
