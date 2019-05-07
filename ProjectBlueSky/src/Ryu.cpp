#include "Ryu.h"
#include "GameCtrl.h"
#include "SceneMng.h"

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

		if (animTable[GetAnim()][ANIM_TBL_LOOP] || animEndFlag)
		{
			if (ctl.GetPadData(padNum, THUMB_L_RIGHT))
			{
				pos.x += 4;
				if (dir == DIR_LEFT)
				{
					SetAnim("後ろ移動");
				}
				else
				{
					SetAnim("前移動");
				}
			}
			else if (ctl.GetPadData(padNum, THUMB_L_LEFT))
			{
				pos.x -= 4;
				if (dir == DIR_LEFT)
				{
					SetAnim("前移動");
				}
				else
				{
					SetAnim("後ろ移動");
				}

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

				if (dir == DIR_LEFT)
				{
					SetAnim("ジャンプ_前");
				}
				else
				{
					SetAnim("ジャンプ_後ろ");
				}
			}
			// 斜め右上ジャンプ
			else if (ctl.GetPadData(padNum, THUMB_L_UP) && ctl.GetPadData(padNum, THUMB_L_RIGHT))
			{
				jumpSpeed = { 2, -30 };
				jumpFlag = true;

				if (dir == DIR_LEFT)
				{
					SetAnim("ジャンプ_後ろ");
				}
				else
				{
					SetAnim("ジャンプ_前");
				}
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
				SetAnim("波動拳");
			}
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
