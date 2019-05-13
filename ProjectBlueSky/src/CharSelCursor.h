#pragma once
#include <array>
#include "VECTOR2.h"
#include "Obj.h"

#define CHAR_CNT_MAX (8)
#define PALYER_CNT_MAX (2)

class CharSelCursor :
	public Obj
{
public:
	CharSelCursor(PAD_ID padId);
	~CharSelCursor();

private:
	VECTOR2 sSize;
	void SetMove(const GameCtrl &ctl, weekListObj objList);		// ¶°¿Ù‚Ì“®‚«
	bool CheckObjType(OBJ_TYPE type);
	int charID;													// ‘I‘ğ‚Å‚«‚é·¬×‚ÌID
	int Init(void);
	void Draw(void);

	std::array<VECTOR2, CHAR_CNT_MAX> posTbl;					// ·¬×ID‚É‘Î‰‚µ‚½ˆÊ’u‚ÌÃ°ÌŞÙ
	std::array<unsigned int, PALYER_CNT_MAX> colorTbl;					// ÌßÚ²±°‚É‘Î‰‚µ‚½F‚ÌÃ°ÌŞÙ
};