#pragma once
#include "Shader.h"

class ShaderManager
{
public:
	static HLSLShader playerShader;

	static void LoadShaders();
};