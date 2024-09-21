#vert

#version 410 core
                                                                                    
layout (location = 0) in vec3 aPos;  
layout (location = 2) in vec2 aTexCoords;                                        

out vec2 texCoords;

uniform int width;
uniform int height;
                                                            
void main()                                                                         
{
    texCoords = aTexCoords;
    vec3 pos = aPos.xyz;
    pos *= vec3(width, 0, height);
    gl_Position = vec4(pos, 1.0);                                       
}

#tesc

#version 410 core

// Number of vertices in a patch
layout (vertices=4) out;

uniform mat4 model;
uniform mat4 view;

in vec2 texCoords[];

out vec2 outTexCoords[];

const int MIN_TESS_LEVEL = 4;
const int MAX_TESS_LEVEL = 64;
const float MIN_DISTANCE = 20;
const float MAX_DISTANCE = 200;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    outTexCoords[gl_InvocationID] = texCoords[gl_InvocationID];

    //If we are in Vertex 0 in the patch
    if(gl_InvocationID == 0)
    {
        // transform each vertex into eye space
        vec4 eyeSpacePos00 = view * model * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = view * model * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = view * model * gl_in[2].gl_Position;
        vec4 eyeSpacePos11 = view * model * gl_in[3].gl_Position;

        // "distance" from camera scaled between 0 and 1
        float distance00 = clamp((abs(eyeSpacePos00.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
        float distance01 = clamp((abs(eyeSpacePos01.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
        float distance10 = clamp((abs(eyeSpacePos10.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
        float distance11 = clamp((abs(eyeSpacePos11.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);

        // interpolate edge tessellation level based on closer vertex
        float tessLevel0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
        float tessLevel1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
        float tessLevel2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
        float tessLevel3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );

        // set the corresponding outer edge tessellation levels
        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        // set the inner tessellation levels to the max of the two parallel edges
        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}

#tese

#version 410 core

layout (quads, fractional_odd_spacing, ccw) in;

uniform sampler2D heightMap;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;

in vec2 outTexCoords[];

out vec2 texCoord;

// Send to fragment shader for coloring
out float height;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // retrieve control point texture coordinates
    vec2 t00 = outTexCoords[0];
    vec2 t01 = outTexCoords[1];
    vec2 t10 = outTexCoords[2];
    vec2 t11 = outTexCoords[3];

    // bilinearly interpolate texture coordinate across patch
    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    texCoord = (t1 - t0) * v + t0;

    // lookup texel at patch coordinate for height and scale
    height = texture(heightMap, texCoord).r;

    // get point position
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // compute patch surface normal
    vec4 uVec = p01 - p00;  // X-Axis
    vec4 vVec = p10 - p00;  // Y-Axis
    vec4 normal = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0) );

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // displace point along normal
    p += normal * height * scale;

    gl_Position = projection * view * model * p;
}

#frag

#version 410 core

uniform int textureCount;

uniform sampler2D texture_0;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;

uniform float textureBlend[4];
uniform vec2 textureScale[4];

uniform float scale;

in float height;
in vec2 texCoord;

out vec4 color;

vec4 sampleFromTexture(int textureIndex, vec2 uv)
{
    uv *= textureScale[textureIndex];

    if (textureIndex == 0)  return texture(texture_0, uv);
    else if (textureIndex == 1) return texture(texture_1, uv);
    else if (textureIndex == 2) return texture(texture_2, uv);
    else if (textureIndex == 3) return texture(texture_3, uv);

    return vec4(0.0); // Return black if index is out of bounds
}

void main()
{
    if(height < textureBlend[0])
    {
        color = sampleFromTexture(0, texCoord);
    }
    else if(height >= textureBlend[textureCount - 1])
    {
        color = sampleFromTexture(textureCount - 1, texCoord);
    }
    else
    {
        for(int i=0; i < textureCount - 1; i++)
        {
            if(height >= textureBlend[i] && height < textureBlend[i + 1])
            {
                float b1 = height - textureBlend[i];
                float b2  = textureBlend[i + 1] - height;
                float blend = b1 / (b1 + b2);
                color = mix(sampleFromTexture(i, texCoord), sampleFromTexture(i + 1, texCoord), blend);
                break;
            }
        }
    }
}