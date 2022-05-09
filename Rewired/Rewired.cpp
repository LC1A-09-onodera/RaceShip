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
