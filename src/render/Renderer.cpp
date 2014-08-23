/*
 * Renderer.cpp
 *
 *  Created on: 24/03/2014
 *      Author: remnanjona
 */


#include "../pipeline/Pipeline.h"
#include "../shader/Shader.h"
#include "Renderer.h"

namespace std {

Renderer::Renderer(): buff(GL_ARRAY_BUFFER, false), tree(256) {

	vector<texvec> data = {
			{glm::vec4(-1,-1,0,1), glm::vec2(0,0)}, {glm::vec4(1,-1,0,1), glm::vec2(t.width,0)},
			{glm::vec4(-1,1,0,1), glm::vec2(0,t.height)}, {glm::vec4(1,1,0,1), glm::vec2(t.width,t.height)}
	};

	buff.insert(data);

    // Init Pipeline
    Shader test_vert("glsl/test.vert", GL_VERTEX_SHADER);
    Shader test_frag("glsl/test.frag", GL_FRAGMENT_SHADER);
    tex_pipeline.addStage(test_vert, GL_VERTEX_SHADER_BIT);
    tex_pipeline.addStage(test_frag, GL_FRAGMENT_SHADER_BIT);


    Shader raytracer("glsl/raytrace.comp", GL_COMPUTE_SHADER);
    comp_pipeline.addStage(raytracer, GL_COMPUTE_SHADER_BIT);

}

Renderer::~Renderer() {
	// TODO Auto-generated destructor stub
}

void Renderer::draw() {
	glBindProgramPipeline(comp_pipeline.name);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_RECTANGLE_NV, t.texture);
	glBindImageTexture(0, t.texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	tree.bind(1);

	glDispatchCompute(t.width/16, t.height/16, 1);
	glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);

	glBindProgramPipeline(tex_pipeline.name);

	glBindBuffer( GL_ARRAY_BUFFER, buff.location );
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(texvec), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(texvec), (GLvoid*)sizeof(glm::vec4));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindTexture(GL_TEXTURE_RECTANGLE_NV, t.texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

} /* namespace std */
