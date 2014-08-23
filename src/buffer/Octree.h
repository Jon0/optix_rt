/*
 * Octree.h
 *
 *  Created on: 20/11/2013
 *      Author: remnanjona
 */

#ifndef OCTREE_H_
#define OCTREE_H_



#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Buffer.h"

namespace std {

struct Node {
	int child[8];
	int parent;
};

struct OctreeBuffer {
	int total_size;
	int brick_size;
	Node n[20000];
};

struct uc_rgba {
	unsigned char r, g, b, a;
};

class Octree {
public:
	GLuint addr;
	unsigned int levels;

	Octree(unsigned int);
	virtual ~Octree();

	int genNode(glm::ivec3, glm::ivec3, int);

	uc_rgba *getNode(glm::ivec3);

	void bind(GLuint);

private:
	Buffer<OctreeBuffer> ob;
	int free_ind;

	uc_rgba *image_data;
};

} /* namespace std */
#endif /* OCTREE_H_ */
