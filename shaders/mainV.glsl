#version 400
in vec3 vp;
void main(){
    gl_Position = vec4(vp.xy, vp.z, 1.0);
}
