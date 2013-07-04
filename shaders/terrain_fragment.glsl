varying vec4 vColor;
varying vec2 vTexCoord;

uniform sampler2D tex0;

void main() {
        gl_FragColor = vColor * texture2D(tex0, vTexCoord);
}
