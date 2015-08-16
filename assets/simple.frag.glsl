#version 440

in vec2 o_texs;
in vec3 o_norms;

out vec4 fragColour;

uniform sampler2D texDiffuse;

void main() {
  fragColour = vec4(o_norms * 0.1, 1.0) + vec4(texture(texDiffuse, o_texs));
}
