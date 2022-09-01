#pragma once
#include "Shader.h"

class ShaderManager
{
public:
	static HLSLShader playerShader;
	static HLSLShader waterShader;
	static HLSLShader waterFaceObjectShader;
	static HLSLShader normalPlaneShader;
	static HLSLShader mosaicPlaneShader;
	static HLSLShader monoPlaneShader;
	static HLSLShader blurPlaneShader;
	
	static HLSLShader postWater;
	static HLSLShader postMosaic;
	static HLSLShader postBlur;
	static HLSLShader postNormal;
	static void LoadShaders();
};