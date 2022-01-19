/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "vertex.h"
#include "transform.h"
#include "object.h"
#include "box.h"

typedef int TriangleIndex[3];

//Mesh object to place in scene
class PolyMesh : public Object{
	int vertex_count;
	int triangle_count;
	Box box{};
	Vertex* vertex;
	Vector* face_normal;
	Vector* vertex_normal;
	TriangleIndex* triangle;
public:
	PolyMesh(char* file);
	PolyMesh(char* file, Transform* transform);
	virtual void intersection(const Ray& ray, Hit &hit) override;
private:
	void do_construct(char* file, Transform* transform);
	void compute_face_normal(int which_triangle, Vector& normal);
	void compute_vertex_normals(void);
	void construct_box();
	void triangle_intersection(const Ray& ray, Hit& hit, int which_triangle);
	bool rayTriangleIntersect(const Ray& ray, const Vertex& v0, const Vertex& v1, const Vertex& v2, float& t);
};
