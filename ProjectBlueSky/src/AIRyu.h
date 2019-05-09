#pragma once

#include "AICharacter.h"

class AIRyu :
	public AICharacter
{
public:
	AIRyu(VECTOR2 pos, VECTOR2 offset, DIR charaDir);
	~AIRyu();
};

