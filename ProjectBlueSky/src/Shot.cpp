#include"DxLib.h"
#include "SceneMng.h"
#include "Shot.h"


#define MOVE_SPPED (7)

Shot::Shot(VECTOR2 pos, VECTOR2 offset, DIR dir, PAD_ID id) : Obj(offset)
{
	ssize = lpSceneMng.GetScreenSize();

	Init("image/棒人間/Shot.png", VECTOR2(256, 256), VECTOR2(1, 1), pos, dir == DIR_RIGHT);

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
	if ((pos.x < (0 - divSize.x)) || ((pos.x - divSize.x) > ssize.x))
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

		// 波動拳の当たり判定の表示
		if (!hitData.hitFlag)
		{
				DrawBox(drawOffset.x + pos.x, drawOffset.y +  pos.y + (divSize.y/2), drawOffset.x + pos.x + 100, drawOffset.y + pos.y + (divSize.y / 2) + 100,0x0000ff,true);
		}

}

void Shot::CheckHitFlag(void)
{
	if (hitData.hitFlag)
	{
		deleteFlag = true;
	}
}
