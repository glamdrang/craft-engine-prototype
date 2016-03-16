#version 330

uniform sampler2D tex;

in vec2 rv2_texpos;

out vec4 fragColor;

void main()
{
    vec4 color = texture(tex, rv2_texpos);

    fragColor = color;
}
