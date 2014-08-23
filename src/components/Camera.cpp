/*
 * Camera.cpp
 *
 *  Created on: 25/08/2013
 *      Author: remnanjona
 */

#include <iostream>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../pipeline/Pipeline.h"
#include "Camera.h"

namespace std {

Camera::Camera():
		focus {0, 0, 0},
		cam_angle {1, 0, 0, 0},
		cam_angle_d {1, 0, 0, 0},
		click_old {1, 0, 0, 0},
		click_new {1, 0, 0, 0},
		camera_properties { GL_SHADER_STORAGE_BUFFER, true }
{
	cam_aspect = 1.0;
	viewzoom = 200.0;

	// mouse action settings
	arcball_x = arcball_y = 0.0;
	arcball_radius = 1.0;
	click_x = click_y = 0;
	windowwidth = windowheight = 1;
}

Camera::~Camera() {}

void Camera::update( float tick ) {
	cam_angle = cam_angle_d * cam_angle;
	cam_angle_d = glm::slerp( glm::quat(), cam_angle_d, ( 1 - tick * 10 ) );

	camera_properties.data->P = glm::perspective(45.0f, cam_aspect, 1.0f, 1000.0f);
	camera_properties.data->V = glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -viewzoom) ) * glm::mat4_cast(cam_angle);
	camera_properties.data->I = glm::inverse(camera_properties.data->P * camera_properties.data->V);
	camera_properties.update();
}

void Camera::resize(int x, int y) {
	windowwidth = x;
	windowheight = y;
	cam_aspect = (double) x / (double) y;
	arcball_x = (x / 2.0);
	arcball_y = (y / 2.0);
	arcball_radius = (x / 2.0);
}

void Camera::keyPressed(unsigned char c) {
	if (c == '/') {
		cout << "camera quat = " << cam_angle.w << ", " << cam_angle.x << ", " <<  cam_angle.y << ", " <<  cam_angle.z << endl;
		cout << "camera zoom = " << viewzoom << endl;
	}
}

int Camera::mouseClicked(int button, int state, int x, int y) {
	if (state) {
		control[0] = control[1] = control[2] = false;
	}

	if (button == 0) {
		control[0] = true;
		getArc(arcball_x, arcball_y, x, y, arcball_radius, click_new); // initial click down
		click_old = click_new;
	} else if (button == 2) {
		// panning
		control[1] = true;
		click_x = x;
		click_y = y;
	} else if (button == 3) {
		viewzoom /= 1.05;	// scroll back
	} else if (button == 4) {
		viewzoom *= 1.05;	// scroll forward
	}
	return true;

}

int Camera::mouseDragged(int x, int y) {
	if (control[0]) {
		getArc(arcball_x, arcball_y, x, y, arcball_radius, click_new);
		glm::quat q = cam_angle_d = click_new * glm::inverse(click_old);
		cam_angle_d = q * cam_angle_d;
		click_old = click_new;
		return true;
	}
	else if (control[1]) {
		float xn = click_x - x;
		float yn = click_y - y;
		float len_sq = xn*xn + yn*yn;
		if (len_sq > 0.1) {
			float len = sqrt(len_sq);
			glm::vec3 add = glm::axis( glm::inverse(cam_angle) * glm::quat(0, xn, yn, 0) * cam_angle );
			focus = focus + add * (len / arcball_radius);
			click_x = x;
			click_y = y;
		}
		return true;
	}
}

glm::quat Camera::cameraAngle() {
	return cam_angle;
}

glm::vec3 Camera::project(const glm::vec3 &v) {
	return glm::project( v, camera_properties.data->V, camera_properties.data->P, glm::vec4(0, 0, windowwidth, windowheight) );
}

glm::vec3 Camera::unProject(int x, int y) {
	GLdouble point[3];
	return glm::vec3(point[0], point[1], point[2]);
}

glm::mat4 Camera::viewMatrix() {
	return camera_properties.data->V;
}

glm::mat4 Camera::projectionMatrix() {
	return camera_properties.data->P;
}

Buffer<CameraProperties> *Camera::properties() {
	return &camera_properties;
}

void getArc(int arcx, int arcy, int ix, int iy, float rad, glm::quat &result) {
	float x = (ix - arcx) / rad;
	float y = (iy - arcy) / rad;

	// check click is inside the arcball radius
	if (x*x + y*y < 1.0) {
		float z = sqrt(1 - (x*x + y*y));
		result = glm::quat(0, x, y, z);
	}
	else {
		float len = sqrt(x*x + y*y);
		result = glm::quat(0, x / len, y / len, 0);
	}
}

void getUnitCircle(int arcx, int arcy, int ix, int iy, glm::quat &result) {
	float x = ix - arcx;
	float y = iy - arcy;
	float len = sqrt(x*x + y*y);
	result = glm::quat(0, x / len, y / len, 0);
}

} /* namespace std */
