#version 130

uniform sampler2D uni_Texture;

uniform vec2 uni_LightPositions[20];
uniform vec3 uni_LightColors[20];
uniform int uni_LightCount;

in vec4 ex_Color;
in vec2 ex_TexCoords;
in vec4 ex_VertexPosition;

out vec4 out_Color;
	
// 1.0f, 0.9f, 0.8f, 1.0f

vec3 apply_light(vec2 position, vec3 color) {
	float distance = length(position - ex_VertexPosition.xy);
	float intensity = 1.0f - distance * 0.0039f;
	return color * max(intensity, 0.0f);
}

void main() {

	vec3 lights = vec3(0.0f, 0.0f, 0.0f);
	
	vec4 base = ex_Color * texture(uni_Texture, ex_TexCoords);
	//vec4 base = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	vec3 used_light = vec3(0.0f);
	
	for (int i = 0; i < uni_LightCount; i++) {
		vec3 light = apply_light(uni_LightPositions[i], uni_LightColors[i]);
		used_light.r = max(used_light.r, light.r);
		used_light.g = max(used_light.g, light.g);
		used_light.b = max(used_light.b, light.b);
	}
	
	lights = used_light;
	
	out_Color = min(vec4(base.rgb * lights, base.a), base);
}
