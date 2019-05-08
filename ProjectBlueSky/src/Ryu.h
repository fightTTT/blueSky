#pragma once
#include "Character.h"

class Ryu :
	public Character
{
public:
	Ryu(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir);
	~Ryu();
};
