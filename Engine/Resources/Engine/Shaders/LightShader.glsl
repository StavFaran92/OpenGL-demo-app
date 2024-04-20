#vert

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 

#frag

#version 330 core
out vec4 FragColor;

// camera
uniform vec3 viewPos;

void main()
{
    vec3 a = viewPos;
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
}