uniform mat4 modelViewProjectionMatrix;
uniform mat4 texMat0;

void main () {
	gl_Position = modelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = texMat0 * gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
