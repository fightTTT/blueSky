#include "Ryu.h"
#include "GameCtrl.h"
#include "SceneMng.h"

Ryu::Ryu()
{
	auto ssize = lpSceneMng.GetScreenSize();
	
	characterName = "�����E";

	Init("image/�����E/�ҋ@/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(ssize.x - 290, ssize.y - 178));
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
		}
		if (ctl.GetPadData(PAD_1, THUMB_L_LEFT))
		{
			pos.x -= 4;
		}

		// �΂ߍ���W�����v
		if (ctl.GetPadData(PAD_1, THUMB_L_UP) && ctl.GetPadData(PAD_1, THUMB_L_LEFT))
		{
			jumpSpeed = { -2, -30 };
			jumpFlag = true;
		}
		// �΂߉E��W�����v
		else if (ctl.GetPadData(PAD_1, THUMB_L_UP) && ctl.GetPadData(PAD_1, THUMB_L_RIGHT))
		{
			jumpSpeed = { 2, -30 };
			jumpFlag = true;
		}
		// ��W�����v
		else if (ctl.GetPadData(PAD_1, THUMB_L_UP))
		{
			jumpSpeed = { 0, -30 };
			jumpFlag = true;
		}
	}
	else
	{
		// �W�����v��
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	if (pos.y > ssize.y - 178)
	{
		pos.y = ssize.y - 178;
		jumpFlag = false;
	}
}
