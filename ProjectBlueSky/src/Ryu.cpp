#include "Ryu.h"
#include "GameCtrl.h"
#include "SceneMng.h"

Ryu::Ryu()
{
	auto ssize = lpSceneMng.GetScreenSize();
	
	characterName = "ƒŠƒ…ƒE";

	Init("image/ƒŠƒ…ƒE/‘Ò‹@/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(ssize.x - 290, ssize.y - 178));
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

		if (ctl.GetPadData(PAD_1, THUMB_L_UP) && ctl.GetPadData(PAD_1, THUMB_L_LEFT))
		{
			jumpSpeed.y = -30;
			jumpSpeed.x = -2;
			jumpFlag = true;
		}
		else if (ctl.GetPadData(PAD_1, THUMB_L_UP) && ctl.GetPadData(PAD_1, THUMB_L_RIGHT))
		{
			jumpSpeed.y = -30;
			jumpSpeed.x = 2;
			jumpFlag = true;
		}
		else if (ctl.GetPadData(PAD_1, THUMB_L_UP))
		{
			jumpSpeed.y = -30;
			jumpSpeed.x = 0;
			jumpFlag = true;
		}
	}
	else
	{
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	if (pos.y > ssize.y - 178)
	{
		pos.y = ssize.y - 178;
		jumpFlag = false;
	}
}
