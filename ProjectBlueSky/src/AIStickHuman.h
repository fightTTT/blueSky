#pragma once

#include "AICharacter.h"

class AIStickHuman : 
	public AICharacter
{
public:
	AIStickHuman(VECTOR2 pos, VECTOR2 offset, DIR charaDir);
	~AIStickHuman();
};

