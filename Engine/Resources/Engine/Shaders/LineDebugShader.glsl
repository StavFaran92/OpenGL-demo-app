#vert

#version 330

uniform vec3 p1;
uniform vec3 p2;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec3 point = (gl_VertexID == 0) ? p1 : p2;

	gl_Position = projection * view * vec4(point, 1.0);
}

#frag

#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}