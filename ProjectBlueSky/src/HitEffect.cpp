#include "HitEffect.h"


HitEffect::HitEffect(VECTOR2 pos)
{
	Init("image/�G�t�F�N�g/offsetEffect.png", VECTOR2(240, 240), VECTOR2(4, 1), pos, false);
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