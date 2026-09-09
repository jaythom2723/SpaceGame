#version 460 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D OBTex;

void main()
{
    FragColor = texture(OBTex, TexCoord) * vertexColor;
}