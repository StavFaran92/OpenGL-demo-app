#vert

#version 410 core
                                                                                    
layout (location = 0) in vec3 aPos;  
layout (location = 2) in vec2 aTexCoords;                                        

out vec2 texCoords;
                                                                                    
void main()                                                                         
{
    texCoords = aTexCoords;
    gl_Position = vec4(aPos.xyz, 1.0);                                       
}

#tesc

#version 410 core

// Number of vertices in a patch
layout (vertices=4) out;

in vec2 texCoords[];

out vec2 outTexCoords[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    outTexCoords[gl_InvocationID] = texCoords[gl_InvocationID];

    //If we are in Vertex 0 in the patch
    if(gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 16;
        gl_TessLevelOuter[1] = 16;
        gl_TessLevelOuter[2] = 16;
        gl_TessLevelOuter[3] = 16;

        gl_TessLevelInner[0] = 16;
        gl_TessLevelInner[1] = 16;
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
    vec2 texCoord = (t1 - t0) * v + t0;

    // lookup texel at patch coordinate for height and scale
    height = texture(heightMap, texCoord).y * scale;

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
    p += normal * height;

    gl_Position = projection * view * model * p;
}

#frag

#version 410 core

uniform float scale;

in float height;

out vec4 color;

void main()
{
    float h = (height)/scale;
    color = vec4(h, h, h, 1.0);
}