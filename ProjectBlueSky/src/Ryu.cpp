#include "Ryu.h"
#include "GameCtrl.h"
#include "SceneMng.h"

//Ryu::Ryu()
//{
//	auto ssize = lpSceneMng.GetScreenSize();
//	
//	characterName = "リュウ";
//
//	Init("image/リュウ/待機/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(ssize.x - 290, ssize.y - 178));
//
//	jumpSpeed = { 0, 0 };
//	jumpFlag = false;
//}

Ryu::Ryu(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	auto ssize = lpSceneMng.GetScreenSize();

	characterName = "リュウ";

	Init("image/リュウ/待機/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(pos.x, pos.y));

	jumpSpeed = { 0, 0 };
	jumpFlag = false;
	padNum = id;
	dir = charaDir;
}

Ryu::~Ryu()
{
}

void Ryu::SetMove(const GameCtrl & ctl)
{
	auto ssize = lpSceneMng.GetScreenSize();

	if (!jumpFlag)
	{
		dir = tmpDir;

		if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
		{
			pos.x += 4;
			SetAnim("後ろ移動");
		}
		else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
		{
			pos.x -= 4;
			SetAnim("前移動");
		}
		else
		{
			SetAnim("待機");
		}

		// 斜め左上ジャンプ
		if (ctl.GetPadData(padNum, THUMB_L_UP) && ctl.GetPadData(padNum, THUMB_L_LEFT))
		{
			jumpSpeed = { -2, -30 };
			jumpFlag = true;
			SetAnim("ジャンプ_前");
		}
		// 斜め右上ジャンプ
		else if (ctl.GetPadData(padNum, THUMB_L_UP) && ctl.GetPadData(padNum, THUMB_L_RIGHT))
		{
			jumpSpeed = { 2, -30 };
			jumpFlag = true;
			SetAnim("ジャンプ_後ろ");
		}
		// 上ジャンプ
		else if (ctl.GetPadData(padNum, THUMB_L_UP))
		{
			jumpSpeed = { 0, -30 };
			jumpFlag = true;
			SetAnim("ジャンプ_上");
		}
		else if (ctl.GetPadData(padNum, BUTTON_X))
		{
			longAttackFlag = true;
			longAttackPos = pos;
		}
	}
	else
	{
		// ジャンプ中
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	if (pos.y > ssize.y)
	{
		pos.y = ssize.y;
		jumpFlag = false;
	}

	if (longAttackFlag)
	{
		VECTOR2 speed;
		if (dir == DIR_LEFT)
		{
			speed = { -4, 0 };
		}
		else
		{
			speed = { 4, 0 };
		}
		longAttackPos += speed;
	}
}

void Ryu::LongAttack()
{
}
