#include"DxLib.h"
#include "SceneMng.h"
#include "Shot.h"


#define MOVE_SPPED (8)

Shot::Shot(VECTOR2 pos, VECTOR2 offset, DIR dir, PAD_ID id) : Obj(offset)
{
	ssize = lpSceneMng.GetScreenSize();
	hitBoxSize = VECTOR2(0, 0);

	padID = id;

	interval = 4;

	switch (lpSceneMng.GetCharID(padID))
	{
	case 0:
		Init("image/ショット系/hado.png", VECTOR2(128, 128), VECTOR2(4, 2), pos, dir != DIR_RIGHT);
		hitBoxSize = VECTOR2(100, 100);
		break;
	case 1:
		Init("image/ショット系/sitotsu.png", VECTOR2(128, 128), VECTOR2(5, 2), pos, dir != DIR_RIGHT);
		hitBoxSize = VECTOR2(100, 100);
		break;
	case 2:
		Init("image/ショット系/kikoh.png", VECTOR2(128, 128), VECTOR2(5, 2), pos, dir != DIR_RIGHT);
		hitBoxSize = VECTOR2(100, 100);
		break;
	case 3:
		Init("image/ショット系/kamehameha.png", VECTOR2(128, 128), VECTOR2(4, 1), pos, dir != DIR_RIGHT);
		hitBoxSize = VECTOR2(100, 100);
		break;
	case 4:
		Init("image/ショット系/sonic.png", VECTOR2(128, 128), VECTOR2(4, 2), pos, dir != DIR_RIGHT);
		hitBoxSize = VECTOR2(100, 100);
		interval = 2;
		break;
	case 5:
		Init("image/ショット系/rock.png", VECTOR2(128, 128), VECTOR2(5, 4), pos, dir != DIR_RIGHT);
		hitBoxSize = VECTOR2(100, 100);
		break;
	case 6:
		Init("image/ショット系/psycocutter.png", VECTOR2(128, 128), VECTOR2(6, 1), pos, dir != DIR_RIGHT);
		hitBoxSize = VECTOR2(100, 100);
		break;
	case 7:
		Init("image/ショット系/ibuki.png", VECTOR2(128, 128), VECTOR2(5, 2), pos, dir != DIR_RIGHT);
		hitBoxSize = VECTOR2(100, 100);
		interval = 3;
		break;
	default:
		break;
	}

	AddAnim("移動", 0, 0, (divCut.x * divCut.y), interval, true, 0, 0);
	SetAnim("移動");

	if (dir == DIR_RIGHT)
	{
		speed = MOVE_SPPED;
	}
	else
	{
		speed = -MOVE_SPPED;
	}

	deleteFlag = false;
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

	// ｼｮｯﾄの当たり判定の表示
	if (!hitData.hitFlag)
	{
		VECTOR2 centerPos = (pos + (divSize / 2));
		VECTOR2 halfHitBoxSize = (hitBoxSize / 2);

		DrawBox(drawOffset.x + centerPos.x - halfHitBoxSize.x,
				drawOffset.y + centerPos.y - halfHitBoxSize.y,
				drawOffset.x + centerPos.x + halfHitBoxSize.x,
				drawOffset.y + centerPos.y + halfHitBoxSize.y,
				0x0000ff,
				false);
	}
}

void Shot::CheckHitFlag(void)
{
	if (hitData.hitFlag)
	{
		deleteFlag = true;
	}
}
