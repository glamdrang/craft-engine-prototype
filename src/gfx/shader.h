#pragma once
#include "../common.h"

/*
 * Takes a vertex shader file and fragment shader file and combines them into a program.
 * Call glDeleteProgram to cleanup.
 */
EXPORTED GLuint LoadShader(const char *vertex_path, const char *fragment_path);