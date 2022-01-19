/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

//Class to store vertices
class Vertex {
public:
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };
	float w{ 1.0f };

	Vertex() = default;
	Vertex(float x, float y, float z) : x{ x }, y{ y }, z{ z }{}
	Vertex(float x, float y, float z, float w) : x{ x }, y{ y }, z{ z }, w{ w }{}
};
