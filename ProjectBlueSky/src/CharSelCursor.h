#pragma once
#include <array>
#include "VECTOR2.h"
#include "Obj.h"

#define CHAR_CNT_MAX (8)

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
	int charID;
	int Init(void);
	void Draw(void);

	std::array<VECTOR2, CHAR_CNT_MAX> posTbl;			// ·¬×ID‚É‘Î‰ž‚µ‚½ˆÊ’u‚ÌÃ°ÌÞÙ

};