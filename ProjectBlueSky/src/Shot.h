#pragma once

#include "Obj.h"
#include <array>

class Shot :
	public Obj
{
public:
	Shot(VECTOR2 pos, VECTOR2 offset, DIR dir, PAD_ID id);
	~Shot();

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	bool CheckDeth();
	bool CheckObjType(OBJ_TYPE type);
	void Draw(void);

private:
	VECTOR2 ssize;
	int speed;
	void CheckHitFlag(void);

	VECTOR2 hitBoxSize;			// “–‚½‚è”»’è‚Ì‹éŒ`‚Ì»²½Þ

	int interval;
};

