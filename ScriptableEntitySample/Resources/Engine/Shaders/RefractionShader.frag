#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 viewPos;
uniform samplerCube skybox;
uniform float refractiveRatio;

void main()
{             
    vec3 I = normalize(Position - viewPos);
    vec3 R = refract(I, normalize(Normal), refractiveRatio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
} 