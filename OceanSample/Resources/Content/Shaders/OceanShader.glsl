#vert

uniform float amplitude;
uniform vec2 waveDirection;
uniform float waveLength;
uniform float waveSpeed;
uniform float steepness;

void vert(inout vec3 aPos, inout vec3 aNorm)
{
	float w = PI2 / waveLength;
	float pSpeed = 2 * waveSpeed / waveLength;
	float q = steepness / (w * amplitude);
	
	vec2 dir = normalize(waveDirection);

	float a = dot(vec2(pos.x, pos.y),dir) * w + time * pSpeed;

	aPos.x = pos.x + q * amplitude * dir.x * cos(a);
	aPos.y = pos.y + q * amplitude * dir.y * cos(a);
	aPos.z = - amplitude * sin(a);
	
	aNorm.x = - dir.x * w * amplitude * cos(a);
	aNorm.y = - dir.y * w * amplitude * cos(a);
	aNorm.z = 1 - q * w * amplitude * sin(a);
}

#frag

void frag(inout vec3 color)
{
	// color = vec3(0.0f, 0.329f, 0.576f) * color;
	color = getPixelNormal() * color;
}