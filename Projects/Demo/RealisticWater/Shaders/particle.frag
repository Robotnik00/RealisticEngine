#version 130

in vec3 fposition;
in vec3 fnormal;

void main()
{
    float val = dot(normalize(fposition), vec3(0,1,0));
    gl_FragColor = vec4(0.0,val * 0.7,val * 1,.05);
}
