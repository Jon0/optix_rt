/*
 * Renderer.h
 *
 *  Created on: 24/03/2014
 *      Author: remnanjona
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../buffer/Buffer.h"
#include "../buffer/Texture.h"
#include "../buffer/Octree.h"

namespace std {

struct texvec {
	glm::vec4 pos;
	glm::vec2 texCoor;
};

class Renderer {
public:
	Renderer();
	virtual ~Renderer();

	void draw();

private:
	Pipeline tex_pipeline;
	Pipeline comp_pipeline;

	Buffer<texvec> buff;
	Texture t;
	Octree tree;
};

} /* namespace std */

#endif /* RENDERER_H_ */
