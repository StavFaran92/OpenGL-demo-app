#version 330 core
out vec4 FragColor;

// camera
uniform vec3 viewPos;

void main()
{
    vec3 a = viewPos;
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
}