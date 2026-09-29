#version 460 core

layout (local_size_x=8, local_size_y=8, local_size_z=1) in;

layout (r32f, binding=0) uniform image2D mask;

uniform int imageWidth;
uniform int imageHeight;

uniform vec2 galaxyCenter;
uniform float galaxyRadius;

uniform float spiralArmCount;
uniform float spiralArmTightness;
uniform float spiralArmWidth;

void main()
{
    ivec2 pcoord = ivec2(gl_GlobalInvocationID.xy);
    vec2 uv = (vec2(pcoord) + 0.5) / vec2(imageWidth, imageHeight);
    vec2 pos = uv * 2.0 - 1.0;
    // non-square stretch fix
    pos.x *= float(imageWidth) / float(imageHeight);

    vec2 offset = pos - galaxyCenter;

    float radius = length(offset);
    float normalizedRadius = radius / galaxyRadius;

    // calculate the angle of the pixel to the galaxy's center
    float angle = atan(offset.y, offset.x);

    // creating the spiral
    float spiralAngle = angle + normalizedRadius * spiralArmTightness;
    // repeat the spiral for the number of arms
    float repeatedAngle = spiralAngle * spiralArmCount;
    // distance to the nearest arm
    float armDistance = abs(sin(repeatedAngle * 0.5));

    // convert distance to influence
    float influence = 1.0 - smoothstep(0.0, spiralArmWidth, armDistance);

    // fade galaxy toward outer edge
    float outerFalloff = 1.0 - smoothstep(0.70, 1.0, normalizedRadius);

    // fade the very center ever so slightly
    float centerFalloff = smoothstep(0.0, 0.15, normalizedRadius);

    // combine the shape
    float galaxyMask = influence * outerFalloff * centerFalloff;

    galaxyMask = clamp(galaxyMask, 0.0, 1.0);

    imageStore(mask, pcoord, vec4(galaxyMask, 0.0, 0.0, 1.0));
}