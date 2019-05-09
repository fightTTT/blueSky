#include "Collision.h"


Collision::Collision()
{
}

Collision::~Collision()
{
}

void Collision::SetColData(std::string animName, std::vector<ColInfo> info)
{
	colData[animName] = info;
}

bool Collision::isLoad(std::string animName)
{
	return colData.count(animName) == 1;
}
