uniform mat4 modelViewProjectionMatrix;

attribute vec3 aVertexPos;

void main () {
        gl_Position = modelViewProjectionMatrix * vec4(aVertexPos,1.0);
}
