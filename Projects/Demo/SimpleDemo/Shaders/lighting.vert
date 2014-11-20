#version 130

in vec3 position;
in vec3 normal;
in vec3 texcoord;
in vec3 tangent;

uniform mat4 projMat, viewMat, modelMat;
uniform mat3 normalMat;

uniform vec3 cameraPosition;


out vec3 fposition;
out vec3 fnormal;
out vec3 ftexcoord;
out mat3 ftanspace;

void main()
{
  fposition = (modelMat * vec4(position, 1.0)).xyz;
  fnormal = normalize(normalMat * normal);
  ftexcoord = texcoord;



  vec3 ftangent = (modelMat * vec4(tangent, 1.0)).xyz;

  vec3 fbitangent = cross(fnormal, ftangent);

  ftanspace = mat3(ftangent, fbitangent, fnormal);


  gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0);
}
