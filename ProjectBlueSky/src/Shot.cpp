#include "SceneMng.h"
#include "Shot.h"

Shot::Shot(VECTOR2 pos, VECTOR2 offset, DIR dir, PAD_ID id) : Obj(offset)
{
	ssize = lpSceneMng.GetScreenSize();

	Init("image/ñ_êlä‘/Shot.png", VECTOR2(256, 256), VECTOR2(1, 1), pos, dir == DIR_RIGHT);

	if (dir == DIR_RIGHT)
	{
		speed = 6;
	}
	else
	{
		speed = -6;
	}

	deleteFlag = false;
	padID = id;
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
