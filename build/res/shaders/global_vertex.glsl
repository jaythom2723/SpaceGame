#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 tPos;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0, 1.0);
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}