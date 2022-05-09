#pragma once
#include "../BaseDirectX/Input.h"

enum class KeyCode
{
	CodeNone,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
	Key1,Key2,Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key0,
	Tab,Space,LShift, RShift,Enter,

	PadA,PadB,PadX,PadY,PadR,PadL
};

class InputKey
{
	
};

class Rewired
{
	list<KeyCode> keys;
public:
	void Init(int count,...);
	bool GetKey();
	bool GetKeyDown();
	bool GetKeyUp();

};