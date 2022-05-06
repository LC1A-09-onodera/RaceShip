#pragma once
#include "Shader.h"

class ShaderManager
{
public:
	static HLSLShader playerShader;
	static HLSLShader thunderShader;
	static HLSLShader othelloShader;
	
	static HLSLShader postWater;
	static HLSLShader postMosaic;
	static HLSLShader postBlur;
	static HLSLShader postNormal;
	//static HLSLShader translucentShader;
	static void LoadShaders();
};