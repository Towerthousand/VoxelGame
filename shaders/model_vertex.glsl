uniform mat4 modelViewProjectionMatrix;
uniform float ambientLight;

attribute vec3 aVertexPos;
attribute vec4 aVertexColor;

varying vec4 vColor;

void main () {
        gl_Position = modelViewProjectionMatrix * vec4(aVertexPos,1.0);
        vColor = aVertexColor * vec4(ambientLight,ambientLight,ambientLight,1.0);
}
