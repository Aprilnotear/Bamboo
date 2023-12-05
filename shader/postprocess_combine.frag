#version 450
#extension GL_GOOGLE_include_directive : enable

#include "hdr.h"

layout(location = 0) in vec2 f_tex_coord;

layout(input_attachment_index = 0, binding = 0) uniform subpassInput blured_texture_sampler;
layout(input_attachment_index = 0, binding = 1) uniform subpassInput outline_texture_sampler;

layout(push_constant) uniform PCO {
    layout(offset = 0) int bloom_fx_on;
    layout(offset = 4) float exposure;
} pco;

layout(location = 0) out vec4 o_color;

void main()
{
    vec3 hdr_color = subpassLoad(outline_texture_sampler).rgb;
    vec3 bloom_color = subpassLoad(blured_texture_sampler).rgb;

    if(pco.bloom_fx_on == 1)
        hdr_color += bloom_color;
    o_color = vec4(tonemap(hdr_color.xyz, pco.exposure), 1.0);
}