#version 460 core

layout (local_size_x=8, local_size_y=8, local_size_z=1) in;

uniform uint imageWidth;
uniform uint imageHeight;
uniform uint noiseFrequency;
layout (r32f, binding=0) uniform image2D noise;

float rand(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}

vec2 getRandomUnitLengthVector(vec2 v)
{
    float angle = rand(v) * 2.0 * 3.14159265359;
    vec2 ret = vec2(cos(angle), sin(angle));
    float length = sqrt(ret.x * ret.x + ret.y * ret.y);
    return vec2(ret.x / length, ret.y / length);
}

vec2 interpolation(vec2 cell)
{
    // linear = cell
    // cubic = cell * cell * (3.0 - 2.0 * cell)
    // quintic = cell * cell * cell * (cell * (cell * 6.0 - 15.0) + 10.0)
    // cosine = (1.0 - cos(3.14159265359 * cell)) / 2.0
    // cubic polynomial = cell * cell * (2.0 - cell)
    // exponential-ish curve = cell * cell * cell * cell (cell to the N power)
    
    vec2 cubic      = cell * cell * (3.0 - 2.0 * cell);
    vec2 quintic    = cell * cell * cell * (cell * (cell * 6.0 - 15.0) + 10.0);
    vec2 cosine     = (1.0 - cos(3.14159265359 * cell)) / 2.0;
    vec2 polynomial = cell * cell * (2.0 - cell);

    vec2 result =
            cubic       * 0.15
          + quintic     * 0.45
          + cosine      * 0.25
          + polynomial  * 0.15;

    return result;
}

void main()
{
    uvec2 ucoord = gl_GlobalInvocationID.xy;
    vec2 ncoord = vec2(ucoord) / noiseFrequency;
    vec2 cell = fract(ncoord);

    // corner positions
    vec2 topl = floor(ncoord);
    vec2 topr = floor(topl + vec2(1.0, 0.0));
    vec2 botl = floor(topl + vec2(0.0, 1.0));
    vec2 botr = floor(topl + vec2(1.0, 1.0));

    // gradient vectors
    vec2 gA = getRandomUnitLengthVector(topl);   // Top Left
    vec2 gB = getRandomUnitLengthVector(topr);   // Top Right
    vec2 gC = getRandomUnitLengthVector(botl);   // Bottom Left
    vec2 gD = getRandomUnitLengthVector(botr);   // Bottom Right

    // offsets
    vec2 oA = ncoord - topl;
    vec2 oB = ncoord - topr;
    vec2 oC = ncoord - botl;
    vec2 oD = ncoord - botr;

    // dots
    float dA = dot(gA, oA);
    float dB = dot(gB, oB);
    float dC = dot(gC, oC);
    float dD = dot(gD, oD);

    // interpolation
    vec2 u = interpolation(cell);

    float top = mix(dA, dB, u.x);
    float bot = mix(dC, dD, u.x);
    float value = mix(top, bot, u.y);

    // value storing
    imageStore(noise, ivec2(ucoord), vec4(value, 0.0, 0.0, 0.0));
}