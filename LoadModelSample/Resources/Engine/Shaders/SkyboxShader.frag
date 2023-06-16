uniform samplerCube skybox;
in vec3 TexCoords3D;

vec3 main()
{    
    vec3 color = texture(skybox, TexCoords3D).xyz;
	
	return color;
}