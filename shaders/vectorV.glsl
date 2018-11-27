#version 400
in vec3 vp;
void main(){
    gl_Position = vec4(vp.x *  -1 , vp.y * -1, vp.z, 1.0);
}
