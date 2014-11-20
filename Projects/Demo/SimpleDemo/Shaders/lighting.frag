#version 130

// a stub for more advanced shaders in the future

in vec3 fposition;
in vec3 fnormal;
in vec3 ftexcoord;
in mat3 ftanspace;

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
    int HasNormalTex;
    sampler2D DiffuseTexture;
    sampler2D NormalTexture;
};

uniform vec3 cameraPosition;

uniform sampler2D depthMap;

uniform Light light;
uniform Material material;

void main()
{
  vec3 nnormal = normalize(fnormal);


  if(material.HasNormalTex == 1)
  {
    nnormal = 2.0 * texture(material.NormalTexture, vec2(ftexcoord.x, ftexcoord.y)).bgr - vec3(1.0,1.0,1.0);

    nnormal = normalize(ftanspace * nnormal);
  }



  float lightDistance = length(abs(light.Position - fposition));
  vec3 lightDir = normalize(light.Position - fposition);
  vec3 viewDirection = normalize(cameraPosition - fposition);

  vec3 ISpec = light.I0 * material.KSpec * pow(clamp(dot(reflect(-lightDir, nnormal), viewDirection),0.0,1.0), material.Shininess);
  if(material.Shininess == 0.0)
  {
      ISpec = vec3(0.0);
  }

  vec3 KDiff = material.KDiff;
  if(material.HasDiffuseTex == 1)
  {
    KDiff = texture(material.DiffuseTexture, vec2(ftexcoord.x, ftexcoord.y)).bgr;
  }


  vec3 IDiff = light.I0 * KDiff * clamp(dot(nnormal, lightDir),0,1) /
          (lightDistance * lightDistance * light.QuadraticAttenuation + 1.0);
  vec3 IAmbient = material.IAmbient * material.KDiff;
  gl_FragData[0] = vec4(IDiff + ISpec, 1.0);
}
