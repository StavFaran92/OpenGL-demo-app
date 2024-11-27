#vert

#version 330

layout (location = 0) in float aVertexID;

uniform vec3 uPoint1;
uniform vec3 uPoint2;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec3 position = (aVertexID < 0.5) ? uPoint1 : uPoint2;

	gl_Position = projection * view * model * vec4(position, 1.0);
}

#frag

#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}