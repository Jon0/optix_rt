#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "buffer/Buffer.h"
#include "buffer/Octree.h"
#include "components/Camera.h"
#include "pipeline/Pipeline.h"
#include "render/Renderer.h"
#include "shader/Shader.h"

using namespace std;

void error_callback(int error, const char* description) {
	cerr << description << endl;
}

int main(int argc, char *argv[]) {
	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(1536, 864, "Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Initialize GLEW
    if (glewInit() != GLEW_OK) {
    	cerr << "Failed to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }

	/* Init vao */
    GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    /*
     * camera viewpoint
     */
    Camera camera;
    camera.resize(1080, 768);
    camera.update(0.0f);
    camera.properties()->bind(1);

    Renderer r;

    /*
     * gl error check
     */
    if (int error = glGetError()) cout << "initialise error " << error << endl;

    /*
     * start loop
     */
    bool mDown = false;
    double mousex, mousey;
	while (!glfwWindowShouldClose(window))
	{
	    int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			glfwGetCursorPos(window, &mousex, &mousey);

			if (!mDown) {
				camera.mouseClicked(0, 0, mousex, height-mousey);
			}
			else {
				camera.mouseDragged(mousex, height-mousey);
			}
			mDown = true;
		}
		else if (mDown) {
			camera.mouseClicked(0, 1, mousex, height-mousey);
			mDown = false;
		}
	    camera.update(0.01f);


		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		r.draw();

		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();

	    if (int error = glGetError()) cout << "error " << error << endl;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
