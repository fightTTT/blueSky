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
	void SetMove(const GameCtrl &ctl, weekListObj objList);		// ｶｰｿﾙの動き
	bool CheckObjType(OBJ_TYPE type);
	int charID;													// 選択できるｷｬﾗのID
	int Init(void);
	void Draw(void);

	std::array<VECTOR2, CHAR_CNT_MAX> posTbl;					// ｷｬﾗIDに対応した位置のﾃｰﾌﾞﾙ
	std::array<unsigned int, PALYER_CNT_MAX> colorTbl;					// ﾌﾟﾚｲｱｰに対応した色のﾃｰﾌﾞﾙ

	bool decidFlag;			//選択決定しているかどうかのﾌﾗｸﾞ(true:決定している, false:決定していない)
};