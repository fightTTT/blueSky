#pragma once

#include "Obj.h"
#include "VECTOR2.h"

class HitEffect :
	public Obj
{
public:
	HitEffect();
	~HitEffect();

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	bool CheckDeth();
	bool CheckObjType(OBJ_TYPE type);
	void Draw(void);

private:

	VECTOR2 pos;

};

