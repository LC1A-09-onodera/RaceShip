#include "Rewired.h"

void Rewired::Init(int count,...)
{
	va_list ap;
	va_start(ap, count);
	for (int i = 0;i < count;i ++)
	{
		keys.push_back(va_arg(ap, KeyCode));
	}
	va_end(ap);
}

bool Rewired::GetKey()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::Key(*keyItr))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::GetKeyDown()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::KeyTrigger(*keyItr))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::GetKeyUp()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::KeyEnd(*keyItr))
		{
			return true;
		}
	}
	return false;
}

void Rewired::AddKey(KeyCode key)
{
	keys.push_back(key);
}
