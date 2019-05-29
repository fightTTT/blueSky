#pragma once

#include "Obj.h"
#include "VECTOR2.h"

class HitEffect :
	public Obj
{
public:
	HitEffect(VECTOR2 pos, VECTOR2 drawOffset, bool colorChange);
	~HitEffect();

	bool CheckDeth();
	bool CheckObjType(OBJ_TYPE type);
	void Draw();

	unsigned int brightColor;
};

