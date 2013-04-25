uniform mat4 modelViewProjectionMatrix;
uniform float ambientLight;

void main () {
        gl_Position = modelViewProjectionMatrix * gl_Vertex;
        gl_FrontColor = gl_Color * vec4(ambientLight,ambientLight,ambientLight,1);
}
