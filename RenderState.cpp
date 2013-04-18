#include "RenderState.hpp"

RenderState::RenderState() {
	model = mat4f::fromIdentity();
	view = mat4f::fromIdentity();
	projection = mat4f::fromIdentity();
}

RenderState::~RenderState() {
}

void RenderState::initShaderUniforms(const ShaderProgram& shader) {
	//Set up texture handles and texture matrices
	shader.use();
	for (int i = 0; i < MAX_TEXTURES; ++i) {
		std::string uniformID1 = "tex" + toString(i);
		if(shader.getUniLoc(uniformID1) != -1)
			shader.sendUniform1i(uniformID1,i);
		std::string uniformID2 = "texMat" + toString(i);
		if(shader.getUniLoc(uniformID2) != -1)
			shader.sendUniformMat4f(uniformID2,texture[i][0]);
	}
}

void RenderState::updateShaderUniforms(const ShaderProgram& shader) const { //change model or time depending vars
	shader.use();
//	if(shader.getUniLoc("globalTime") != -1)
//		shader.sendUniform1f("globalTime",GLOBALCLOCK.getElapsedTime().asSeconds());
//	if(shader.getUniLoc("modelMatrix") != -1)
//		shader.sendUniformMat4f("modelMatrix",model[0]);
//	if(shader.getUniLoc("viewMatrix") != -1)
//		shader.sendUniformMat4f("viewMatrix",view[0]);
//	if(shader.getUniLoc("projectionMatrix") != -1)
//		shader.sendUniformMat4f("projectionMatrix",projection[0]);
//	if(shader.getUniLoc("modelViewProjectionMatrix") != -1)
		shader.sendUniformMat4f("modelViewProjectionMatrix",(projection*view*model)[0]);
//	if(shader.getUniLoc("modelViewMatrix") != -1)
//		shader.sendUniformMat4f("modelViewMatrix",(view*model)[0]);
}
