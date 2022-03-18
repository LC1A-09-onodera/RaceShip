#include "Hole.h"

void Hole::Init(XMFLOAT3 position)
{
	each.position = ConvertXMFLOAT3toXMVECTOR(position);
	isActive = true;
	life = MaxLife;
}
