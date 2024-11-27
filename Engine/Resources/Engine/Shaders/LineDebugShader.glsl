#vert

#version 330

uniform mat4 m1;
uniform mat4 m2;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    mat4 model = (gl_VertexID == 0) ? m1 : m2;

	gl_Position = projection * view * model * vec4(0,0,0, 1.0);
}

#frag

#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}