#version 330 

// ----- Definitions ----- //

// ----- In ----- //

in vec3 TexCoords3D;

// ----- Out ----- //

out vec4 colour;

// ----- Uniforms ----- //

uniform samplerCube skybox;

// ----- Methods ----- //

void main()
{    
    colour = vec4(texture(skybox, TexCoords3D).xyz, 1.0);
}