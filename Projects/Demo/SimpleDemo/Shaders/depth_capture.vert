#version 130

in vec3 position;

uniform mat4 projMat, lightMat, modelMat;


void main()
{
    gl_Position = projMat * lightMat * modelMat * vec4(position, 1.0);
}
