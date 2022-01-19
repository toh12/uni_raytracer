#pragma once

#include "nanoflann.hpp"
#include "photon.h"
#include "hit.h"
#include <vector>

//Class to store photons in a way the Nanoflann library can access
class PhotonStore : public std::vector<Photon> {
public:
	//Nanoflann calls to get number of photons
	inline size_t kdtree_get_point_count() const { return size(); }

	//Nanoflann calls to get the value photon idx in the dim'th dimension
	inline float kdtree_get_pt(const size_t idx, const size_t dim) const {
		if (dim == 0) {
			return this->at(idx).ray.position.x;
		}
		else if (dim == 1) {
			return this->at(idx).ray.position.y;
		}
		else if (dim == 2) {
			return this->at(idx).ray.position.z;
		}
		else {
			return 0.0f;
		}
	}

	//Nanoflann calls to get a bounding box, returns false for default
	template <class BBOX> bool kdtree_get_bbox(BBOX& bb) const { return false; }
};

//Class to lookup photons in a kd-tree by wrapping the Nanoflann library
class PhotonMap {
	const size_t n{ 500 }; //value of N for KNN search
	const float r{ 0.05f }; //value of radius for radius search
public:
	//Map setup and storage
	PhotonStore& data;
	PhotonMap(PhotonStore& data) : data{ data } {}
	void buildTree();

	//Tree search methods
	bool search_knn(const Vertex& point);
	bool search_radius(const Vertex& point);
	void radiance_knn(const Hit& hit, const Vector& viewer, Colour& colour);
	void radiance_radius(const Hit& hit, const Vector& viewer, Colour& colour);
private:
	//3D kd-tree that maps to the photon store
	nanoflann::KDTreeSingleIndexAdaptor< nanoflann::L2_Simple_Adaptor<float, PhotonStore>, PhotonStore, 3>* kdtree{ nullptr };

	//Storage for Nanoflann kd-tree search results
	float* vec3d{ new float[3] };
	size_t* indices{ new size_t[n] };
	float* dists{ new float[n] };
	std::vector<std::pair<size_t, float>> results_radius;
	nanoflann::KNNResultSet<float> results_knn{ n };
	nanoflann::SearchParams params{};
};