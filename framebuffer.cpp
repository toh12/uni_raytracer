/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "framebuffer.h"
#include <stdlib.h>
#include <float.h>
#include <iostream>
#include <fstream>

using namespace std;

FrameBuffer::FrameBuffer(int w, int h) : width{ w }, height{ h } {
    if ((w<0)||(w>2048)||(h<0)||(h>2048)) {
        exit(-1);
    }
    framebuffer = new Pixel[w * h];
    if (!framebuffer) {
        exit(-2);
    }
    for (int i = 0; i < w*h; i += 1) {
        framebuffer[i].red   = 0.0f;
        framebuffer[i].green = 0.0f;
        framebuffer[i].blue  = 0.0f;
        framebuffer[i].depth = 0.0f;
    }
    return;
}

int FrameBuffer::plotPixel(int x, int y, float red, float green, float blue) {
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
        return -1;
    }
    framebuffer[y * width + x].red = red;
    framebuffer[y * width + x].green = green;
    framebuffer[y * width + x].blue = blue;
    return 0;
}

int FrameBuffer::plotDepth(int x, int y, float depth) {
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
        return -1;
    }
    framebuffer[y * width + x].depth = depth;
    return 0;
}

int FrameBuffer::getDepth(int x, int y, float &depth) {
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
        return -1;
    }
    depth = framebuffer[y * width + x].depth;
    return 0;
}

int FrameBuffer::getPixel(int x, int y, float &red, float &green, float &blue) {
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
        return -1;
    }
    red = this->framebuffer[y * this->width + x].red;
    green = this->framebuffer[y * this->width + x].green;
    blue = this->framebuffer[y * this->width + x].blue;
    return 0;
}

int FrameBuffer::writeRGBFile(char *filename) {
    float min = 0.0f;
    float max = 0.0f;
    ofstream outfile;
    outfile.open(filename, ofstream::binary);
    if (!outfile.is_open()) {
        return -1;
    }
    for (int i = 0; i < width * height; i += 1) {
        if (framebuffer[i].red > max) max = framebuffer[i].red;
        if (framebuffer[i].green > max) max = framebuffer[i].green;
        if (framebuffer[i].blue > max) max = framebuffer[i].blue;
        if (framebuffer[i].red < min) min = framebuffer[i].red;
        if (framebuffer[i].green < min) min = framebuffer[i].green;
        if (framebuffer[i].blue < min) min = framebuffer[i].blue;
    }
    float diff = max - min;
    if (diff == 0.0f) diff = 1.0f;
    outfile << "P6\n";
    outfile << this->width << " " << this->height << "\n255\n";

    for (int j = 0; j<  this->width*this->height; j += 1) {
        outfile << (unsigned char)(((this->framebuffer[j].red-min)/diff)*255.0);
        outfile << (unsigned char)(((this->framebuffer[j].green)/diff)*255.0);
        outfile << (unsigned char)(((this->framebuffer[j].blue)/diff)*255.0);
    }
    outfile.close();
    return 0;
}

int FrameBuffer::writeDepthFile(char *filename) {
    float max = 0;
    float min = FLT_MAX;
    ofstream outfile;
    outfile.open(filename, ofstream::binary);
    if (!outfile.is_open()) {
        return -1;
    }

    for (int i = 0; i < width*height; i += 1) {
        if (framebuffer[i].depth > max) max = framebuffer[i].depth;
        if (framebuffer[i].depth < min) min = framebuffer[i].depth;
    }
    float diff = max - min;
    if (diff == 0.0f) diff = 1.0f;

    outfile << "P6\n";
    outfile << this->width << " " << this->height << "\n255\n";
    for (int j = 0; j<  width*height; j += 1) {
        unsigned char pd = (unsigned char)(((framebuffer[j].depth-min)/diff)*255.0);
        outfile << pd << pd << pd;
    }
    outfile.close();
    return 0;
}
