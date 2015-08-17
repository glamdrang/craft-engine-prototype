#version 440

in vec2 o_texs;
in vec3 o_norms;

out vec4 fragColour;

uniform sampler2D texDiffuse;
uniform vec3 colorDiffuse;

void main() {
  fragColour = vec4(colorDiffuse, 1.0) + texture(texDiffuse, o_texs);
}
