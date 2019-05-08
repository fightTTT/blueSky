#pragma once

#include "Obj.h"

class Shot :
	public Obj
{
public:
	Shot(VECTOR2 pos, DIR dir);
	~Shot();

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	bool CheckDeth();
	bool CheckObjType(OBJ_TYPE type);

private:
	VECTOR2 ssize;
	int speed;

	bool deleteFlag;			// íœƒtƒ‰ƒO
};

