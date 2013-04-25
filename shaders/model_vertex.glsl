uniform mat4 modelViewProjectionMatrix;
uniform float shadow;

void main () {
	gl_Position = modelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color * vec4(1-shadow,1-shadow,1-shadow,1);
}
