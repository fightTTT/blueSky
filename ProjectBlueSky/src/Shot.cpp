#include"DxLib.h"
#include "SceneMng.h"
#include "Shot.h"


#define MOVE_SPPED (7)

Shot::Shot(VECTOR2 pos, VECTOR2 offset, DIR dir, PAD_ID id) : Obj(offset)
{
	ssize = lpSceneMng.GetScreenSize();

	Init("image/ñ_êlä‘/Shot.png", VECTOR2(256, 256), VECTOR2(1, 1), pos, dir == DIR_RIGHT);

	if (dir == DIR_RIGHT)
	{
		speed = MOVE_SPPED;
	}
	else
	{
		speed = -MOVE_SPPED;
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

void Shot::Draw(void)
{

		Obj::Draw();

		// îgìÆåùÇÃìñÇΩÇËîªíËÇÃï\é¶
		if (!hitData.hitFlag)
		{
			VECTOR2 startPos = { 0,0 };
			VECTOR2 endPos = { 0,0 };
				startPos = { pos.x - 50,pos.y - 50 };
				endPos = { pos.x + 50,pos.y + 50 };
				DrawBox(startPos.x, startPos.y, endPos.x, endPos.y,0xff0000,true);
		}

}

void Shot::CheckHitFlag(void)
{
	if (hitData.hitFlag)
	{
		deleteFlag = true;
	}
}
