#include "Shot.h"


Shot::Shot(VECTOR2 pos, DIR dir)
{
	Init("image/Shot2.png", VECTOR2(120, 120), VECTOR2(1, 1), pos, dir == DIR_RIGHT);

	if (dir == DIR_RIGHT)
	{
		speed = 4;
	}
	else
	{
		speed = -4;
	}

	deleteFlag = false;
}

Shot::~Shot()
{
}

void Shot::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	pos.x += speed;
}

bool Shot::CheckDeth()
{
	return deleteFlag;
}

bool Shot::CheckObjType(OBJ_TYPE type)
{
	return type == OBJ_TYPE_SHOT;
}
