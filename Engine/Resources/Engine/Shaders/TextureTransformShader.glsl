#vert

#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 fragPos;

void main()
{
    vec3 pos = aPos;
    pos.y = -pos.y;
    fragPos = pos.xy;
    gl_Position = vec4(pos, 1.0);
}  

#frag

#version 330 core

in vec2 fragPos;

out vec4 color;

uniform sampler2D source;

void main()
{
    color = vec4(texture(source, fragPos).rgb  , 1.0);
}  