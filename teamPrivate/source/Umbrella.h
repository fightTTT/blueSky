#pragma once
#include "Obj.h"


class Umbrella :
	public Obj
{
public:
	Umbrella();
	Umbrella(sharedListFloor floorList, VECTOR2 pos, int padNum);
	~Umbrella();

	const int GetPadNum(void);

	bool initAnim(void);

	void Draw(void);

private:
	bool CheckObjType(OBJ_TYPE type);
	void SetMove(const GameController &controller, weakListObj objList, weakListChicken chickenList, bool entryFlag);
	void UniqueAttack(VECTOR2& tmpPos);					// 固有攻撃の動きの設定
	void UniqueColGround(void);							// ｷｬﾗごとの地面や床と会ったった時の処理を行う
	void SetNextAnim(bool animEndFlag); 
	
	void PlaySPSe(void);	// 必殺技の再生
};

