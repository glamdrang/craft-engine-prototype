#version 440

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

out vec2 o_texs;
out vec3 o_norms;

void main() {
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);

  o_texs = tex;
  o_norms = norm;
}