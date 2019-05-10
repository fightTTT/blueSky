#include "SceneMng.h"
#include "Shot.h"


Shot::Shot(VECTOR2 pos, DIR dir)
{
	ssize = lpSceneMng.GetScreenSize();

	Init("image/Shot.png", VECTOR2(256, 256), VECTOR2(1, 1), pos, dir == DIR_RIGHT);

	if (dir == DIR_RIGHT)
	{
		speed = 6;
	}
	else
	{
		speed = -6;
	}

	deleteFlag = false;
}

Shot::~Shot()
{
}

void Shot::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	pos.x += speed;
	if ((pos.x < (0 - divSize.x)) || (pos.x > ssize.x))
	{
		deleteFlag = true;
	}
}

bool Shot::CheckDeth()
{
	return deleteFlag;
}

bool Shot::CheckObjType(OBJ_TYPE type)
{
	return type == OBJ_TYPE_SHOT;
}
