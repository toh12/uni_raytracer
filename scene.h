/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "photonmap.h"
#include "object.h"
#include "light.h"
#include "framebuffer.h"

// Scene is a class that is used to build a scene database of objects
// and lights and then trace a ray through it.
class Scene {
	//Photon Mapping data
	PhotonStore photon_data;
	PhotonMap map{ photon_data };
	PhotonStore caustic_data;
	PhotonMap caustics{ caustic_data };
	int numPhotons{ 100000 };
	int numCaustic{ 1000000 };

	//Depth of field variables
	float perpDist = 0.1f;
	float focalLen = 4.0f;
	float appature = 0.0f;
	int numSamples = 1;
	int sqrtNumSamples = 1;

public:
	//Scene
	Object* objects{ nullptr };
	Light* lights{ nullptr };
	enum class Mode	{
		full,basic,photons,caustics
	};

	//Scene Setup methods
	Scene() = default;
	Scene(float imgPlaneDist, float focalLen, float appature, int sqrtNumSamples);
	void add(Object* object);
	void add(Light* light);

	//Scene drawing methods
	void init(bool hasPhotons = true, bool hasCaustics = true); 
	void draw_scene(FrameBuffer* fb, int width, int height, Mode mode); 

private:
	//Raytracing methods
	void object_test(const Ray& ray, Hit& best_hit);
	void raytrace_basic(const Ray& ray, Colour& colour, float& depth, int recursion_depth);
	void raytrace_caustics(const Ray& ray, Colour& colour, float& depth, int recursion_depth);
	void raytrace_photons(const Ray& ray, Colour& colour, float& depth, int recursion_depth);
	void raytrace_full(const Ray& ray, Colour& colour, float& depth, int recursion_depth);

	//Photon Mapping methods
	void photontrace(PhotonStore& vec, Photon&& photon, Object* toHit = nullptr);
	void gen_photonmap();
	void gen_causticmap();
};
