#version 130

in vec3 position;
in vec3 positionOffset;

out vec3 fposition;

uniform mat4 projMat, viewMat;

void main()
{

    fposition = position;
    gl_Position = projMat * viewMat * vec4(fposition+ positionOffset, 1.0);

}
