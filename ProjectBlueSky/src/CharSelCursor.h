#pragma once
#include "Obj.h"
class CharSelCursor :
	public Obj
{
public:
	CharSelCursor();
	~CharSelCursor();

private:
	VECTOR2 sSize;
	void SetMove(const GameCtrl &ctl, weekListObj objList);
	bool CheckObjType(OBJ_TYPE type);
	int Init(void);
	void Draw(void);
};