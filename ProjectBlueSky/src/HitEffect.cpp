#include "HitEffect.h"


HitEffect::HitEffect()
{
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