uniform mat4 modelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 texMat0;
uniform float globalTime;

void main () {
	gl_Position =  ProjectionMatrix * modelViewMatrix * (/*vec4(0,sin(globalTime+gl_Vertex.x),0,1) +*/ gl_Vertex);
	gl_TexCoord[0] = texMat0 * gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
