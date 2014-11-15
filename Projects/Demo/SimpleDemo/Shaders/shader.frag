#version 150

// a stub for more advanced shaders in the future

in vec3 fposition;
in vec3 fnormal;
in vec3 ftexcoord;
in mat3 ftanspace;
in vec3 ftangent1;


struct Light
{
    vec3 Position;
    vec3 I0;
    float QuadraticAttenuation;
};

struct Material
{
    vec3 KDiff;
    vec3 KSpec;
    vec3 IAmbient;
    float Shininess;
    int HasDiffuseTex;
    int hasNormalTex;
};

uniform vec3 cameraPosition;

uniform vec3 udiffuse;

uniform sampler2D diffTexture;
uniform sampler2D normTexture;

uniform int hasDiffuseTex;
uniform int hasNormalTex;

uniform Light light1;
uniform Material material;

void main()
{
  vec3 nnormal = normalize(fnormal);


  if(hasNormalTex == 1)
  {
    nnormal = 2.0 * texture2D(normTexture, vec2(ftexcoord.x, ftexcoord.y)).bgr - vec3(1.0,1.0,1.0);

    nnormal = normalize(ftanspace * nnormal);
  }

  vec3 camerapos = vec3(0.0,2.5,7.0);
  vec3 lightpos = camerapos;

  vec3 lightDir = normalize(lightpos - fposition);
  vec3 viewDirection = normalize(camerapos - fposition);

  vec3 ISpec = vec3(0.1,0.1,0.1) * pow(clamp(dot(reflect(-lightDir, nnormal), viewDirection),0.0,1.0), 50);


  float val = dot(nnormal, vec3(0.0,0.0,1.0));

  vec3 IDiff = udiffuse;
  if(hasDiffuseTex == 1)
  {
    IDiff =  texture2D(diffTexture, vec2(ftexcoord.x, ftexcoord.y)).bgr;
  }
  if(IDiff.x >= 0.99 && IDiff.y >= 0.99 && IDiff.z >= 0.99)
  {
    discard;
  }

  IDiff *= val;

  gl_FragColor = vec4(IDiff + ISpec, 1.0);
}
