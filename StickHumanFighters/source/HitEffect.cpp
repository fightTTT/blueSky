#include "HitEffect.h"

#include "DxLib.h"

HitEffect::HitEffect(VECTOR2 pos, VECTOR2 drawOffset, bool colorChange) : Obj(drawOffset)
{
	Init("image/エフェクト/offsetEffect.png", VECTOR2(50, 50), VECTOR2(4, 1), pos, false);

	AddAnim("HIT", 0, 0, (divCut.x * divCut.y), 5, false, 0, 0);
	SetAnim("HIT");

	if (colorChange)
	{
		brightColor = 0x00ff00;
	}
	else
	{
		brightColor = 0xffffff;
	}
}

HitEffect::~HitEffect()
{
}

bool HitEffect::CheckDeth()
{
	return animEndFlag;
}

bool HitEffect::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_EFFECT);
}

void HitEffect::Draw()
{
	SetDrawBright((brightColor & 0xff0000) >> 4 * 4, (brightColor & 0x00ff00) >> 2 * 4, brightColor & 0x0000ff);
	Obj::Draw();
	SetDrawBright(255, 255, 255);
}