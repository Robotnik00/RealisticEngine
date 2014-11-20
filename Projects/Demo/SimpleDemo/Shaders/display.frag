#version 130

in vec3 fposition;

uniform sampler2D texture;

void main()
{
    vec2 texcoords = 0.5 * fposition.xy + vec2(0.5,0.5);

    vec3 rgb = texture2D(texture, texcoords).rgb;
    gl_FragColor = vec4(rgb, 1.0);
}
