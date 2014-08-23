/*
 * Camera.h
 *
 *  Created on: 25/08/2013
 *      Author: remnanjona
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../buffer/Buffer.h"

namespace std {

struct CameraProperties {
	glm::mat4 P;
	glm::mat4 V;
	glm::mat4 I;
};

void getArc(int, int, int, int, float, glm::quat &);
void getUnitCircle(int, int, int, int, glm::quat &);

class Camera {
private:
	/*
	 * point the camera looks at
	 */
	glm::vec3 focus;

	glm::quat cam_angle, cam_angle_d, click_old, click_new;
	bool control[3];
	int windowwidth, windowheight, click_x, click_y;
	float viewzoom, cam_aspect, arcball_radius, arcball_x, arcball_y;
	GLfloat temp_matrix [16], proj_matrix [16], model_matrix [16];
	GLdouble proj_matrixd [16], model_matrixd [16];
	Buffer<CameraProperties> camera_properties;

public:
	Camera();
	virtual ~Camera();

	void update( float );
	void resize(int, int);

	void keyPressed(unsigned char c);
	int mouseClicked(int, int, int, int);
	int mouseDragged(int, int);

	glm::quat cameraAngle();
	glm::vec3 project(const glm::vec3 &);
	glm::vec3 unProject(int, int);
	glm::mat4 viewMatrix();
	glm::mat4 projectionMatrix();
	Buffer<CameraProperties> *properties();
};

} /* namespace std */
#endif /* CAMERA_H_ */
