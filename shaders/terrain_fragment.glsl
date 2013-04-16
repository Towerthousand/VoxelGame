uniform sampler2D tex0;

void main() {
	gl_FragColor = gl_Color * texture2D(tex0, gl_TexCoord[0].xy);
}
