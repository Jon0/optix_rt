/*
 * Texture.cpp
 *
 *  Created on: 20/11/2013
 *      Author: remnanjona
 */

#include <iostream>
#include <stdlib.h>
#include <glm/ext.hpp>

#include "Octree.h"

namespace std {

void makeSphere(uc_rgba *image_data, int levels, glm::vec3 mid, float radius) {
	for (int i = 0; i < levels; ++i) {
		for (int j = 0; j < levels; ++j) {
			for (int k = 0; k < levels; ++k) {
				float dist = glm::distance(mid, glm::vec3(i,j,k));
				if (dist < radius) {
					glm::vec3 n =  glm::vec3(i,j,k) - mid;
					if (glm::length(n) > 0) n = glm::normalize(n);
					image_data[i + j * levels + k * levels * levels] = {128 + 128*n.x, 128 + 128*n.y, 128 + 128*n.y, 255};
				}
			}
		}
	}
}

Octree::Octree(unsigned int l): ob { GL_SHADER_STORAGE_BUFFER, true } {
	free_ind = 0;
	levels = l;
	glGenTextures(1, &addr);
	glBindTexture(GL_TEXTURE_3D, addr);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	image_data = new uc_rgba [levels*levels*levels]();
	srand( time(NULL) );

	for (int s = 0; s < 6; ++s) {
		glm::vec3 mid = glm::vec3(levels/4.0f + rand()%levels/2.0f,
				levels/4.0f + rand()%levels/2.0f,
				levels/4.0f + rand()%levels/2.0f);

		float radius = rand() % levels/5.0f;
		makeSphere(image_data, levels, mid, radius);
	}


	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, levels, levels, levels, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, image_data);

	//glGenerateMipmap(GL_TEXTURE_3D);

	glm::ivec3 half = glm::ivec3(levels/2.0f, levels/2.0f, levels/2.0f);
	ob.data->total_size = levels;
	ob.data->brick_size = 8;
	int root = genNode(-half, half, -1);
	ob.update();

	cout << free_ind << " octree nodes used " << endl;

}

Octree::~Octree() {
	// TODO Auto-generated destructor stub
}

int Octree::genNode(glm::ivec3 start, glm::ivec3 end, int parent) {
	Node nd;
	nd.parent = parent;

	// check if node is just empty space
	bool hasContent = false;
	unsigned char init = getNode(start)->a;
	for (int i = start.x; i < end.x; ++i) {
		for (int j = start.y; j < end.y; ++j) {
			for (int k = start.z; k < end.z; ++k) {
				if (getNode(glm::ivec3(i, j, k))->a != init) {
					hasContent = true;
					break;
				}
			}
		}
	}

	int node_index = free_ind;
	free_ind++;

	if (hasContent && start.x + (ob.data->brick_size - 1) < end.x) {

		// create inner nodes
		int h = (end.x - start.x) / 2;
		glm::ivec3 mid = end/2 + start/2;

		// 8 children in octree node
		nd.child[0] = genNode(start, mid, node_index);
		nd.child[1] = genNode(start + glm::ivec3(0, 0, h), mid + glm::ivec3(0, 0, h), node_index);
		nd.child[2] = genNode(start + glm::ivec3(0, h, 0), mid + glm::ivec3(0, h, 0), node_index);
		nd.child[3] = genNode(start + glm::ivec3(0, h, h), mid + glm::ivec3(0, h, h), node_index);

		nd.child[4] = genNode(start + glm::ivec3(h, 0, 0), mid + glm::ivec3(h, 0, 0), node_index);
		nd.child[5] = genNode(start + glm::ivec3(h, 0, h), mid + glm::ivec3(h, 0, h), node_index);
		nd.child[6] = genNode(start + glm::ivec3(h, h, 0), mid + glm::ivec3(h, h, 0), node_index);
		nd.child[7] = genNode(start + glm::ivec3(h, h, h), mid + glm::ivec3(h, h, h), node_index);
	}
	else {

		// leaf node
		nd.child[0] = -1;	// indicate leaf
		nd.child[1] = hasContent? 1 : 0;	// is node solid

		// start and end points in voxel data
		nd.child[2] = start.x;
		nd.child[3] = start.y;
		nd.child[4] = start.z;

		nd.child[5] = end.x;
		nd.child[6] = end.y;
		nd.child[7] = end.z;

		//TODO link neighbors
	}

	ob.data->n[node_index] = nd;
	return node_index;
}

uc_rgba *Octree::getNode(glm::ivec3 p) {
	return &image_data[(p.x+levels/2) + (p.y+levels/2) * levels + (p.z+levels/2) * levels * levels];
}

void Octree::bind(GLuint i) {
	glBindImageTexture(i, addr, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);
	ob.bind(i+1);
}

} /* namespace std */
