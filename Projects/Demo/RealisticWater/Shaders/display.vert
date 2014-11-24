#version 130

in vec3 position;

out vec3 fposition;


void main()
{
    fposition = position;
    gl_Position = vec4(position, 1.0);
}
