#include "HitEffect.h"


HitEffect::HitEffect(VECTOR2 pos, VECTOR2 drawOffset) : Obj(drawOffset)
{
	Init("image/エフェクト/offsetEffect.png", VECTOR2(50, 50), VECTOR2(4, 1), pos, false);

	AddAnim("HIT", 0, 0, (divCut.x * divCut.y), 5, false, 0, 0);
	SetAnim("HIT");
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
	return false;
}

void HitEffect::Draw()
{
	Obj::Draw();
}