#pragma once
#include "Character.h"

enum PAD_ID;

class Ryu :
	public Character
{
public:
	Ryu(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir);
	~Ryu();

	void SetMove(const GameCtrl &ctl);

	void LongAttack();

private:

	PAD_ID padNum;
	bool jumpFlag;
	VECTOR2 jumpSpeed;
};
