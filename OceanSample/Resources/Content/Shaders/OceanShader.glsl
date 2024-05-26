#vert

uniform float amplitude;
uniform vec2 waveDirection;
uniform float waveLength;
uniform float waveSpeed;
uniform float steepness;

vec3 vert(vec3 aPos)
{
	vec3 vPos = aPos;
	float w = PI2 / waveLength;
	float pSpeed = 2 * waveSpeed / waveLength;
	float q = steepness / (w * amplitude);
	
	vec2 dir = normalize(waveDirection);

	float a = dot(vec2(pos.x, pos.y),dir) * w + time * pSpeed;

	vPos.x = pos.x + q * amplitude * dir.x * cos(a);
	vPos.y = pos.y + q * amplitude * dir.y * cos(a);
	vPos.z = - amplitude * sin(a);
	
	// Normal.x = - dir.x * w * amplitude * cos(a);
	// Normal.y = - dir.y * w * amplitude * cos(a);
	// Normal.z = 1 - q * w * amplitude * sin(a);

    return vPos;
}

#frag

vec3 frag(vec3 color)
{
	return vec3(0.0f, 0.329f, 0.576f) * color;
}