#include "photonmap.h"
#include "object.h"
#include "material.h"

//Initilizes the KD-Tree from the photons in the PhotonStore
void PhotonMap::buildTree() {
	kdtree = new nanoflann::KDTreeSingleIndexAdaptor< nanoflann::L2_Simple_Adaptor<float, PhotonStore>, PhotonStore, 3>(3, data);
	kdtree->buildIndex();
}

//Performs a KNN search around the given point. Returns true if a shadow photon is found.
bool PhotonMap::search_knn(const Vertex& point) {
	vec3d[0] = point.x;
	vec3d[1] = point.y;
	vec3d[2] = point.z;
	results_knn.init(indices, dists);
	kdtree->findNeighbors(results_knn, vec3d, params);
	for (size_t i = 0; i < n; i++) {
		if (data[indices[i]].type == Photon::Type::shadow) {
			return true;
		}
	}
	return false;
}

//Performs a radius search around the given point. Returns true if a shadow photon is found.
bool PhotonMap::search_radius(const Vertex& point) {
	vec3d[0] = point.x;
	vec3d[1] = point.y;
	vec3d[2] = point.z;
	kdtree->radiusSearch(vec3d, r, results_radius, params);
	for (size_t i = 0; i < results_radius.size(); i++) {
		if (data[results_radius[i].first].type == Photon::Type::shadow) {
			return true;
		}
	}
	return false;
}

//Calculates the radiance from the knn results set
void PhotonMap::radiance_knn(const Hit& hit, const Vector& viewer, Colour& colour) {
	Colour col;
	for (size_t i = 0; i < results_knn.size(); i++) {
		if (data[indices[i]].type == Photon::Type::indirect) {
			hit.what->material->compute_light_colour(viewer, hit.normal, data[indices[i]].ray.direction, col);
			col.scale(data[indices[i]].intensity);
			colour.add(col);
		}
	}
	colour.scale(1.0f / (float)results_knn.size());
}

//Calculates the radiance from the radius results set
void PhotonMap::radiance_radius(const Hit& hit, const Vector& viewer, Colour& colour) {
	Colour col;
	for (size_t i = 0; i < results_radius.size(); i++) {
		if (data[results_radius[i].first].type == Photon::Type::indirect) {
			hit.what->material->compute_light_colour(viewer, hit.normal, data[results_radius[i].first].ray.direction, col);
			col.scale(data[results_radius[i].first].intensity);
			colour.add(col);
		}
	}
	colour.scale(1.0f / (float)results_radius.size());
}