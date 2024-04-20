#vert

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  

#frag

#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

// ----- In ----- //

in vec2 TexCoords;

// ----- Out ----- //

out float FragColor;

// ----- Uniforms ----- //

uniform sampler2D gSSAOColorBuffer;

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
// ----- Variables ----- //

void main() 
{ 
    // retrieve data from G-buffer
    vec2 texelSize = 1.0 / vec2(textureSize(gSSAOColorBuffer, 0));
    float result = 0.f;
    for(int i=-2; i<=2; i++)
    {
        for(int j=-2; j<=2; j++)
        {
            vec2 offset = vec2(float(i), float(j)) * texelSize;
            result += texture(gSSAOColorBuffer, TexCoords + offset).r;
        }
    }

    result /= (5.0 * 5.0);
    FragColor = result;
} 