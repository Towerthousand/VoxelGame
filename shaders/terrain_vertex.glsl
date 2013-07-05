attribute vec3 aVertexPos;
attribute vec2 aTexCoord;
attribute vec4 aVertexColor;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 texMat0;

varying vec4 vColor;
varying vec2 vTexCoord;

void main () {
        gl_Position = modelViewProjectionMatrix * vec4(aVertexPos,1.0);
        vTexCoord = vec4(texMat0 * vec4(aTexCoord,0.0,0.0)).xy;
        vColor = aVertexColor/255.0;
}
