#pragma once
#include <list>
#include <string>
#include <DirectXMath.h>

template <typename T>
struct ScriptableObjectElement
{
	std::string m_objectName;
	T m_element;
};
class BaseScriptableObject
{
protected:
	std::list<ScriptableObjectElement<int>> m_intElement;
	std::list<ScriptableObjectElement<float>> m_floatElement;
	std::list<ScriptableObjectElement<DirectX::XMFLOAT3>> m_float3Element;
	std::list<ScriptableObjectElement<std::string>> m_stringElement;
public:
	virtual void LoadFile(const char *path) = 0;
	int GetInt(std::string name);
	float GetFloat(std::string name);
	DirectX::XMFLOAT3 GetXMFLOAT3(std::string name);
	std::string GetString(std::string name);
};