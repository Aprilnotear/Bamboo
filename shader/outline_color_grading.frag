#version 450

layout(location = 0) in vec2 f_tex_coord;
layout(location = 0) out vec4 bright_color;
layout(location = 1) out vec4 o_color;

layout(set = 0, binding = 0) uniform sampler2D color_texture_sampler;
layout(set = 0, binding = 1) uniform sampler2D outline_texture_sampler;
layout(set = 0, binding = 2) uniform sampler3D color_grading_lut_texture_sampler;

layout(push_constant) uniform PCO 
{
	layout(offset = 0) int is_selecting;
	layout(offset = 4) float bloom_threshold;
} pco;

#define OUTLINE_COLOR vec4(0.89, 0.61, 0.003, 1.0)

void main()
{
	o_color = texture(color_texture_sampler, f_tex_coord);

	// lut
	ivec3 dim = textureSize(color_grading_lut_texture_sampler, 0);
	if (dim.x > 1)
	{
		vec3 uvw = vec3(1.0 - o_color.g, o_color.r, o_color.b) + vec3(-0.5, 0.5, 0.5) / vec3(dim.y, dim.x, dim.z);
		o_color = texture(color_grading_lut_texture_sampler, uvw);
	}

	// brightness
	float brightness = dot(o_color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > pco.bloom_threshold)
        bright_color = vec4(o_color.rgb, 1.0);
	else
		bright_color = vec4(0.0, 0.0, 0.0, 1.0);

	// is_selecting
	if (bool(pco.is_selecting) && texture(outline_texture_sampler, f_tex_coord).a > 0.5)
	{
	    o_color = OUTLINE_COLOR;
	}
}