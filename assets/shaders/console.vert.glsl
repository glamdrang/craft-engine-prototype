#version 330

layout(location = 0)
in vec4 av4_pos;
layout(location = 1)
in vec2 av2_texpos;

out vec2 rv2_texpos;

void main()
{
    gl_Position = av4_pos;
    rv2_texpos = av2_texpos;
}
