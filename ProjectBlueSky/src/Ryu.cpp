#include "Ryu.h"
#include "GameCtrl.h"

Ryu::Ryu()
{
	characterName = "ƒŠƒ…ƒE";

	Init("image/ƒŠƒ…ƒE/‘Ò‹@/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(300, 300));
	InitAnim();
}

Ryu::~Ryu()
{
}

void Ryu::SetMove(const GameCtrl & ctl)
{
	if (ctl.GetPadData(PAD_1, THUMB_L_RIGHT))
	{
		pos.x += 4;
	}

	if (ctl.GetPadData(PAD_1, THUMB_L_LEFT))
	{
		pos.x -= 4;
	}
}
