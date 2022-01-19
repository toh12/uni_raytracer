#pragma once

#include "vector.h"
#include "object.h"

//Object representing an infinite plane defined by a normal and an offset d
class Plane : public Object {
public:
    Vector normal;
    float d{ 0.0f };
    Plane() = default;
    Plane(Vector normal, float d = 0.0f) : normal{ normal }, d{ d }{ normal.normalise(); }

    virtual void intersection(const Ray& ray, Hit& hit) override {
        hit.flag = false;

        //Calculate is there is an intersection
        if (normal.dot(ray.direction) > -0.0001f) { //no plane intersection
            return;
        }
        Vector rpos{ ray.position.x, ray.position.y, ray.position.z };
        hit.t = -(normal.dot(rpos) + d) / normal.dot(ray.direction);
        if (hit.t < 0.000000001f) { //behind ray origin
            return;
        }

        //Calculate intersection point
        hit.flag = true;
        hit.what = this;
        hit.position.x = ray.position.x + (hit.t * ray.direction.x);
        hit.position.y = ray.position.y + (hit.t * ray.direction.y);
        hit.position.z = ray.position.z + (hit.t * ray.direction.z);
        hit.normal.x = normal.x;
        hit.normal.y = normal.y;
        hit.normal.z = normal.z;
	}
};