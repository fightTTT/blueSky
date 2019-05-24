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
	int backCurID;												// ﾊﾞｯｸｼｰﾝ用ｶｰｿﾙのID
	int Init(void);
	void Draw(void);

	std::array<VECTOR2, CHAR_CNT_MAX> charSelPosTbl;					// ｷｬﾗIDに対応した位置のﾃｰﾌﾞﾙ
	std::array<VECTOR2, 2> sceneCurPosTbl;								// ｼｰﾝﾊﾞｯｸ用ﾎﾞﾀﾝのﾃｰﾌﾞﾙ
	std::array<unsigned int, PALYER_CNT_MAX> colorTbl;					// ﾌﾟﾚｲｱｰに対応した色のﾃｰﾌﾞﾙ

	bool decidFlag;			//選択決定しているかどうかのﾌﾗｸﾞ(true:決定している, false:決定していない)

	int mCount;		// ｶｰｿﾙの回転用変数
	int charCurMask;		// ｷｬﾗｾﾚｶｰｿﾙのﾏｽｸ
	int sceneCurMask;		// ｼｰﾝﾊﾞｯｸｶｰｿﾙのﾏｽｸ
	bool padIdFlag;			// どのﾊﾟｯﾄﾞが使用されたかのﾌﾗｸﾞ(2P対戦時推奨)　true = 1P, false = 2P
};