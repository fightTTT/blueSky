#include "Ryu.h"
#include "GameCtrl.h"
#include "SceneMng.h"

Ryu::Ryu()
{
	auto ssize = lpSceneMng.GetScreenSize();
	
	characterName = "リュウ";

	Init("image/リュウ/待機/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(ssize.x - 290, ssize.y - 178));
	InitAnim();

	jumpSpeed = { 0, 0 };
	jumpFlag = false;
}

Ryu::~Ryu()
{
}

void Ryu::SetMove(const GameCtrl & ctl)
{
	auto ssize = lpSceneMng.GetScreenSize();

	if (!jumpFlag)
	{
		if (ctl.GetPadData(PAD_1, THUMB_L_RIGHT))
		{
			pos.x += 4;
			SetAnim("後ろ移動");
		}
		else if (ctl.GetPadData(PAD_1, THUMB_L_LEFT))
		{
			pos.x -= 4;
			SetAnim("前移動");
		}
		else
		{
			SetAnim("待機");
		}

		// 斜め左上ジャンプ
		if (ctl.GetPadData(PAD_1, THUMB_L_UP) && ctl.GetPadData(PAD_1, THUMB_L_LEFT))
		{
			jumpSpeed = { -2, -30 };
			jumpFlag = true;
			SetAnim("ジャンプ_前");
		}
		// 斜め右上ジャンプ
		else if (ctl.GetPadData(PAD_1, THUMB_L_UP) && ctl.GetPadData(PAD_1, THUMB_L_RIGHT))
		{
			jumpSpeed = { 2, -30 };
			jumpFlag = true;
			SetAnim("ジャンプ_後ろ");
		}
		// 上ジャンプ
		else if (ctl.GetPadData(PAD_1, THUMB_L_UP))
		{
			jumpSpeed = { 0, -30 };
			jumpFlag = true;
			SetAnim("ジャンプ_上");
		}
	}
	else
	{
		// ジャンプ中
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	if (pos.y > ssize.y - 178)
	{
		pos.y = ssize.y - 178;
		jumpFlag = false;
	}
}
