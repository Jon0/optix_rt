/*
 * Texture.cpp
 *
 *  Created on: 24/03/2014
 *      Author: remnanjona
 */

#include <stdlib.h>

#include "Texture.h"

namespace std {

Texture::Texture() {
	// TODO Auto-generated constructor stub
	width = 1536;
	height = 864;

	int *dt = new int [width*height];
	for (int i = 0; i < width*height; ++i) dt[i] = 0xff000000 | rand();

	glEnable (GL_TEXTURE_RECTANGLE_NV);
	createTexture(dt);

	delete dt;
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

void Texture::createTexture(int *image_data) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, texture);
	glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	// select modulate to mix texture with color for shading
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void Texture::updateTexture(int *image_data) {
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, texture);
	glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
}

bool Texture::containsPoint(int x, int y) {
	return 0 <= x && x < width && 0 <= y && y < height;
}

void Texture::setPixel(int *image_data, int x, int y, int color) {
	if ( containsPoint(x, y) ) {
		image_data[x + width * y] = color;
	}
}

} /* namespace std */
