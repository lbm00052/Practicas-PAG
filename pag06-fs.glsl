#version 410
in vec3 colorVS;
out vec4 colorFragmento;
void main()
{   colorFragmento = vec4(colorVS, 1.0);
}