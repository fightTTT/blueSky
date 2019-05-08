#pragma once
#include "Character.h"

enum PAD_ID;

class Ryu :
	public Character
{
public:
	Ryu(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir);
	~Ryu();

	void SetMove(const GameCtrl &ctl, weekListObj objList);

private:

	PAD_ID padNum;		// 使用しているコントローラー番号
	bool jumpFlag;
	VECTOR2 jumpSpeed;
};
