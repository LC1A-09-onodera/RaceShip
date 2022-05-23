#pragma once
#include "../BaseDirectX/Input.h"

class InputKey
{
	
};

class Rewired
{
	list<KeyCode> keys;
public:
	void Init(int count, ...);
	bool GetKey();
	bool GetKeyDown();
	bool GetKeyUp();
	void AddKey(KeyCode key);
};