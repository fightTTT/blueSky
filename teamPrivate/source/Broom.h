#pragma once
#include "Obj.h"
class Broom :
	public Obj
{
public:
	Broom();
	Broom(sharedListFloor floorList, VECTOR2 pos, int padNum);
	~Broom();

	const int GetPadNum(void);

	bool initAnim(void);

	void Draw(void);

private:
	bool CheckObjType(OBJ_TYPE type);
	void SetMove(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag);
	void PosUpDate(VECTOR2& tmpPos, int maxEndPosY);		// 座標の補正や重力による影響の計算を行う
	void UniqueAttack(VECTOR2& tmpPos);					// 固有攻撃の動きの設定
	void SetNextAnim(bool animEndFlag);
	void PlaySPSe(void);
	int GetJumpCntMax(void);						// 最大ｼﾞｬﾝﾌﾟ数を得る
};

