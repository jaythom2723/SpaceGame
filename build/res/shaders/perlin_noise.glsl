#version 460 core

layout (local_size_x=8, local_size_y=8, local_size_z=1) in;

uniform uint imageWidth;
uniform uint imageHeight;
layout (r32f, binding=0) uniform image2D noise;
// layout (std460, binding=1) buffer gradients
// {
//     vec2 data[][];
// };

float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}

vec2 getRandomUnitLengthVector(vec2 ucoord)
{
    float angle = rand(ucoord) * 2.0 * 3.14159;
    vec2 v = vec2(cos(angle), sin(angle));
    float length = sqrt(pow(v.x, 2) + pow(v.y, 2));
    return vec2(v.x / length, v.y / length);
}

void main()
{
    uvec2 ucoord = gl_GlobalInvocationID.xy;
    vec2 noiseCoord = ucoord / 1000.0; // TODO: don't hardcode this value
    vec2 cellPos = fract(noiseCoord);
    uint index = ucoord.y * imageWidth + ucoord.x;

    vec2 topLeft = vec2(floor(noiseCoord));
    vec2 topRight = topLeft + vec2(1, 0);
    vec2 botLeft = topLeft + vec2(0, 1);
    vec2 botRight = topLeft + vec2(1, 1);

    vec2 gradTopLeft = getRandomUnitLengthVector(topLeft);
    vec2 gradTopRight = getRandomUnitLengthVector(topRight);
    vec2 gradBotLeft = getRandomUnitLengthVector(botLeft);
    vec2 gradBotRight = getRandomUnitLengthVector(botRight);

    vec2 offTopLeft = gradTopLeft - topLeft;
    vec2 offTopRight = gradTopRight - topRight;
    vec2 offBotLeft = gradBotLeft - botLeft;
    vec2 offBotRight = gradBotRight - botRight;

    float dotTopLeft = dot(gradTopLeft, offTopLeft);
    float dotTopRight = dot(gradTopRight, offTopRight);
    float dotBotLeft = dot(gradBotLeft, offBotLeft);
    float dotBotRight = dot(gradBotRight, offBotRight);

    float sx = smoothstep(0.0, 1.0, cellPos.x);
    float sy = smoothstep(0.0, 1.0, cellPos.y);

    float top = mix(dotTopLeft, dotTopRight, sx);
    float bot = mix(dotBotLeft, dotBotRight, sx);
    float value = mix(top, bot, sy);

    imageStore(noise, ivec2(ucoord), vec4(value, 0.0, 0.0, 1.0));
}