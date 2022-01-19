/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "scene.h"
#include "polymesh.h"
#include "sphere.h"
#include "point_light.h"
#include "plane.h"
#include "phong.h"

//Creates a cornell box object
Object* new_cornell_box() {
    //Create the planes that form the walls
    Plane* floor = new Plane{ Vector{0.0f,1.0f,0.0f}, 4.0f };
    Plane* right = new Plane{ Vector{-1.0f,0.0f,0.0f}, 6.0f };
    Plane* left  = new Plane{ Vector{1.0f,0.0f,0.0f}, 6.0f };
    Plane* ceiling = new Plane{ Vector{0.0f,-1.0f,0.0f}, 6.0f };
    Plane* front = new Plane{ Vector{0.0f,0.0f,-1.0f}, 10.0f };
    Plane* back = new Plane{ Vector{0.0f,0.0f, 1.0f}, 1.0f };

    //Create colours for the walls
    Phong* white = new Phong{};
    white->ambient  = Colour{ 0.1f,0.1f,0.1f };
    white->diffuse  = Colour{ 0.5f,0.5f,0.5f };
    white->specular = Colour{ 0.3f,0.3f,0.3f };
    white->p_reflect = 0.5f;
    white->p_transmit = 0.0f;
    white->p_absorb = 0.5f;
    white->power = 40.0f;

    Phong* red = new Phong{};
    red->ambient = Colour{ 0.2f,0.0f,0.0f };
    red->diffuse = Colour{ 0.7f,0.0f,0.0f };
    red->specular = Colour{ 0.1f,0.0f,0.0f };
    red->p_reflect = 0.7f;
    red->p_transmit = 0.0f;
    red->p_absorb = 0.2f;
    red->power = 40.0f;

    Phong* green = new Phong{};
    green->ambient  = Colour{ 0.0f,0.2f,0.0f };
    green->diffuse  = Colour{ 0.0f,0.7f,0.0f };
    green->specular = Colour{ 0.0f,0.1f,0.0f };
    green->p_reflect = 0.7f;
    green->p_transmit = 0.0f;
    green->p_absorb = 0.2f;
    green->power = 40.0f;

    Phong* blue = new Phong{};
    blue->ambient = Colour{ 0.0f,0.0f,0.2f };
    blue->diffuse = Colour{ 0.0f,0.0f,0.7f };
    blue->specular = Colour{ 0.0f,0.0f,0.1f };
    blue->p_reflect = 0.7f;
    blue->p_transmit = 0.0f;
    blue->p_absorb = 0.2f;
    blue->power = 40.0f;

    //Assign colours to the walls
    floor->material = white;
    ceiling->material = white;
    front->material = white;
    back->material = blue;
    left->material = red;
    right->material = green;

    //Link the box together as one object & return it
    floor->next = ceiling;
    ceiling->next = front;
    front->next = back;
    back->next = left;
    left->next = right;
    return floor;
}

//Creates a grey shiny Phong material
Material* new_reflective_material() {
    Phong* shiny = new Phong{};
    shiny->ambient  = Colour{ 0.1f,0.1f,0.1f };
    shiny->diffuse  = Colour{ 0.1f,0.1f,0.1f };
    shiny->specular = Colour{ 0.4f,0.4f,0.4f };
    shiny->p_reflect = 0.0f;
    shiny->p_transmit = 0.8f;
    shiny->p_absorb = 0.2f;
    shiny->isDiffuse = false;
    shiny->isSpecular = true;
    shiny->isReflective = true;
    shiny->reflective_index = 0.7f;
    shiny->power = 40.0f;
    return shiny;
}

//Creates a grey seethrough Phong material 
Material* new_refractive_material() {
    Phong* tlucent = new Phong{};
    tlucent->ambient = Colour{ 0.1f,0.1f,0.1f };
    tlucent->diffuse = Colour{ 0.1f,0.1f,0.1f };
    tlucent->specular = Colour{ 0.4f,0.4f,0.4f };
    tlucent->p_reflect = 0.0f;
    tlucent->p_transmit = 0.8f;
    tlucent->p_absorb = 0.2f;
    tlucent->isDiffuse = false;
    tlucent->isSpecular = true;
    tlucent->isReflective = true;
    tlucent->isRefractive = true;
    tlucent->refraction_index = 1.52f;
    tlucent->power = 40.0f;
    return tlucent;
}

//Creates a new yellow matt Phong material
Material* new_diffuse_material() {
    Phong* matt = new Phong{};
    matt->ambient  = Colour{ 0.2f,0.2f,0.0f };
    matt->diffuse  = Colour{ 0.4f,0.4f,0.0f };
    matt->specular = Colour{ 0.4f,0.4f,0.0f };
    matt->p_reflect = 0.5f;
    matt->p_transmit = 0.0f;
    matt->p_absorb = 0.5;
    matt->power = 40.0f;
    return matt;
}

//Setup scene and run the raytracer
int main(int argc, char* argv[]) {
    //Create a framebuffer & Scene
    int width = 1280;
    int height = 1280;
    FrameBuffer* fb = new FrameBuffer{ width,height };
    Scene scene{ 0.1f, 1.75f, 0.0f, 1 };

    //Transform to move the teapot model to somewhere visible.
    Transform* transform = new Transform{
        0.5f, 0.0f, 0.0f,  0.0f,
        0.0f, 0.5f, 0.0f,  -1.0f,
        0.0f, 0.0f, 0.5f,  3.0f,
        0.0f, 0.0f, 0.0f,  1.0f
    };

    //Read in the teapot model.
    PolyMesh* teapot = new PolyMesh{ (char*)"teapot.ply", transform };
    teapot->material = new_diffuse_material();

    //Setup other objects
    Sphere* sphereL = new Sphere{ Vertex{ -3.0f, -1.8f, 6.5f }, 1.3f };
    Sphere* sphereR = new Sphere{ Vertex{  3.0f, -1.8f, 6.5f }, 1.3f };
    sphereL->material = new_reflective_material();
    sphereR->material = new_refractive_material();

    //Setup Lights
    PointLight* pointLight = new PointLight{ Vertex{0.0f, 2.0f, 0.0f}, Colour{1.0f, 1.0f, 1.0f} };

    //Setup Scene
    scene.add(pointLight);
    scene.add(new_cornell_box());
    scene.add(sphereL);
    scene.add(sphereR);
    scene.add(teapot);
                 
    //Do raytracing
    scene.init(true, true);
    scene.draw_scene(fb, width, height, Scene::Mode::full);

    //Output the framebuffer.
    fb->writeRGBFile((char *)"image.ppm");
    fb->writeDepthFile((char *)"depth.ppm");

    return 0;
}
