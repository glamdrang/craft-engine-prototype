#version 440

in vec2 o_texs;
in vec3 o_norms;

out vec4 fragColour;

uniform sampler2D texDiffuse;

void main() {
  fragColour = texture(texDiffuse, o_texs);
}
