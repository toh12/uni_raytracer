/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "scene.h"
#include "plane.h"

using namespace std;

//Sets up scene to use depth of field with the given information
Scene::Scene(float imgPlaneDist, float focalLen, float appature, int sqrtNumSamples) :
    perpDist{ imgPlaneDist }, focalLen{ focalLen }, appature{ appature },
    sqrtNumSamples{ sqrtNumSamples }, numSamples{ sqrtNumSamples * sqrtNumSamples }{}

//Adds an object to the end of the scene's object list
void Scene::add(Object* object) {
    if (objects == nullptr) {
        objects = object;
    }
    else {
        Object* ptr = objects;
        while (ptr->next != nullptr) {
            ptr = ptr->next;
        }
        ptr->next = object;
    }
}

//Adds a light to the end of the scene's light list
void Scene::add(Light* light) {
    if (lights == nullptr) {
        lights = light;
    }
    else {
        Light* ptr = lights;
        while (ptr->next != nullptr) {
            ptr = ptr->next;
        }
        ptr->next = light;
    }
}

//Initializes the requested photon maps
void Scene::init(bool hasPhotons, bool hasCaustics) {
    if (hasPhotons) {
        gen_photonmap();
    }
    if (hasCaustics) {
        gen_causticmap();
    }
}

//Draws the scene and outputs the result to the framebuffer
void Scene::draw_scene(FrameBuffer* fb, int width, int height, Mode mode) {
    std::cout << "Beginning rayrtacing\n";

    //Declare reusable variables
    float imgDist, scale, fx, fy, depth, d, kx, ky;
    Ray ray;
    Hit imgHit;
    Vertex focalPoint;
    Colour colour, col;
    Plane imgPlane{ Vector{0.0,0.0,-1.0f}, perpDist };

    //Set the raytracer to be called
	void (Scene::* raytrace)(const Ray&, Colour&, float&, int);
    switch (mode) {
    case Scene::Mode::full:
        raytrace = &Scene::raytrace_full;
        break;
    case Scene::Mode::basic:
        raytrace = &Scene::raytrace_basic;
        break;
    case Scene::Mode::photons:
        raytrace = &Scene::raytrace_photons;
        break;
    case Scene::Mode::caustics:
        raytrace = &Scene::raytrace_caustics;
        break;
    default:
        return;
    }

    //Do the raytracing for each pixel
    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            fx = (float)x / (float)width;
            fy = (float)y / (float)height;

            //Calculate focal point for depth of field
            ray.direction.x = (fx - 0.5f);
            ray.direction.y = (0.5f - fy);
            ray.direction.z = 0.5f;
            ray.direction.normalise();
            imgPlane.intersection(ray, imgHit);
            imgDist = 0.0f;
            imgDist += imgHit.position.x * imgHit.position.x;
            imgDist += imgHit.position.y * imgHit.position.y;
            imgDist += imgHit.position.z * imgHit.position.z;
            imgDist = sqrtf(imgDist);
            scale = imgDist / (perpDist / (perpDist + focalLen));
            focalPoint.x = ray.direction.x * scale;
            focalPoint.y = ray.direction.y * scale;
            focalPoint.z = ray.direction.z * scale;

            //Sample rays from img plane to focal point and take the average
            colour.scale(0.0f);
            depth = 0.0f;
            for (int k = 0; k < numSamples; k++) {
                kx = ((float)rand() / (float)RAND_MAX);
                ky = ((float)rand() / (float)RAND_MAX);
                kx = (kx / (float)sqrtNumSamples) + (float)(k / sqrtNumSamples);
                ky = (ky / (float)sqrtNumSamples) + (float)(k % sqrtNumSamples);
                kx = 2.0f * (kx - 0.5f) * appature;
                ky = 2.0f * (ky - 0.5f) * appature;
                ray.position.x = imgHit.position.x + kx;
                ray.position.y = imgHit.position.y + ky;
                ray.position.z = imgHit.position.z;
                ray.direction.x = focalPoint.x - ray.position.x;
                ray.direction.y = focalPoint.y - ray.position.y;
                ray.direction.z = focalPoint.z - ray.position.z;
                ray.direction.normalise();
                (this->*raytrace)(ray, col, d, 10);
                depth += d;
                colour.add(col);
            }
            depth = depth / (float)numSamples;
            colour.scale(1.0f / (float)numSamples);

            //Plot result to the outputs
            fb->plotPixel(x, y, colour.r, colour.g, colour.b);
            fb->plotDepth(x, y, depth);
        }
        std::cerr << "*" << flush;
    }
    std::cout << "Rayrtacing complete\n";
}

//Finds the closest object that intersects the given ray
void Scene::object_test(const Ray& ray, Hit& best_hit) {
    Object* obj = objects;
    best_hit.flag = false;
    while (obj != 0) {
        Hit obj_hit;
        obj_hit.flag = false;
        obj->intersection(ray, obj_hit);
        if (obj_hit.flag) {
            if (obj_hit.t > 0.0f) {
                if (best_hit.flag == false) {
                    best_hit = obj_hit;
                }
                else if (obj_hit.t < best_hit.t) {
                    best_hit = obj_hit;
                }
            }
        }
        obj = obj->next;
    }
}

//Raytraces by colouring the best hit, for debugging scene layout
void Scene::raytrace_basic(const Ray& ray, Colour& colour, float& depth, int recursion_depth) {
    Hit best_hit;
    object_test(ray, best_hit);
    if (best_hit.flag) {
        //Set base colour & depth
        depth = best_hit.t;
        best_hit.what->material->compute_base_colour(colour);

        //Add light colour
        Light* light = lights;
        Vector viewer, ldir;
        Colour intensity, scaling;
        viewer.x = -best_hit.position.x;
        viewer.y = -best_hit.position.y;
        viewer.z = -best_hit.position.z;
        viewer.normalise();

        while (light != nullptr) {
            light->get_direction(best_hit.position, ldir);
            light->get_intensity(best_hit.position, scaling);
            best_hit.what->material->compute_light_colour(viewer, best_hit.normal, ldir, intensity);
            intensity.scale(scaling);
            colour.add(intensity);
            light = light->next;
        }
    }
    else {
        depth = 7.0f;
        colour.r = 0.0f;
        colour.g = 0.0f;
        colour.b = 0.0f;
    }
}

//Raytraces by flat colouring and adding caustics only, for debugging caustics
void Scene::raytrace_caustics(const Ray& ray, Colour& colour, float& depth, int recursion_depth) {
    Hit best_hit;
    object_test(ray, best_hit);
    if (best_hit.flag) {
        //Set base colour & depth
        depth = best_hit.t;
        best_hit.what->material->compute_base_colour(colour);

        //Add caustics colour
        Vector viewer;
        viewer.x = -best_hit.position.x;
        viewer.y = -best_hit.position.y;
        viewer.z = -best_hit.position.z;
        viewer.normalise();
        Colour col;
        caustics.search_knn(best_hit.position);
        caustics.radiance_knn(best_hit, viewer, col);
        colour.add(col);
    }
    else {
        depth = 7.0f;
        colour.r = 0.0f;
        colour.g = 0.0f;
        colour.b = 0.0f;
    }
}

//Raytraces by using the photon map only, for debugging indirect lighting
void Scene::raytrace_photons(const Ray& ray, Colour& colour, float& depth, int recursion_depth) {
    Hit best_hit;
    object_test(ray, best_hit);
    if (best_hit.flag) {
        //Set base depth
        depth = best_hit.t;

        //Take photon map colour as the colour
        Vector viewer;
        viewer.x = -best_hit.position.x;
        viewer.y = -best_hit.position.y;
        viewer.z = -best_hit.position.z;
        viewer.normalise();
        map.search_knn(best_hit.position);
        map.radiance_knn(best_hit, viewer, colour);
    }
    else {
        depth = 7.0f;
        colour.r = 0.0f;
        colour.g = 0.0f;
        colour.b = 0.0f;
    }
}

//Does a full raytrace of a ray using photon/causting maps and recursion
void Scene::raytrace_full(const Ray& ray, Colour& colour, float& depth, int recursion_depth) {
    // first step, find the closest primitive
    Hit shadow_hit;
    Hit best_hit;
    object_test(ray, best_hit);

    // if we found a primitive then compute the colour we should see
    if (best_hit.flag) {
        depth = best_hit.t;
        Vector viewer;
        viewer.x = -best_hit.position.x;
        viewer.y = -best_hit.position.y;
        viewer.z = -best_hit.position.z;
        viewer.normalise();

        //Add colour from indirect lighting (photon map if possible)
        bool inShadow = true;
        if (best_hit.what->material->isDiffuse) {
            inShadow = map.search_knn(best_hit.position);
            map.radiance_knn(best_hit, viewer, colour);
        }
        else {
            best_hit.what->material->compute_base_colour(colour);
        }

        //Add colour from caustics
        Colour col;
        caustics.search_knn(best_hit.position);
        caustics.radiance_knn(best_hit, viewer, col);
        colour.add(col);

        //Calculate direct lighting for each light
        Light* light = lights;
        while (light != nullptr) {
            Vector ldir;
            bool lit = light->get_direction(best_hit.position, ldir);
            if (ldir.dot(best_hit.normal) > 0.0f) {
                lit = false; //light is facing wrong way.
            }

            //Check for shadow rays if necessary
            if (lit && inShadow) {
                Ray shadow_ray;
                shadow_ray.direction.x = -ldir.x;
                shadow_ray.direction.y = -ldir.y;
                shadow_ray.direction.z = -ldir.z;
                shadow_ray.position.x = best_hit.position.x + (0.0001f * shadow_ray.direction.x);
                shadow_ray.position.y = best_hit.position.y + (0.0001f * shadow_ray.direction.y);
                shadow_ray.position.z = best_hit.position.z + (0.0001f * shadow_ray.direction.z);
                object_test(shadow_ray, shadow_hit);
                if (shadow_hit.flag == true) {
                    Vector sdir;
                    light->get_direction(shadow_hit.position, sdir);
                    if (sdir.dot(ldir) > 0.0f && shadow_hit.t < 1000000000.0f) {
                        lit = false; //there's a shadow so no lighting, if realistically close
                    }
                }
            }

            //No shadow, compute direct lighting
            if (lit) {
                Colour intensity;
                Colour scaling;
                light->get_intensity(best_hit.position, scaling);
                best_hit.what->material->compute_light_colour(viewer, best_hit.normal, ldir, intensity);
                intensity.scale(scaling);
                colour.add(intensity);
            }
            light = light->next;
        }

        // Ray recursion on specular materials
        if (recursion_depth > 0 && best_hit.what->material->isSpecular) {
            // Compute reflection ray if material supports it.
            if (best_hit.what->material->isReflective) {
                Colour reflect_colour;
                float reflect_depth;
                Ray reflect_ray;
                best_hit.normal.reflection(ray.direction, reflect_ray.direction);
                reflect_ray.position.x = best_hit.position.x + (0.0001f * reflect_ray.direction.x);
                reflect_ray.position.y = best_hit.position.y + (0.0001f * reflect_ray.direction.y);
                reflect_ray.position.z = best_hit.position.z + (0.0001f * reflect_ray.direction.z);
                raytrace_full(reflect_ray, reflect_colour, reflect_depth, recursion_depth - 1);
                float k;
                if (best_hit.what->material->isRefractive) {
                    k = best_hit.normal.fresnel(ray.direction, best_hit.what->material->refraction_index);
                }
                else {
                    k = best_hit.what->material->reflective_index;
                }
                reflect_colour.scale(k);
                colour.add(reflect_colour);
            }
            // Compute refraction ray if material supports it.
            if (best_hit.what->material->isRefractive) {
                Colour refract_colour;
                float refract_depth;
                Ray refract_ray;
                best_hit.normal.refraction(ray.direction, best_hit.what->material->refraction_index, refract_ray.direction);
                refract_ray.position.x = best_hit.position.x + (0.0001f * refract_ray.direction.x);
                refract_ray.position.y = best_hit.position.y + (0.0001f * refract_ray.direction.y);
                refract_ray.position.z = best_hit.position.z + (0.0001f * refract_ray.direction.z);
                raytrace_full(refract_ray, refract_colour, refract_depth, recursion_depth - 1);
                float k = 1 - best_hit.normal.fresnel(ray.direction, best_hit.what->material->refraction_index);
                refract_colour.scale(k);
                colour.add(refract_colour);
            }
        }
    }
    else {
        depth = 7.0f;
        colour.r = 0.0f;
        colour.g = 0.0f;
        colour.b = 0.0f;
    }
}

//Traces a photon through the scene and adds it to the given PhotonStore
void Scene::photontrace(PhotonStore& vec, Photon&& photon, Object* toHit) {
    //Test that photon has hit a desired object
    Hit best_hit;
    object_test(photon.ray, best_hit);
    if (best_hit.flag && (best_hit.what == toHit || toHit == nullptr)) {
        photon.ray.position = best_hit.position;

        //Photon is direct: add a shadow photon to the map
        if (photon.type == Photon::Type::direct) {
            Hit shadow_hit;
            Photon shadowPhoton;
            shadowPhoton.type = Photon::Type::shadow;
            shadowPhoton.ray.direction = photon.ray.direction;
            shadowPhoton.ray.position.x = best_hit.position.x + (0.0001f * photon.ray.direction.x);
            shadowPhoton.ray.position.y = best_hit.position.y + (0.0001f * photon.ray.direction.y);
            shadowPhoton.ray.position.z = best_hit.position.z + (0.0001f * photon.ray.direction.z);
            shadowPhoton.intensity.r = 0.0f;
            shadowPhoton.intensity.g = 0.0f;
            shadowPhoton.intensity.b = 0.0f;
            object_test(shadowPhoton.ray, shadow_hit);
            while (shadow_hit.flag) {
                if (shadow_hit.normal.dot(shadowPhoton.ray.direction) < -0.0001f) break;
                shadowPhoton.ray.position.x = shadow_hit.position.x + (0.0001f * shadowPhoton.ray.direction.x);
                shadowPhoton.ray.position.y = shadow_hit.position.y + (0.0001f * shadowPhoton.ray.direction.y);
                shadowPhoton.ray.position.z = shadow_hit.position.z + (0.0001f * shadowPhoton.ray.direction.z);
                object_test(shadowPhoton.ray, shadow_hit);
            }
            if (shadow_hit.flag) {
                shadowPhoton.ray.position = shadow_hit.position;
                vec.push_back(move(shadowPhoton));
            }
        }

        //Use Russian roulette to decide what happens to the photon
        float p = (float)rand() / (float)RAND_MAX;

        //Photon is diffusely reflected as a new photon
        if (p < best_hit.what->material->p_reflect) {
            Photon reflectPhoton;
            best_hit.what->material->gen_reflect_photon(photon, best_hit.normal, reflectPhoton);
            photontrace(vec, move(reflectPhoton));
        }
        //Photon is specularlly transmitted as a new photon
        else if (p < best_hit.what->material->p_reflect + best_hit.what->material->p_transmit) {
            if (best_hit.what->material->isReflective || best_hit.what->material->isRefractive) {
                Photon transmitPhoton;
                transmitPhoton.type = Photon::Type::indirect;
                transmitPhoton.intensity = photon.intensity;
                if (best_hit.what->material->isRefractive) {
                    best_hit.normal.refraction(photon.ray.direction, best_hit.what->material->refraction_index, transmitPhoton.ray.direction);
                    transmitPhoton.intensity.scale(1 - best_hit.normal.fresnel(photon.ray.direction, best_hit.what->material->refraction_index));
                }
                else {
                    best_hit.normal.reflection(photon.ray.direction, transmitPhoton.ray.direction);
                    transmitPhoton.intensity.scale(best_hit.what->material->reflective_index);
                }
                transmitPhoton.ray.position.x = best_hit.position.x + (0.00001f * transmitPhoton.ray.direction.x);
                transmitPhoton.ray.position.y = best_hit.position.y + (0.00001f * transmitPhoton.ray.direction.y);
                transmitPhoton.ray.position.z = best_hit.position.z + (0.00001f * transmitPhoton.ray.direction.z);
                photontrace(vec, move(transmitPhoton));
            }
        }
        //Photon is absorbed, no action taken
        else {}

        //Add original photon to map
        if (best_hit.what->material->isDiffuse) {
            vec.push_back(move(photon));
        }
    }
}

//Generates the global photon map by firing numPhotons from each light
void Scene::gen_photonmap() {
    //Setup
    std::cout << "Beginning photon map generation\n";
    Light* light = lights;
    int numPrint = numPhotons / 80;

    //Generate Photons
    while (light != nullptr) {
        if (light->emitsPhotons) {
            for (int i = 0; i < numPhotons; i++) {
                Photon photon;
                light->gen_photon(photon);
                photontrace(photon_data, move(photon));
                if (i % numPrint == 0) {
                    cout << "*" << flush;
                }
            }
        }
        light = light->next;
    }

    //Build Map
    map.buildTree();
    std::cout << "Photon map generation complete\n";
}

//Generates the caustic photon map by firing numPhotons from each light
//to each caustic object
void Scene::gen_causticmap() {
    //Setup
    std::cout << "Beginning caustic map generation\n";
    Light* light = lights;
    int numPrint = numCaustic / 80;

    //Generate Data
    while (light != nullptr) {
        if (light->emitsPhotons) {
            Object* object = objects;
            while (object != nullptr) {
                if (object->material->isSpecular) {
                    for (int i = 0; i < numCaustic; i++) {
                        Photon photon;
                        Vertex point;
                        object->randomSurfacePoint(point);
                        light->gen_photon_at(photon, point);
                        photontrace(caustic_data, move(photon), object);
                        if (i % numPrint == 0) {
                            cout << "*" << flush;
                        }
                    }
                }
                object = object->next;
            }
        }
        light = light->next;
    };

    //Build Map
    caustics.buildTree();
    std::cout << "Caustic map generation complete\n";
}
