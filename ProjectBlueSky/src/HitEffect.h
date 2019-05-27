#pragma once

#include "Obj.h"
#include "VECTOR2.h"

class HitEffect :
	public Obj
{
public:
	HitEffect();
	~HitEffect();

	bool CheckDeth();
	bool CheckObjType(OBJ_TYPE type);
	void Draw();

private:

	VECTOR2 pos;

};

