/*
 * Texture.h
 *
 *  Created on: 24/03/2014
 *      Author: remnanjona
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>

namespace std {

class Texture {
public:
	int height, width;
	GLuint texture;

	Texture();
	virtual ~Texture();

	void createTexture(int *image_data);

	void updateTexture(int *image_data);

	bool containsPoint(int x, int y);

	void setPixel(int *image_data, int x, int y, int color);

	void draw(int x, int y, int z);
};

} /* namespace std */

#endif /* TEXTURE_H_ */
