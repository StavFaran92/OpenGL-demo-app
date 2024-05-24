#vert

#version 330

void main()
{
	gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}

#frag

#version 330

vec3 main()
{      
    return vec3(0.0f, 0.0f, 0.0f);
}