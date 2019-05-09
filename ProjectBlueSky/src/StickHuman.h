#pragma once
#include "Character.h"

class StickHuman :
	public Character
{
public:
	StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir);
	~StickHuman();
};

