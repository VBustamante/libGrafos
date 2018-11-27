#version 400

in vec2 tex_coord;
uniform sampler2D tex;
uniform vec4 color;
void main(){

    float alpha = texture(tex, tex_coord).r;
    gl_FragColor = vec4(1, 1, 1, alpha) * color;
}