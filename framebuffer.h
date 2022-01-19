/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

typedef struct {
  float red;
  float green;
  float blue;
  float depth;
}Pixel;

//Stores the raytracing output (unedited)
class FrameBuffer {
	int width;
	int height;
	Pixel *framebuffer;
 public:
	FrameBuffer(int w, int h);
	int plotPixel(int x, int y, float red, float green, float blue);
	int plotDepth(int x, int y, float depth);
	int getPixel(int x, int y, float &red, float &green, float &blue);
	int getDepth(int x, int y, float &depth);
	int writeRGBFile(char *filename);
	int writeDepthFile(char *filename);
};
